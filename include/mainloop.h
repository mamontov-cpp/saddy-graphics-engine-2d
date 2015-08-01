/*! \file mainloop.h
	

	Describes a main event loop. 
	Note, that you should not directly use this class,
	use sad::Renderer::run() instead.
 */
#pragma once

namespace sad
{
class Renderer;

namespace os
{
class SystemEventDispatcher;
}

/*! Describes a main event loop.
	Note, that you should not directly use this class,
	use sad::Renderer::run() instead.
 */
class MainLoop
{
public:
	/*! Constructs new main loop 
	 */
	MainLoop();
	/*! Frees a main loop, removing attached dispatcher
	 */
	~MainLoop();
	/*! Sets new renderer attached to a loop
		\param[in] r renderer
	 */
	virtual void setRenderer(sad::Renderer * r);
	/*! Returns a renderer attached to a loop
		\return attached renderer
	 */
	virtual sad::Renderer * renderer() const;
	/*! Runs a main loop
	 */
	void run();
	/*! Stops a main loop by setting a running flag to false
	 */
	void stop();
	/*! Returns current dispatcher for loop
		\return dispatcher for loop
	 */
	sad::os::SystemEventDispatcher *  dispatcher();
	/*! Determines, whether main loop is running
		\return whether main loop is running
     */
	bool running() const;	
protected:
	/*! Tries to elevate priority of current process, when performing main loop
	 */
	void tryElevatePriority();
	/*! Tries to set emergency handler, which should handle case, when user
		closes console window on Windows
	 */
	void trySetEmergencyShudownHandler();
	/*! Registers current renderer in a Windows platform in table of current 
		renderers. That is done because of implementation of event system on
		windows platform.
	 */
	void registerRenderer();
	/*! Unregisters current renderer in a Windows platform in table of current 
		renderers. That is done because of implementation of event system on
		windows platform.
	 */
	void unregisterRenderer();
	/*! Initializes keyboard locales on Linux, allowing to handle keyboard langugage
		switch.
	 */
	void initKeyboardInput();
	/*! Forces built-in OS scheduler switch to other processes
	 */
	void forceSchedulerSwitchToOtherProcesses();
	/*! A real unchecked main loop function.
	 */
	void perform();
	/*! Determines, whether main loop is running
	 */
	bool m_running;
	/*! A renderer, linked to loop
	 */
	sad::Renderer * m_renderer;
	/*! A system event disptacher for dispachign all events
	 */
	sad::os::SystemEventDispatcher * m_dispatcher;
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

