#include <phy52d/app.h>
#include <phy52d/object.h>

#include <renderer.h>
#include <pipeline/pipeline.h>

sad::phy52d::App::App() : m_world(nullptr), m_step_task(nullptr), m_layer(0), m_renderer(nullptr)
{
}

sad::phy52d::App::~App()
{
    if (m_world)
    {
        m_world->delRef();
    }
}

void sad::phy52d::App::initApp(unsigned int layer, sad::Renderer * r)
{
    m_layer = layer;
    // Fetch global renderer, if needed
    if (r == nullptr)
    {
        r = sad::Renderer::ref();
    }

    m_renderer = r;
    m_world = nullptr;
    createWorld();

    // Inserts world step task to an end of pipeline
    m_step_task = new p2d::WorldStepTask(m_world, r);
    m_step_task->mark("sad::p2d::WorldStepTask");
    r->pipeline()->append(m_step_task);
}

sad::Scene * sad::phy52d::App::scene() const
{
    return m_renderer->scenes()[m_layer];
}

void sad::phy52d::App::createWorld()
{
    m_world = new sad::p2d::World();
    m_world->setDetector(new p2d::SimpleCollisionDetector());
    m_world->addRef();
}

void sad::phy52d::App::quit()
{
    m_renderer->quit();
}


void sad::phy52d::App::run()
{

}

void sad::phy52d::App::removeObject(sad::phy52d::Object * o)
{
    sad::p2d::Body * b = o->body();
    m_renderer->scenes()[m_layer]->remove(o);
    m_world->removeBody(b);
}


void sad::phy52d::App::addObject(sad::phy52d::Object * o, const sad::String& additional_group)
{
    o->setApp(this);
    m_renderer->scenes()[m_layer]->add(o);
    m_world->addBody(o->body());
    if (!additional_group.empty())
    {
        m_world->addBodyToGroup(additional_group, o->body());
    }
}

sad::p2d::World* sad::phy52d::App::world() const
{
    return this->m_world;
}
