#include "fpsinterpolation.h"

sad::FPSInterpolation::FPSInterpolation()
{
    reset();
}

sad::FPSInterpolation::~FPSInterpolation()
{
    
}

void sad::FPSInterpolation::reset()
{
    m_fps = 75;
    m_setimmediately = true;
    m_reset =  false;
    m_frames = 0;
}


void sad::FPSInterpolation::start()
{
    if (m_setimmediately || m_reset)
    {
        m_timer.start();
        m_reset = false;
    }
}

void sad::FPSInterpolation::stop()
{
    ++m_frames;
    m_timer.stop();
    double elapsed = m_timer.elapsed();
    if (m_setimmediately || elapsed > 500.0)
    {
        double newfps = 1000.0 * m_frames / elapsed; 
        m_fps =  newfps * 0.8 + m_fps * 0.2;
        m_frames = 0;
        m_reset = true;
        m_setimmediately = false;
    }
}

void sad::FPSInterpolation::resetTimer()
{
    m_timer.start();
}

double sad::FPSInterpolation::fps()
{
    return m_fps;
}

