/*! \file mainloop.h
    

    Describes a main event loop. 
    Note, that you should not directly use this class,
    use sad::Renderer::run() instead.
 */
#pragma once
#ifdef _WIN32
    #include <functional>
    #include <sadvector.h>
    #include <sadmutex.h>
#endif

namespace sad
{
class Renderer;

namespace os
{
class SystemEventDispatcher;
}
/*!
 * A value, so main loop will be run only once
 */
#define SAD_MAIN_LOOP_RUN_ONLY_ONCE  (true)

/*! Describes a main event loop.
    Note, that you should not directly use this class,
    use sad::Renderer::run() instead.
 */
class MainLoop //-V690
{
public:
    /*! Constructs new main loop 
     */
    MainLoop();
    /*! Frees a main loop, removing attached dispatcher
     */
    virtual ~MainLoop();
    /*! Sets new renderer attached to a loop
        \param[in] r renderer
     */
    virtual void setRenderer(sad::Renderer * r);
    /*! Returns a renderer attached to a loop
        \return attached renderer
     */
    virtual sad::Renderer * renderer() const;
    /*! Runs a main loop
        \param[in] once if once, loop will be run only once
     */
    virtual void run(bool once = false);
    /*! Stops a main loop by setting a running flag to false
     */
    void stop();
    /*! Returns current dispatcher for loop
        \return dispatcher for loop
     */
    virtual sad::os::SystemEventDispatcher *  dispatcher();
    /*! Determines, whether main loop is running
        \return whether main loop is running
     */
    bool running() const;
    /*! Should called in run() before perform() to perform crucial system settings
     */
    virtual void initMainLoop();
    /*! Deinits main loop after finishing it
     */
    virtual void deinitMainLoop();
#ifdef _WIN32
    /*! Pushes dispatches for main loop
        \param[in] f function
     */
    void pushDispatch(const std::function<void()>& f);
    /*! Runs and cleans dispatches for main loop
     */
    void runAndCleanDispatches();
#endif
protected:
    /*! Tries to elevate priority of current process, when performing main loop
     */
    virtual void tryElevatePriority();
    /*! Tries to set emergency handler, which should handle case, when user
        closes console window on Windows
     */
    void trySetEmergencyShudownHandler();
    /*! Registers current renderer in a Windows platform in table of current 
        renderers. That is done because of implementation of event system on
        windows platform.
     */
    virtual void registerRenderer();
    /*! Unregisters current renderer in a Windows platform in table of current 
        renderers. That is done because of implementation of event system on
        windows platform.
     */
    virtual void unregisterRenderer();
    /*! Initializes keyboard locales on Linux, allowing to handle keyboard langugage
        switch.
     */
    virtual void initKeyboardInput();
    /*! Forces built-in OS scheduler switch to other processes
     */
    void forceSchedulerSwitchToOtherProcesses();
    /*! Determines, whether main loop is running
     */
    bool m_running;
    /*! A renderer, linked to loop
     */
    sad::Renderer * m_renderer;
    /*! A system event disptacher for dispachign all events
     */
    sad::os::SystemEventDispatcher * m_dispatcher;
#ifdef _WIN32
    sad::Vector<std::function<void()> > m_event_dispatches;
    /*! An event dispatches lock
     */
    sad::Mutex m_event_dispatches_lock;
#endif
private:
    /*! Disabled to made main loop non-copyable
        \param[in] o other main loop
     */
    MainLoop(const sad::MainLoop & o);
    /*! Disabled to made main loop non-copyable
        \param[in] o other main loop
        \return this
     */
    const MainLoop & operator=(const sad::MainLoop & o);
};

}

