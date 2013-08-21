#include "p2d/worldsteptask.h"


p2d::WorldStepTask::WorldStepTask(p2d::World * w, sad::Renderer * r)
: m_world(w), m_renderer(r)
{

}

void p2d::WorldStepTask::perform()
{
	m_world->step(1000.0 / (double)(m_renderer->fps()));
}
