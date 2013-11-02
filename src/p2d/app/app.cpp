#include <p2d/app/app.h>
#include <p2d/app/object.h>


sad::p2d::app::App::App()
{
}

sad::p2d::app::App::~App()
{
	if (m_world)
		delete m_world;
}

void sad::p2d::app::App::initApp()
{
	m_world = NULL;
	createWorld();

	m_steptask = new p2d::WorldStepTask(m_world);
	sad::Input::ref()->addPostRenderTask(m_steptask);
}

sad::Scene * sad::p2d::app::App::scene()
{
	return sad::Renderer::ref()->getCurrentScene();
}

void sad::p2d::app::App::createWorld()
{
	m_world = new sad::p2d::World();
	m_world->setDetector(new p2d::SimpleCollisionDetector());
}

void sad::p2d::app::App::quit()
{
	sad::Renderer::ref()->quit();
}


void sad::p2d::app::App::run()
{

}

void sad::p2d::app::App::removeObject(sad::p2d::app::Object * o)
{
	sad::p2d::Body * b = o->body();
	sad::Renderer::ref()->getCurrentScene()->remove(o);
	m_world->remove(b);
}


void sad::p2d::app::App::addObject(sad::p2d::app::Object * o)
{
	o->setApp(this);
	sad::Renderer::ref()->getCurrentScene()->add(o);
	m_world->add(o->body());
}

