#include "objectdependentfpsinterpolation.h"
#include "renderer.h"


sad::ObjectDependentFPSInterpolation::ObjectDependentFPSInterpolation(): 
m_renderer(nullptr),
m_total_renderer_items(0)
{
    m_interval_per_item.clear();
    reset();
}

sad::ObjectDependentFPSInterpolation::~ObjectDependentFPSInterpolation()
{
    
}

void sad::ObjectDependentFPSInterpolation::reset()
{
    m_fps = 75;
    m_is_set_immediately = true;
    m_reset =  false;
    m_frames = 0;
    
    m_total_renderer_items = 0;
    m_interval_per_item.setValue(0);
}


void sad::ObjectDependentFPSInterpolation::start()
{
    if (m_is_set_immediately || m_reset)
    {
        m_timer.start();
        m_reset = false;
    }
}

void sad::ObjectDependentFPSInterpolation::stop()
{
    ++m_frames;
    m_timer.stop();
    double elapsed = m_timer.elapsed();
    if (m_renderer)
    {
        m_total_renderer_items = std::max(m_total_renderer_items, m_renderer->totalSceneObjects());
    }
    if (m_is_set_immediately || elapsed > 100.0)
    {
        double newfps = 1000.0 * m_frames / elapsed; 
        m_fps =  newfps;
        if (m_total_renderer_items)
        {
            //printf("Setting time for %lf %u %u\n", elapsed, m_frames, m_total_renderer_items);
            m_interval_per_item.setValue( elapsed / m_frames / m_total_renderer_items );
        }
        else
        {
            m_interval_per_item.clear();
        }
        m_frames = 0;
        m_reset = true;
        m_is_set_immediately = false;
        m_total_renderer_items = 0;
    }
}

void sad::ObjectDependentFPSInterpolation::resetTimer()
{
    m_timer.start();
}

double sad::ObjectDependentFPSInterpolation::fps()
{
    if (m_renderer && m_interval_per_item.exists())
    {
        unsigned int totalitems = m_renderer->totalSceneObjects();
        if (totalitems)
        {
            double framerenderingtime = (double)totalitems * m_interval_per_item.value();
            double result = 1000.0 / (framerenderingtime);
            //printf("%u %lf %lf %lf\n", totalitems, m_interval_per_item.value(), result, framerenderingtime);
            return result;
        }
    }
    return m_fps;
}

void sad::ObjectDependentFPSInterpolation::setRenderer(sad::Renderer * renderer)
{
    m_renderer = renderer;
}
