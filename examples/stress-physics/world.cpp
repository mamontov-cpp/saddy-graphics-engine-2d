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
}


World::~World()
{
	delete m_world;
	delete m_walls;
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
			g[i]->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -1) ) );
		}
	}

	// Build elastic grid for all 9 nodes

	// TODO: Simplify
	g[1]->body()->tangentialForces().add( new ElasticForce(g[0]->body(), g[1]->body()) );
	g[1]->body()->tangentialForces().add( new ElasticForce(g[4]->body(), g[1]->body()) );
	
	g[2]->body()->tangentialForces().add( new ElasticForce(g[1]->body(), g[2]->body()) );
	g[2]->body()->tangentialForces().add( new ElasticForce(g[5]->body(), g[2]->body()) );
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
	if (ev.m_object_2->type() == p2d::BT_LEFT || ev.m_object_2->type() == p2d::BT_RIGHT) 
	{
		ev.m_object_1->setHorizontalSpeed(ev.m_object_1->body()->tangentialVelocity().x() * -1);
	}
	else
	{
		ev.m_object_1->setVerticalSpeed(ev.m_object_1->body()->tangentialVelocity().y() * -1);
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
