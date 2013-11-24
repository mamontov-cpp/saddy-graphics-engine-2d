/*! \file systemeventdispatcher.h
	\author HiddenSeeker

	Defines a dispatcher, which dispatches a system events
 */
#pragma once
#include "../maybe.h"
#include "../sadsize.h"

namespace sad
{

class Renderer;

namespace os
{

class SystemWindowEvent;
class KeyDecoder;

/*! Determines, whether event is dispatched, and also result code for
	this dispatch operation, when it's needed to be provided to main loop
 */
typedef sad::Maybe<long> SystemWindowEventDispatchResult;

/*! A system event dispatcher is a dispatcher, which dispatches
	window system events. Also it's used to handle some corner cases, which
	we shouldn't handle by input system but should handle to make other API stuff 
	useful (like fixed size windows on Windows).
 */ 
class SystemEventDispatcher
{
public:
	/*! Creates default event dispatcher
	 */
	SystemEventDispatcher();
	/*! Destroys a dispatcher
	 */
	~SystemEventDispatcher();
	/*! Sets attached renderer to a dispatcher
	 */
	void setRenderer(sad::Renderer * renderer);
	/*! Returns renderer, attached to dispatcher
	 */
	sad::Renderer * renderer() const;
	/*! Starts new iteration of main loop for working with window properties.
		Must be called at start of main loop
	 */
	void reset();

	/*! Dispatches an event
		\param[in] e event, that is being dispatched
		\return result of event, that was dispatched
	 */
	sad::os::SystemWindowEventDispatchResult dispatch(SystemWindowEvent & e);
protected:
	/*! Attached renderer to a dispatcher
	 */
	sad::Renderer * m_renderer;
	/*! A decoder for decoding keys of data
	 */
	sad::os::KeyDecoder * m_keys;
};

}

}

