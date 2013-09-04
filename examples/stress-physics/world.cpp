#include "world.h"
#include "worldobject.h"
#include <orthocamera.h>
#include <texturemanager.h>
#include <input.h>
#include <extra/background.h>
#include "worldobject.h"

#include "gridnode.h"
#include "gridnodedge.h"

#include "elasticforce.h"

World::World()
{
	m_world = new p2d::World();
	m_walls = new Walls();
	m_find = new p2d::FindContactPoints();
}


World::~World()
{
	delete m_world;
	delete m_walls;
	delete m_find;
}




void World::run()
{
	

	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sc->add(new sad::Background("background"));
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("Physics stress test");


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

	// Add walls
	hst::vector<p2d::Body *> bodies = m_walls->bodies();
	for(int i = 0; i < bodies.count(); i++)
	{
		m_world->add(bodies[i]);
	}

	// Produce a grid of nodes
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
	}

	// Build elastic grid for all 9 nodes

	// TODO: Simplify
	g[1]->body()->tangentialForces().add( new ElasticForce(g[0]->body(), g[1]->body()) );
	g[1]->body()->tangentialForces().add( new ElasticForce(g[4]->body(), g[1]->body()) );
	
	g[3]->body()->tangentialForces().add( new ElasticForce(g[0]->body(), g[3]->body()) );
	g[3]->body()->tangentialForces().add( new ElasticForce(g[6]->body(), g[3]->body()) );
	

	g[4]->body()->tangentialForces().add( new ElasticForce(g[3]->body(), g[4]->body()) );
	g[4]->body()->tangentialForces().add( new ElasticForce(g[1]->body(), g[4]->body()) );
	g[4]->body()->tangentialForces().add( new ElasticForce(g[5]->body(), g[4]->body()) );
	g[4]->body()->tangentialForces().add( new ElasticForce(g[7]->body(), g[4]->body()) );

	g[5]->body()->tangentialForces().add( new ElasticForce(g[2]->body(), g[5]->body()) );
	g[5]->body()->tangentialForces().add( new ElasticForce(g[4]->body(), g[5]->body()) );
	g[5]->body()->tangentialForces().add( new ElasticForce(g[4]->body(), g[8]->body()) );

	g[6]->body()->tangentialForces().add( new ElasticForce(g[3]->body(), g[6]->body()) );
	g[6]->body()->tangentialForces().add( new ElasticForce(g[7]->body(), g[6]->body()) );

	g[7]->body()->tangentialForces().add( new ElasticForce(g[6]->body(), g[7]->body()) );
	g[7]->body()->tangentialForces().add( new ElasticForce(g[4]->body(), g[7]->body()) );
	g[7]->body()->tangentialForces().add( new ElasticForce(g[8]->body(), g[7]->body()) );

	g[8]->body()->tangentialForces().add( new ElasticForce(g[5]->body(), g[8]->body()) );
	g[8]->body()->tangentialForces().add( new ElasticForce(g[7]->body(), g[8]->body()) );
    
	sc->add( new GridNodeEdge(g[0]->body(), g[1]->body()) );
	sc->add( new GridNodeEdge(g[1]->body(), g[2]->body()) );
	sc->add( new GridNodeEdge(g[0]->body(), g[3]->body()) );
	sc->add( new GridNodeEdge(g[1]->body(), g[4]->body()) );
	sc->add( new GridNodeEdge(g[3]->body(), g[4]->body()) );
	sc->add( new GridNodeEdge(g[2]->body(), g[5]->body()) );
	sc->add( new GridNodeEdge(g[4]->body(), g[5]->body()) );
	sc->add( new GridNodeEdge(g[3]->body(), g[6]->body()) );
	sc->add( new GridNodeEdge(g[4]->body(), g[7]->body()) );
	sc->add( new GridNodeEdge(g[6]->body(), g[7]->body()) );
	sc->add( new GridNodeEdge(g[5]->body(), g[8]->body()) );
	sc->add( new GridNodeEdge(g[7]->body(), g[8]->body()) );
    
	sc->add( new World::FPSLabel() );

	for(int i = 0 ; i < 9; i++)
	{
		this->addObject(g[i]);
	}

	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}


void World::quit()
{
	sad::Renderer::ref()->quit();
}

void World::addObject(WorldObject * o)
{
	o->setWorld(this);
	sad::Renderer::ref()->getCurrentScene()->add(o);
	m_world->add(o->body());
}


void World::onWallNode(const p2d::CollisionEvent<GridNode, Wall> & ev)
{
	p2d::BoundType bt = ev.m_object_2->type();
	double x = fabs(ev.m_object_1->body()->tangentialVelocity().x());
	double y = fabs(ev.m_object_1->body()->tangentialVelocity().y());
	if (ev.m_object_2->type() == p2d::BT_LEFT)
	{
		ev.m_object_1->setHorizontalSpeed(x);
	}
	if (ev.m_object_2->type() == p2d::BT_RIGHT)
	{
		ev.m_object_1->setHorizontalSpeed(-x);
	}
	if (ev.m_object_2->type() == p2d::BT_UP)
	{
		ev.m_object_1->setVerticalSpeed(-y);
	}
	if (ev.m_object_2->type() == p2d::BT_DOWN)
	{
		ev.m_object_1->setVerticalSpeed(y);
	}
}


World::FPSLabel::FPSLabel()
{
	// Extract font with label
	this->font()=sad::Renderer::ref()->fonts()->get("times_lg");
	this->point()=pointf(0,sad::Renderer::ref()->settings().height());
	// Set rendering point to current point
	this->render_point()=this->point();
}

void World::FPSLabel::render()
{
	string() = hst::string::number(sad::Renderer::ref()->fps());
	this->Label::render();
}


void World::onNodeNode(const p2d::CollisionEvent<GridNode, GridNode> & ev)
{
	p2d::Vector av1 = ev.m_object_1->body()->averageChangeIndependentTangentialVelocity();
	p2d::Vector av2 = ev.m_object_2->body()->averageChangeIndependentTangentialVelocity();

	p2d::SetOfPointsPair pairs = m_find->invoke(ev.m_object_1->body()->currentShape(),
												av1,
												ev.m_object_2->body()->currentShape(),
												av2
											   );

	p2d::Vector dv = av1 - av2;
	double mdv = p2d::modulo(av1 - av2);
	if (pairs.size() == 1 && fabs(mdv) > 0.0000001)
	{
		// Compute time of impact
		p2d::Vector dc = pairs[0].p1() - pairs[0].p2();
		double      mdc = p2d::distance(pairs[0].p1(), pairs[0].p2());
		double time = mdc / mdv;
		// A before colliison time
		if (p2d::scalar(dc, dv) < 0)
		{
			time *= -1;
		}

		double m1 = ev.m_object_1->body()->weight().value();
		double m2 = ev.m_object_2->body()->weight().value();

		

		p2d::Point normal1 = pairs[0].p1();
		normal1 -= ev.m_object_1->body()->currentShape()->center();
		normal1 = p2d::unit(normal1);

		p2d::Point normal2 = pairs[0].p2();
		normal2 -= ev.m_object_2->body()->currentShape()->center();
		normal2 = p2d::unit(normal2);

		double project1 = p2d::scalar(ev.m_object_1->body()->tangentialVelocity(), normal1);
		p2d::Vector normalPart1 = normal1;
		normalPart1 *= project1;

		p2d::Vector tangentialPart1 = ev.m_object_1->body()->tangentialVelocity();
		tangentialPart1 -= normalPart1;

		double project2 = p2d::scalar(ev.m_object_2->body()->tangentialVelocity(), normal2);
		p2d::Vector normalPart2 = normal2;
		normalPart2 *= project2;

		p2d::Vector tangentialPart2 = ev.m_object_2->body()->tangentialVelocity();
		tangentialPart2 -= normalPart2;

		if (project1 > 0)
		{
			normalPart1 = normalPart1 * (m1 - m2) -  normalPart2 * (2 * m2);
			normalPart1 /= -(m1 + m2);
		}

		ev.m_object_1->body()->sheduleTangentialVelocity(normalPart1 + tangentialPart1);
		

		if (project2 > 0)
		{
			normalPart2 = normalPart2 * (m2 - m1) -  normalPart1 * (2 * m1);
			normalPart2 /= -(m1 + m2);
		}

		ev.m_object_2->body()->sheduleTangentialVelocity(normalPart2 + tangentialPart2);
	}		   
}


