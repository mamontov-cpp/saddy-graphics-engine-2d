/*! \file controls.h
	\author HiddenSeeker

	A basic implementation for controls, which hold a event handlers, which handle queued events
 */
#pragma once
#include "events.h"
#include "handlers.h"
#include "../sadptrvector.h"

namespace sad
{

namespace input
{

/*! A controls, which hold a event handlers, which handle queued events.
	
	Note that controls DOES NOT provide type-checking of events and handlers.
	This is done, due to perfomance issues, caused by typecasting.
 */
class Controls
{
public:
	/*! Creates new empty controls with callbacks
	 */
	Controls();

	/*! Posts event to controls, running callbacks, determined by event type,  with argument of event e
		\param[in] type type of event, which is run
		\param[in] e event which is being emited
	 */
	void postEvent(EventType type, const sad::input::AbstractEvent & e);
protected:
	/*! A shortcut for list of handlers
	 */
	typedef sad::PtrVector<sad::input::AbstractHandler>  HandlerList;
	/*! A handlers, attached to a events
	 */
	HandlerList m_handlers[SAD_INPUT_EVENTTYPE_COUNT];
private:
	/*! This object is non-copyable, this is not implemented
		\param[in] o other controls object
	 */
	Controls(const sad::input::Controls & o);
	/*! This object is non-copyable, this is not implemented
		\param[in] o other controls object
		\return self-reference
	 */
	Controls& operator=(const sad::input::Controls & o);
};

}

}
