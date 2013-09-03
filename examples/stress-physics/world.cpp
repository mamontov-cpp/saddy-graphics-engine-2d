#include "world.h"
#include <orthocamera.h>
#include <texturemanager.h>
#include <extra/background.h>


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

	// Add walls
	hst::vector<p2d::Body *> bodies = m_walls->bodies();
	for(int i = 0; i < bodies.count(); i++)
	{
		m_world->add(bodies[i]);
	}

	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}


void World::quit()
{
	sad::Renderer::ref()->quit();
}
