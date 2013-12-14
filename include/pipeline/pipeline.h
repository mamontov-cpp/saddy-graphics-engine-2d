/*! \file   pipeline.h
	\author HiddenSeeker

	Declares a main pipeline item for declaring what should be done in main loop of renderer
 */
#pragma once
#include "pipelinestep.h"
#include "../temporarilyimmutablecontainer.h"
#include "../sadpair.h"
#include "../sadvector.h"

namespace sad
{

namespace pipeline
{

/*! Declares type of insertion into pipeline
 */
enum PipelineInsertionType
{
	PIT_AFTER,                          //!< Tells to insert after specified event mark
	PIT_SYSTEM_AFTER_LAST_USER_ACTION,  //!< Tells to insert into system actions after last user action
	PIT_BEFORE,                         //!< Tells to insert before specified event mark
	PIT_SYSTEM_BEFORE_FIRST_USER_ACTION,//!< Tells to insert into system actions before first user action
	PIT_BEGIN,                          //!< Tells to insert into beginning of pipeline (user or system, depending on source)
	PIT_END								//!< Tells to insert into ending of pipeline (user or system, depending on source)
};

/*! Declares data, definiting a command fo addding commands into pipeline
 */
typedef sad::Triplet< 
	sad::pipeline::PipelineInsertionType, 
	sad::Maybe<sad::String>, 
	sad::pipeline::Step *
> PipelineInsertionData;

/*! A main pipeline class. Note, that clearing a pipeline clears only user commands, because
	we need system commands to not break system messaging loop
 */
class Pipeline: 
public sad::TemporarilyImmutableContainerWithHeterogeneousCommands<
	sad::pipeline::PipelineInsertionData, 
	sad::pipeline::Step *
>
{
public:
	/*! Creates new empty pipeline
	 */
	Pipeline();
	/*! Runs a pipeline loop
	 */
	void run();
	/*! Destroys all steps of pipeline
	 */
	~Pipeline();
protected:
	/*! Defines a list of steps in pipeline
	 */
	typedef sad::Vector<sad::pipeline::Step *> StepsList;
	/*! Defines a position for insertion into steps list
	 */
	typedef sad::Pair<StepsList *, size_t>  StepListPosition;
	/*! Invokes specified steps
		\param[in] steps steps to be invoked
	 */
	void invokeSteps(StepsList & steps);
	/*! Finds a position in step list by mark. If not found first item in position will be
		NULL.
		\param[in] steps a steps container
		\param[in] mark a string mark, which will be found in step
		\return found position. If not found, first item of position, will be null
	 */
	static StepListPosition findByMark(StepsList* steps, const sad::String & mark);
	/*! Finds a position in step list by step. If not found first item in position will be
		NULL.
		\param[in] steps a steps container
		\param[in] step to be found
		\return found position. If not found, first item of position, will be null
	 */
	static StepListPosition findByStep(StepsList* steps, sad::pipeline::Step* step);
	/*! Finds a position of item with specified mark. If not found first item in position will be
		NULL.
		\param[in] mark a string mark, which will be found in step
		\return found position. If not found, first item of position, will be null
	 */
	StepListPosition findByMark(const sad::String & mark);
	/*! Finds a position of specified step
		\param[in] mark a string mark, which will be found
		\param[in] step to be found
		\return found position. If not found, first item of position, will be null
	 */
	StepListPosition findByStep(sad::pipeline::Step* step);
	/*! Erases all of steps, removing all of items from it and making it empty
		\param[in] steps a steps list
	 */
	static void clearSteps(StepsList* steps);
	/*! Performs immediate insertion to pipeline
	    \param[in] o an insertion command data
	 */
	virtual void addNow(PipelineInsertionData o);
	/*! Immediately removed a step from container
	    \param[in] o immediately removes a pipeline data
	 */
	virtual void removeNow(sad::pipeline::Step * o);
	/*! Immediately removes all data from pipeline
	 */
	virtual void clearNow();
	/*! All system steps of pipeline, executed before user steps 
	 */
	StepsList m_system_steps_before_user;
	/*! A user steps, for a pipeline
	 */
	StepsList m_user_steps;
	/*! All system steps of pipeline, executed before user steps 
	 */
	StepsList m_system_steps_after_user;

};

}

}

