#include <p2d/elasticforce.h>
#include <p2d/app/object.h>
#include <p2d/app/objectemitter.h>
#include <orthocamera.h>
#include <texturemanager.h>
#include <input.h>
#include <extra/background.h>

#include "world.h"
#include "platform.h"
#include "uncoloredbullet.h"
#include "ball.h"
#include "gridnode.h"
#include "gridnodedge.h"

#include "bouncesolver.h"

World::World()
{
	this->initApp();
	/*m_world->setDetector(new p2d::MultisamplingCollisionDetector(3))*/;
	m_walls = new p2d::Walls();
	m_find = new p2d::FindContactPoints();
	m_solver = new BounceSolver();
}


World::~World()
{
	delete m_walls;
	delete m_find;
	delete m_solver;
}




void World::run()
{
	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sc->add(new sad::Background("background"));
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("Physics stress test");

	sad::Renderer::ref()->controls()->setMouseMoveHandler( new sad::MethodEventHandler<sad::Event, World>(this, &World::onMouseMove));

	// Load textures to videocard memory and build mipmaps
	sad::Renderer::ref()->textures()->buildAll();
	SL_MESSAGE("Mips built successfully");	
	
	// Set window size to be fixed
	sad::Renderer::ref()->toggleFixedOn();

	m_steptask = new p2d::WorldStepTask(m_world);
	sad::Input::ref()->addPostRenderTask(m_steptask);


	// SETUP WORLD CALLBACKS HERE!!!
	m_world->addHandler(this, &World::onWallNode);
	m_world->addHandler(this, &World::onNodeNode);
	m_world->addHandler(this, &World::onWallBall);
	m_world->addHandler(this, &World::onBallNode);
	m_world->addHandler(this, &World::onWallUncoloredBullet);
	m_world->addHandler(this, &World::onBallUncoloredBullet);
	m_world->addHandler(this, &World::onNodeUncoloredBullet);
	m_world->addHandler(this, &World::onBallPlatform);
	m_world->addHandler(this, &World::onWallPlatform);

	// Add walls
	hst::vector<p2d::Body *> bodies = m_walls->bodies();
	for(int i = 0; i < bodies.count(); i++)
	{
		m_world->add(bodies[i]);
	}

	// Build elastic grid with 9 nodes

	// Produce a nodes of grids
	GridNode * g[9];
	for(int i = 0; i < 9; i++)
	{
		g[i] = new GridNode();
		g[i]->setPosition(p2d::Point(300 + (i % 3) * 100, 400 - (i / 3) * 100));
		// Add a gravity force
		if (i != 0 && i != 2) 
		{
			g[i]->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -60) ) );
		}
		else
		{
			// Make bodies unmovable
			g[i]->body()->weight().setValue(1E+6);
		}
	}

	// Sets forces, between nodes	
	int forces[20][2] =  {
		{0, 1}, {2, 1}, {4, 1},
		{0, 3}, {4, 3}, {6, 3},
		{3, 4}, {1, 4}, {5, 4}, {7, 4},
		{2, 5}, {4, 5}, {8, 5},
		{3, 6}, {7, 6},
		{6, 7}, {4, 7}, {8, 7},
		{5, 8}, {7, 8}
	};
	for(int i = 0; i < 20; i++)
	{
		int  f = forces[i][0];
		int  s = forces[i][1];
		g[s]->addTangentialForce( new p2d::ElasticForce(g[f]->body(), g[s]->body()) );
	}
	int graphic[12][2] = { 
		{0, 1}, {1, 2}, {0, 3}, {1, 4}, {3, 4}, {2, 5},
		{4, 5}, {3, 6}, {4, 7}, {6, 7}, {5, 8}, {7, 8}
	};
	// Build grafical representaion for elastic grid
	for(int i = 0; i < 12; i++)
	{
		sc->add( new GridNodeEdge(g[graphic[i][0]]->body(), g[graphic[i][1]]->body()) );
	}
    
	// Add elastic grid nodes to scene
	for(int i = 0 ; i < 9; i++)
	{
		this->addObject(g[i]);
	}

	// Add two platforms to scene
	Platform * platform1 = new Platform();
	platform1->setPosition(p2d::Point(150, 300));
	platform1->body()->setWeight(p2d::Weight::constant(1E+9));
	platform1->setTangentialVelocity(p2d::Vector(0, -5));
	this->addObject(platform1);

	Platform * platform2= new Platform();
	platform2->setPosition(p2d::Point(650, 200));
	platform2->body()->setWeight(p2d::Weight::constant(1E+9));
	platform2->setTangentialVelocity(p2d::Vector(0, -5));
	this->addObject(platform2);

	// Add ball to scene
	Ball * ball = new Ball();
	ball->setPosition(p2d::Point(40, 200));
	ball->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -30) ) );
	ball->body()->setCurrentTangentialVelocity(p2d::Vector(30, 120));
	ball->body()->setCurrentAngularVelocity(1.0);
	this->addObject(ball);

	

	// Add FPS counter
	FormattedLabel * label = new FormattedLabel();
	label->setFont("times_lg");
	label->setPoint(0, sad::Renderer::ref()->settings().height());
	label->setFormatString("{0}");
	label->setUpdateInterval(1000.0);
	label->argFPS();
	sc->add( label );

	typedef p2d::app::RandomDirectionObjectEmitter<UncoloredBullet> Emitter;
	Emitter * b = new Emitter(this);
	// Set emitting task at upper middle of screen
	b->setMinPosition(p2d::Vector(400, 570));
	b->setMaxPosition(p2d::Vector(400, 570));

	b->setMaxAngularVelocity(1.0);
	b->setMinAngularVelocity(0.0);
	
	b->setMinSpeed(p2d::Vector(-40, -100));
	b->setMaxSpeed(p2d::Vector(40, -100));
	
	b->setInterval(200);
	b->setMaxCount(3);
	// Added periodical task
	sad::Renderer::ref()->controls()->addPreRenderTask( new TimePeriodicalTask(b) );
	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}


void World::quit()
{
	sad::Renderer::ref()->quit();
}


/*! Handled this type of collision with manual from
	
 */
void World::onWallNode(const p2d::CollisionEvent<GridNode, p2d::Wall> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}



void World::onNodeNode(const p2d::CollisionEvent<GridNode, GridNode> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}


void World::onWallBall(const p2d::CollisionEvent<Ball, p2d::Wall> & ev)
{
	m_solver->pushRotationFriction(0.0001, 1);
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}

void World::onBallNode(const p2d::CollisionEvent<Ball, GridNode> & ev)
{
	m_solver->pushRotationFriction(0.0001, 1);
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}

void World::onWallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, p2d::Wall> & ev)
{
	this->removeObject(ev.m_object_1);
}


void World::onBallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, Ball> & ev)
{
	this->removeObject(ev.m_object_1);
}

void World::onNodeUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, GridNode> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}

void World::onBallPlatform(const p2d::CollisionEvent<Ball, Platform> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}

void World::onMouseMove(const sad::Event & ev)
{
	//SL_DEBUG(fmt::Format("{0} {1}") << ev.x << ev.y);
}


void World::onWallPlatform(const p2d::CollisionEvent<p2d::Wall, Platform> & ev)
{
	m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
}
