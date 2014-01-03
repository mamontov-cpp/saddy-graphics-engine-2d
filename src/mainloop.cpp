#include "mainloop.h"

#include "renderer.h"
#include "window.h"
#include "glcontext.h"
#include "sadsleep.h"
#include "fpsinterpolation.h"
#include "pipeline/pipeline.h"

#include "os/windowhandles.h"
#include "os/systemwindowevent.h"
#include "os/systemeventdispatcher.h"

#include "log/log.h"


#ifdef LINUX
#include <unistd.h>
#endif

sad::MainLoop::MainLoop() : 
m_renderer(NULL),
m_running(false),
m_dispatcher(new sad::os::SystemEventDispatcher())
{

}

sad::MainLoop::~MainLoop()
{
	delete m_dispatcher;
}

void sad::MainLoop::setRenderer(sad::Renderer * r)
{
	m_renderer = r;
	m_dispatcher->setRenderer(r);
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
	initKeyboardInput();
	perform();
	unregisterRenderer();
}

void sad::MainLoop::stop()
{
	m_running = false;
}


sad::os::SystemEventDispatcher *  sad::MainLoop::dispatcher()
{
	return m_dispatcher;
}

bool sad::MainLoop::running() const
{
	return m_running;
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

void sad::MainLoop::initKeyboardInput()
{
#ifdef X11
	setlocale(LC_CTYPE, "");
	XSetLocaleModifiers("");
#endif
}


void sad::MainLoop::forceSchedulerSwitchToOtherProcesses()
{
#ifdef WIN32
			sad::sleep(50);
#endif

#ifdef LINUX
			sched_yield();
#endif	
}


#ifdef WIN32

LRESULT CALLBACK sad_renderer_window_proc (HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool handled = false;
	RegisteredRenderersLock.lock();
	sad::os::SystemWindowEventDispatchResult result;
	if (RegisteredRenderers.contains(wnd))
	{
		sad::os::SystemWindowEvent e(wnd, msg, wparam, lparam);
		const sad::Renderer * r = RegisteredRenderers[wnd];
		result = r->mainLoop()->dispatcher()->dispatch(e);
	}
	RegisteredRenderersLock.unlock();
	if (result.exists())
	{
		return (LRESULT)(result.value());
	}

	return DefWindowProcA(wnd, msg, wparam, lparam);				
}

#endif

#ifdef X11
/*! Predicate for capturing all of X11 events
 *  \return true
 */
static int predicate(Display *, XEvent * e, char *)
{
	return true;
}
#endif

void sad::MainLoop::perform()
{
	m_running = true;
	this->m_renderer->window()->setActive(true);
	this->m_renderer->fpsInterpolation()->reset();
	m_dispatcher->reset();

#ifdef WIN32
	MSG msg;
#endif

#ifdef X11
	sad::os::SystemWindowEvent msg;
#endif
	while(m_running)
	{
#ifdef WIN32
		// Fetch window events. We only fetch one event from queue
		// to reduce slowdown from all event handling 
		if (PeekMessage (
						 &msg, 
						 // A PeekMessage docs state, that multithreading
						 // should work with zero, since sad::Renderer-s must
						 // be running at separate threads. Also, moving here
						 // a handle to window  causes problems with switching
						 // keyboard layout on Windows XP
						 0
						 /*this->window()->handles()->WND*/, 
						 0, 
						 0, 
						 PM_REMOVE
						) != 0
		   )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
#endif

#ifdef X11
		// In fact in linux we get big slowdown if 
		// all events is not dispatched
		while (XCheckIfEvent(m_renderer->window()->handles()->Dpy, &(msg.Event), predicate, NULL) == True)
		{
			m_dispatcher->dispatch(msg);
		}
#endif
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
	}
	this->m_renderer->window()->setActive(false);
	m_running = false;
}

