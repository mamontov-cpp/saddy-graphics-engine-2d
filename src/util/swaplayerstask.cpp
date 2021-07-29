#include "util/swaplayerstask.h"

#include "renderer.h"

sad::util::SwapLayersTask::SwapLayersTask(sad::Renderer* r, sad::Scene * s1, sad::Scene * s2, int l1, int l2)
: m_renderer(r), m_scene1(s1), m_scene2(s2), m_layer1(l1) , m_layer2(l2)
{
    this->setSource(sad::pipeline::StepSource::ST_USER);
}

void sad::util::SwapLayersTask::_process()
{
    m_renderer->m_scenes[m_layer1] = m_scene2;
    m_renderer->m_scenes[m_layer2] = m_scene1;
}


sad::util::SwapLayersTask::~SwapLayersTask()
{

}