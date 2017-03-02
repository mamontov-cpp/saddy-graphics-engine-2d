#include "sadqtmainloop.h"

#include <renderer.h>
#include <window.h>
#include <fpsinterpolation.h>
#include <pipeline/pipeline.h>

// ====================================== PUBLIC METHODS  ======================================

sad::qt::MainLoop::MainLoop()
{
	
}

sad::qt::MainLoop::~MainLoop()
{

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
	return NULL;;
}

// ====================================== PROTECTED METHODS  ======================================


void sad::qt::MainLoop::processEvents()
{
	// TODO: Implement me
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
