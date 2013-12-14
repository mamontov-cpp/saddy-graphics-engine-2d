/*! \file pipelinestep.h
	\author HiddenSeeker

	Describes a base class, for step of pipeline, which should define a process, performed in
	main pipeline, a task which should be performed once and be removed from a pipeline.

	Also pipeline step could be marked, which gives user a power to insert steps at end or add
	them after specified item.
 */
#pragma once
#include "../maybe.h"
#include "../sadstring.h"

namespace sad
{

namespace pipeline
{

/*! A base class, for step of pipeline, which should define a process, performed in
	main pipeline, a task which should be performed once and be removed from a pipeline.

	Also pipeline step could be marked, which gives user a power to insert steps at end or add
	them after specified item.
 */
class Step
{
public:
	/*! Default step has no mark, and also it cannot be located
	 */
	inline Step() : m_enabled(true)
	{
	}
	/*! Marks step with specified mark
		\param[in] mark specified mark
	 */
	void mark(const sad::String & mark);
	/*! Returns mark for step, if any presented
		\return mark for a step
	 */
	const sad::Maybe<sad::String> mark() const;
	/*! Processes step, invoking a user actions
	 */
	virtual void process();
	/*! Enables a pipeline step
	 */
	virtual void enable();
	/*! Disables a pipeline step
	 */
	virtual void disable();
	/*! Determines, whether after processing we should remove step from pipeline
		\return whether we should remove step from pipeline
	 */
	virtual bool shouldBeDestroyedAfterProcessing() = 0;
protected:
	/*! Reimplement this function to create your own step actions
	 */ 
	virtual void _process() = 0;
	/*! Whether we should enable or disable step invocation
	 */
	bool m_enabled;
	/*! A mark, uniquely identifiying a tep
	 */
	sad::Maybe<sad::String> m_mark;
};

}

}