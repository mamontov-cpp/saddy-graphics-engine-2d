/*! \file controls.h
	\author HiddenSeeker

	A basic implementation for controls, which hold a event handlers, which handle queued events
 */
#pragma once
#include "events.h"
#include "handlers.h"
#include "../sadptrvector.h"
#include "handlerconditions.h"

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
	/*! Controls takes ownership for all of conditions and handler. 
		It's frees memory from them, when destroyed
	 */ 
	~Controls();
	/*! Adds new handler and conditions for it invocation
		\param[in] tac defines both type for events, received by handler
		\param[in] h  handler, which will receive events
	 */
	void add(const sad::input::HandlerTypeAndConditions & tac, sad::input::AbstractHandler * h);
	/*! Posts event to controls, running callbacks, determined by event type,  with argument of event e
		\param[in] type type of event, which is run
		\param[in] e event which is being emited
	 */
	void postEvent(EventType type, const sad::input::AbstractEvent & e);
protected:
	/*! A both handle and conditions, stored in controls
	 */
	typedef sad::Pair<sad::input::HandlerConditionsList, sad::input::AbstractHandler *> HandlerAndConditions;
	/*! A shortcut for list of handlers
	 */
	typedef sad::Vector<HandlerAndConditions>  HandlerList;
	/*! A handlers, attached to a event types
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
	/*! Frees memory, from both handler and conditions, setting pair values to zero
		\param[in] o handler and conditions
	 */
	static void freeHandlerAndConditions(HandlerAndConditions & o);
	/*! Invokes a conditions and tries to invoke a handlers, if they are met
		\param[in] o handler and conditions
		\param[in] e event
	 */
	static void tryInvokeHandler(const HandlerAndConditions & o, const sad::input::AbstractEvent & e);
};

}

}
