#include "p2d/worldsteptask.h"


p2d::WorldStepTask::WorldStepTask(p2d::World * w, sad::Renderer * r)
: m_world(w), m_renderer(r), m_enabled(true)
{

}

clock_t debugtime = 0;

void p2d::WorldStepTask::perform()
{
	// 1.0 is a second, so if 1 frame at 1s, we will step second
	SL_DEBUG( fmt::Format("time: {0} ms") << (clock() - debugtime) * 1000.0 / CLOCKS_PER_SEC );
	if (m_enabled)
		m_world->step(1.0 / m_renderer->fps());
	debugtime = clock();
}


void p2d::WorldStepTask::setWorld(p2d::World * world)
{
	m_world = world;
}

void p2d::WorldStepTask::enable()
{
	m_enabled = true;
}


void p2d::WorldStepTask::disable()
{
	m_enabled = false;
}

bool p2d::WorldStepTask::enabled() const
{
	return m_enabled;
}

