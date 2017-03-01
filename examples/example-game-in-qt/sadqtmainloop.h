/*! \file sadqtmainloop.h
    
    Describes a main loop, that can be ran inside a qt event loop
 */
#pragma once
#include <mainloop.h>


namespace sad
{
	
namespace qt
{
	
class MainLoop: public sad::MainLoop
{
public:
	/*! Constructs new main loop
	 */
	MainLoop();
	/*! Frees a main loop, removing attached dispatcher
	 */
	virtual ~MainLoop();
	/*! Runs a main loop
	    \param[in] once if once, loop will be run only once
	 */
	virtual void run(bool once = false);
	/*! Returns NULL, since accessing a dispatcher is disabled for Qt
	    \return NULL
	 */
	virtual sad::os::SystemEventDispatcher *  dispatcher();
protected:
	/*! Processes events for renderer
	 */
	virtual void processEvents();
	/*! Does nothing, since Qt handles this stuff
	 */
	virtual void initKeyboardInput();
	/*! Does nothing, since Qt handles this stuff
	 */
	virtual void registerRenderer();
	/*! Does nothing, since Qt handles this stuff
	 */
	virtual void unregisterRenderer();
};

}

}

