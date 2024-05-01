#include "p2d/worldsteptask.h"
#include "opticksupport.h"


sad::p2d::WorldStepTask::WorldStepTask(sad::p2d::World * w, sad::Renderer * r)
: m_renderer(r), m_world(w)
{
    PROFILER_EVENT;
    if (w)
    {
        w->addRef();
    }
}

void sad::p2d::WorldStepTask::_process()
{
    PROFILER_EVENT;
    if (m_enabled)
    {
        if (m_world)
        {
            m_world->step(this->stepTick());
        }
    }
}

double sad::p2d::WorldStepTask::stepTick() const
{
    PROFILER_EVENT;
    if (!m_renderer)
    {
        return 1.0 / 60.0;
    }
    // 1.0 is a second, so if 1 frame at 1s, we will step second
    const double fps = m_renderer->fps();
    double render_time = 1.0 / fps;
    // If rendering goes extremely slow, like 5 FPS per sec
    // everything can broke, So we avoid this, by setting render time to
    // normal. Also we can't obviously use multiple steps, because
    // it could lead us to spiral of death problem
    if (render_time >= 200.0)
    {
        render_time = 1.0 / 60.0;
    }
    return render_time;
}

void sad::p2d::WorldStepTask::setWorld(sad::p2d::World * world)
{
    PROFILER_EVENT;
    if (m_world)
    {
        m_world->delRef();
    }
    m_world = world;
    if (m_world)
    {
        m_world->addRef();
    }
}

sad::p2d::WorldStepTask::~WorldStepTask()
{
    PROFILER_EVENT;
    if (m_world)
    {
        m_world->delRef();
    }
}
