#include "qt/sadqtmainloop.h"

#include <renderer.h>
#include <window.h>
#include <fpsinterpolation.h>
#include <pipeline/pipeline.h>

#include <input/controls.h>

// ====================================== PUBLIC METHODS  ======================================

sad::qt::MainLoop::MainLoop()
{
    
}

sad::qt::MainLoop::~MainLoop()
{
    for(size_t i = 0; i < m_events.size(); i++)
    {
        delete m_events[i].p2();
    }
}

void sad::qt::MainLoop::run(bool once)
{
    m_running = true;
    if (!once)
    {
        this->m_renderer->fpsInterpolation()->reset();
    }
    while (m_running)
    {	
        this->processEvents();
        // Try render scene if can
        if (this->m_renderer->window()->hidden() == false
            && this->m_renderer->window()->minimized() == false
            && m_running)
        {
            this->m_renderer->pipeline()->run();
        }
        else
        {
            this->m_renderer->fpsInterpolation()->resetTimer();
            this->forceSchedulerSwitchToOtherProcesses();
        }
        if (once)
        {
            m_running = false;
        }
    }
    m_running = false;
}

sad::os::SystemEventDispatcher *  sad::qt::MainLoop::dispatcher()
{
    return nullptr;;
}

void sad::qt::MainLoop::submitEvent(sad::input::EventType t, sad::input::AbstractEvent* ev, bool now)
{
    if (!now)
    {
        m_events << sad::Pair<sad::input::EventType, sad::input::AbstractEvent*>(t, ev);
    }
    else
    {
        if (m_renderer)
        {
            m_renderer->controls()->postEvent(t, *ev);
            delete ev;
        }
    }
}

// ====================================== PROTECTED METHODS  ======================================


void sad::qt::MainLoop::processEvents()
{
    if (m_renderer && m_events.size())
    {
        for(size_t i = 0; i < m_events.size(); i++)
        {			
            m_renderer->controls()->postEvent(m_events[i].p1(), *(m_events[i].p2()));
            delete m_events[i].p2();			
        }
        m_events.clear();
    }
}


void sad::qt::MainLoop::initKeyboardInput()
{
    
}


void sad::qt::MainLoop::registerRenderer()
{

}

void sad::qt::MainLoop::unregisterRenderer()
{

}
