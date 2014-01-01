#include <p2d/app/app.h>
#include <p2d/app/object.h>

#include <renderer.h>
#include <pipeline/pipeline.h>

sad::p2d::app::App::App()
{
}

sad::p2d::app::App::~App()
{
	if (m_world)
		delete m_world;
}

void sad::p2d::app::App::initApp(sad::Renderer * r)
{
	// Fetch global renderer, if needed
	if (r == NULL)
	{
		r = sad::Renderer::ref();
	}

	m_world = NULL;
	createWorld();

	// Insert world step task to an end of pipeline
	m_steptask = new p2d::WorldStepTask(m_world, r);
	m_steptask->mark("p2d::WorldStepTask::perform");
	m_steptask->setSource(sad::pipeline::ST_USER);
	r->pipeline()->insertStep(sad::pipeline::PIT_END, m_steptask);
}

sad::Scene * sad::p2d::app::App::scene()
{
	return sad::Renderer::ref()->scene();
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
	sad::Renderer::ref()->scene()->remove(o);
	m_world->remove(b);
}


void sad::p2d::app::App::addObject(sad::p2d::app::Object * o)
{
	o->setApp(this);
	sad::Renderer::ref()->scene()->add(o);
	m_world->add(o->body());
}

