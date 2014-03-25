#include "p2d/worldsteptask.h"


sad::p2d::WorldStepTask::WorldStepTask(sad::p2d::World * w, sad::Renderer * r)
: m_world(w), m_renderer(r)
{

}

void sad::p2d::WorldStepTask::_process()
{
	// 1.0 is a second, so if 1 frame at 1s, we will step second
	double rendertime = 1.0 / m_renderer->fps();
	// If rendering goes extremely slow, like 5 FPS per sec
	// everything can broke, So we avoid this, by setting rendertime to
	// normal. Also we can't obviously use multiple steps, because
	// it could lead us to spiral of death problem
	if (rendertime >= 200.0)
	{
		rendertime = 1.0 / 60.0; 
	}
	if (m_enabled)
		m_world->step(rendertime);
}

void sad::p2d::WorldStepTask::setWorld(p2d::World * world)
{
	m_world = world;
}
