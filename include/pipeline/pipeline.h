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
	typedef sad::Pair<StepsList *, size_t>  Position;
	/*! Invokes specified steps
		\param[in] steps steps to be invoked
	 */
	void invokeSteps(StepsList & steps);
	/*! Performs immediate insertion to pipeline
	    \param[in] o an insertion command data
	 */
	virtual void addNow(PipelineInsertionData o) = 0;
	/*! Immediately removed a step from container
	    \param[in] o immediately removes a pipeline data
	 */
	virtual void removeNow(sad::pipeline::Step * o) = 0;
	/*! Immediately removes all data from pipeline
	 */
	virtual void clearNow() = 0;
	/*! All system steps of pipeline, executed before user steps 
	 */
	sad::Vector<sad::pipeline::Step *> m_system_steps_before_user;
	/*! A user steps, for a pipeline
	 */
	sad::Vector<sad::pipeline::Step *> m_user_steps;
	/*! All system steps of pipeline, executed before user steps 
	 */
	sad::Vector<sad::pipeline::Step *> m_system_steps_after_user;

};

}

}

