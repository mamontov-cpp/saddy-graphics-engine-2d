#include <p2d/app/app.h>
#include <p2d/app/object.h>

using namespace sad;

p2d::app::App::App()
{
}

p2d::app::App::~App()
{
	if (m_world)
		delete m_world;
}

void p2d::app::App::initApp()
{
	m_world = NULL;
	createWorld();

	m_steptask = new p2d::WorldStepTask(m_world);
	sad::Input::ref()->addPostRenderTask(m_steptask);
}

sad::Scene * p2d::app::App::scene()
{
	return sad::Renderer::ref()->getCurrentScene();
}

void p2d::app::App::createWorld()
{
	m_world = new p2d::World();
	m_world->setDetector(new p2d::SimpleCollisionDetector());
}

void p2d::app::App::quit()
{
	sad::Renderer::ref()->quit();
}


void p2d::app::App::run()
{

}

void p2d::app::App::removeObject(p2d::app::Object * o)
{
	p2d::Body * b = o->body();
	sad::Renderer::ref()->getCurrentScene()->remove(o);
	m_world->remove(b);
}


void p2d::app::App::addObject(p2d::app::Object * o)
{
	o->setApp(this);
	sad::Renderer::ref()->getCurrentScene()->add(o);
	m_world->add(o->body());
}

