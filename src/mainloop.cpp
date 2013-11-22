#include "mainloop.h"

#include "renderer.h"
#include "window.h"
#include "glcontext.h"
#include "os/windowhandles.h"
#include "log/log.h"
#include "sadsleep.h"

sad::MainLoop::MainLoop() : m_renderer(NULL)
{

}

void sad::MainLoop::setRenderer(sad::Renderer * r)
{
	m_renderer = r;
}

sad::Renderer * sad::MainLoop::renderer() const
{
	return m_renderer;
}


void sad::MainLoop::run()
{
	tryElevatePriority();
	trySetEmergencyShudownHandler();
	registerRenderer();
	perform();
	unregisterRenderer();
}

void sad::MainLoop::tryElevatePriority()
{
	if (this->renderer() == NULL) 
	{
		return;
	}
	SL_COND_INTERNAL_SCOPE("sad::MainLoop::tryElevatePriority()", this->renderer());

	bool ok = false;
#ifdef WIN32
	ok = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) == TRUE;
#endif

#ifdef LINUX
	pid_t myprocesspid = getpid();
	sched_param param;
	param.sched_priority = 77; // Don't set too much, since we still may need to switch to other windows	
	ok = sched_setscheduler(myprocesspid, SCHED_FIFO, &param) == 0;
#endif
	if (ok == false)
	{
		SL_COND_LOCAL_INTERNAL("Failed to elevate priority", this->renderer());
	}
}

void sad::MainLoop::yield()
{
#ifdef WIN32
	sad::sleep(50);
#endif

#ifdef LINUX
	sched_yield();
#endif
}

#ifdef WIN32

typedef sad::Hash<HWND, sad::Renderer *> RegisteredRenderersTable;
static  RegisteredRenderersTable RegisteredRenderers;
static  sad::Mutex RegisteredRenderersLock;

// This function should handle every console close event
static int WINAPI  handle_console_close_event(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		for(RegisteredRenderersTable::iterator it = RegisteredRenderers.begin();
			it != RegisteredRenderers.end();
			it++)
		{
			it.value()->emergencyShutdown();
		}
	}
	return FALSE;
}

#endif

void sad::MainLoop::trySetEmergencyShudownHandler()
{
#ifdef WIN32
	SetConsoleCtrlHandler(handle_console_close_event, FALSE);
	SetConsoleCtrlHandler(handle_console_close_event, TRUE);
#endif
}


void sad::MainLoop::registerRenderer()
{
	if (this->renderer() == NULL) 
	{
		return;
	}

#ifdef WIN32
	RegisteredRenderersLock.lock();
	HWND WND = this->renderer()->window()->handles()->WND;
	if (RegisteredRenderers.contains(WND)) 
	{
		RegisteredRenderers[WND] =  this->renderer();
	}
	else
	{
		RegisteredRenderers.insert(WND,  this->renderer());
	}
	RegisteredRenderersLock.unlock();
#endif
}

void sad::MainLoop::unregisterRenderer()
{
	if (this->renderer() == NULL) 
	{
		return;
	}

#ifdef WIN32
	RegisteredRenderersLock.lock();
	HWND WND = this->renderer()->window()->handles()->WND;
	RegisteredRenderers.remove(WND);
	RegisteredRenderersLock.unlock();
#endif
}
