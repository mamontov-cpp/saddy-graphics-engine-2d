/*! \file mainloop.h
	\author HiddenSeeker

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
protected:
	/*! Tries to elevate priority of current process, when performing main loop
	 */
	void tryElevatePriority();
	/*! Yields execution, giving other processes time to run
	 */
	void yield();
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

