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

#define EVENT_LOGGING

#ifdef LINUX
#include <unistd.h>
#include <sched.h>
#endif

#ifdef X11
/*! Predicate for capturing all of X11 events
 *  \return true
 */
static int predicate(Display *, XEvent * e, char *);
#endif

sad::MainLoop::MainLoop() : 
m_renderer(nullptr),
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


void sad::MainLoop::run(bool once)
{
    m_running = true;
    this->m_renderer->window()->setActive(true);
    if (!once)
    {
        this->m_renderer->fpsInterpolation()->reset();
    }
    m_dispatcher->reset();

#ifdef WIN32
    MSG msg;
    //POINT cursorpos;
    //RECT  windowrect;
#endif

#ifdef X11
    sad::os::SystemWindowEvent msg;
#endif
    while (m_running)
    {
#ifdef WIN32
        // There was some kind of bug, when mouse leave was not generated
        // If this occurs one more time try uncommenting this code
        /*
        GetWindowRect(m_renderer->window()->handles()->WND, &windowrect);
        GetCursorPos(&cursorpos);
        if (!PtInRect(&windowrect, cursorpos) && m_dispatcher->m_is_in_window)
        {
            #ifdef EVENT_LOGGING
                SL_COND_LOCAL_INTERNAL("Cursor pos is outside of window, posting MouseLeave", m_renderer);
            #endif
                sad::os::SystemWindowEvent ev(
                m_renderer->window()->handles()->WND,
                WM_MOUSELEAVE,
                0,
                0
            );
            m_dispatcher->dispatch(ev);
        }
        */
        while (PeekMessage(
            &msg,
            // A PeekMessage docs state, that multithreading
            // should work with zero, since sad::Renderer-s must
            // be running at separate threads. Also, moving here
            // a handle to window  causes problems with switching
            // keyboard layout on Windows XP
            0
            /*m_renderer->window()->handles()->WND*/,
            0,
            0,
            PM_REMOVE
        ) != 0
            )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Dispatch windows events
        this->runAndCleanDispatches();
#endif

#ifdef X11
        // In fact in linux we get big slowdown if 
        // all events is not dispatched
        while (XCheckIfEvent(m_renderer->window()->handles()->Dpy, &(msg.Event), predicate, nullptr) != False)
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
#ifdef X11
            XFlush(m_renderer->window()->handles()->Dpy);
#endif
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
    this->m_renderer->window()->setActive(false);
    m_running = false;
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

// ========================================================= PROTECTED METHODS =========================================================

void sad::MainLoop::initMainLoop()
{
    tryElevatePriority();
    trySetEmergencyShutdownHandler();
    registerRenderer();
    initKeyboardInput();
}

void sad::MainLoop::deinitMainLoop()
{
    unregisterRenderer();
}

#ifdef _WIN32

void sad::MainLoop::pushDispatch(const std::function<void()>& f)
{
    this->m_event_dispatches_lock.lock();
    this->m_event_dispatches << f;
    this->m_event_dispatches_lock.unlock();
}

void sad::MainLoop::runAndCleanDispatches()
{
    this->m_event_dispatches_lock.lock();
    for(size_t i = 0; i < this->m_event_dispatches.size(); i++)
    {
        this->m_event_dispatches[i]();
    }
    this->m_event_dispatches.clear();
    this->m_event_dispatches_lock.unlock();

}

#endif

void sad::MainLoop::tryElevatePriority()
{
    if (this->renderer() == nullptr) 
    {
        return;
    }
    SL_COND_INTERNAL_SCOPE("sad::MainLoop::tryElevatePriority()", this->renderer());

    bool ok = false;
    bool affinityresult = false;
#ifdef WIN32
    ok = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) != FALSE;
    affinityresult = SetThreadIdealProcessor(GetCurrentThread(), 1) != static_cast<DWORD>(-1);
    DWORD   dwLastError = ::GetLastError();
    // See http://stackoverflow.com/questions/3006229/get-a-text-from-the-error-code-returns-from-the-getlasterror-function
    // Copies from here
    char   lpBuffer[256] = "?";
    ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
                    nullptr,                                      // No string to be formatted needed
                    dwLastError,                               // Hey Windows: Please explain this error!
                    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language
                    lpBuffer,              // Put the message here
                    255,                     // Number of bytes to store the message
                    nullptr);
    sad::String affinityerror = lpBuffer;
    if (!affinityresult)
    {
        SL_COND_LOCAL_INTERNAL("Error in setting affinity", this->renderer());  
        SL_COND_LOCAL_INTERNAL(affinityerror, this->renderer());
    }
#endif

#ifdef LINUX
    pid_t myprocesspid = getpid();
    sched_param param;
    param.sched_priority = 77; // Don't set too much, since we still may need to switch to other windows	
    ok = sched_setscheduler(myprocesspid, SCHED_FIFO, &param) == 0;

    {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(0, &set);
        CPU_SET(1, &set);
        affinityresult  = (sched_setaffinity(myprocesspid, sizeof(set), &set) == 0);
    }
#endif
    if (ok == false)
    {
        SL_COND_LOCAL_INTERNAL("Failed to elevate priority", this->renderer());
    }
    if (affinityresult == false)
    {
        SL_COND_LOCAL_INTERNAL("Failed to set affinity", this->renderer());        
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

void sad::MainLoop::trySetEmergencyShutdownHandler()
{
#ifdef WIN32
    SetConsoleCtrlHandler(handle_console_close_event, FALSE);
    SetConsoleCtrlHandler(handle_console_close_event, TRUE);
#endif
}


void sad::MainLoop::registerRenderer()
{
    if (this->renderer() == nullptr) 
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
    if (this->renderer() == nullptr) 
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

#ifdef X11
    static char * main_loop_locale_block = nullptr;
    sad::Mutex  main_loop_locale_mtx;

    void main_loop_destroy_modifiers()
    {
        free(main_loop_locale_block);
    }
#endif

void sad::MainLoop::initKeyboardInput()
{
#ifdef X11
    setlocale(LC_CTYPE, "");
    main_loop_locale_mtx.lock();
    char* oldvalue =  main_loop_locale_block;
    main_loop_locale_block = XSetLocaleModifiers("");
    if (main_loop_locale_block != nullptr &&  oldvalue == nullptr)
    {
        atexit(main_loop_destroy_modifiers);
    }
    main_loop_locale_mtx.unlock();
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
