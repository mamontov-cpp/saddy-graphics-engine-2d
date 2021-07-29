#include "fpsinterpolation.h"
#include <iostream>

const double sad::FPSInterpolation::WarmupTime = 500;

const double sad::FPSInterpolation::RegistrationTime = 500;

sad::FPSInterpolation::FPSInterpolation()
{
    reset();
}

sad::FPSInterpolation::~FPSInterpolation()
= default;

void sad::FPSInterpolation::reset()
{
    m_fps = 75;
    m_is_set_immediately = true;
    m_reset =  false;
    m_frames = 0;
}


void sad::FPSInterpolation::start()
{
    if (m_is_set_immediately)
    {
        m_warmup_timer.start();
        m_reset = true;
        m_is_set_immediately = false;
    }
    if (m_reset)
    {
        m_timer.start();
        m_reset = false;
    }
}

void sad::FPSInterpolation::stop()
{
    ++m_frames;
    m_timer.stop();
    m_warmup_timer.stop();
    double elapsed = m_timer.elapsed();
    if (m_warmup_timer.elapsed() <= sad::FPSInterpolation::WarmupTime)
    {
        double newfps = 1000.0 * m_frames / elapsed; 
        m_fps = newfps;
        m_frames = 0;
        m_reset = true;
    }
    else 
    {  
        if (elapsed > sad::FPSInterpolation::RegistrationTime)
        {
            double newfps = 1000.0 * m_frames / elapsed; 
            m_fps =  newfps * 0.8 + m_fps * 0.2;
            m_frames = 0;
            m_reset = true;
        }
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

