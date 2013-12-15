/*! \file   pipeline.h
	\author HiddenSeeker

	Declares a main pipeline item for declaring what should be done in main loop of renderer
 */
#pragma once
#include "pipelinestep.h"
#include "pipelineprocess.h"
#include "pipelinetask.h"

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
	/*! Inserts a step to a position, specified by arguments. Note, that you can use only begin and
		end here.
		\param[in] type an insertion type
		\param[in] step inserted step
		\return reference to a step
	 */
	sad::pipeline::Step * insertStep(
		sad::pipeline::PipelineInsertionType type, 
		sad::pipeline::Step * step
	);
	/*! Inserts a step to a position, specified by arguments. Note, that you can use here any
		insertion type
		\param[in] type an insertion type
		\param[in] mark a mark, where insertion goes
		\param[in] step inserted step
		\return reference to a step
	 */
	sad::pipeline::Step *  insertStep(
		sad::pipeline::PipelineInsertionType type, 
		sad::String mark,
		sad::pipeline::Step * step
	);
	/*! A system method to prepend scene rendering with specified method call. Note, that this
		methods are reserved for sad::Renderer only
		\param[in] o object
		\param[in] f method to be called
		\param[in] mark mark for a step if not empty
		\return reference to a step
	 */
	template<typename _Object, typename  _Method>
	sad::pipeline::Step * systemPrependSceneRenderingWithProcess(
		_Object * o, 
		_Method f, 
		const sad::String & mark = ""
	)
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f);
		step->setSource(sad::pipeline::ST_SYSTEM);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION, step);
	}
	/*! A system method to prepend scene rendering with specified chained method call. Note, that this
		methods are reserved for sad::Renderer only
		\param[in] o object
		\param[in] f method to be called
		\param[in] g second method
		\param[in] mark mark for a step if not empty
		\return reference to a step
	 */
	template<typename _Object, typename  _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * systemPrependSceneRenderingWithProcess(
		_Object * o, 
		_FirstMethod f, 
		_SecondMethod g,
		const sad::String & mark = ""
	)
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f, g);
		step->setSource(sad::pipeline::ST_SYSTEM);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION, step);
	}
	/*! A system method to append process to a container. Note, that this method is reserved for
		renderer only
		\param[in] o object
		\param[in] f method to be called
		\param[in] mark mark for a step if not empty
		\return reference to a step
	 */
	template<typename _Object, typename  _Method>
	sad::pipeline::Step * systemAppendProcess(
		_Object * o, 
		_Method f, 
		const sad::String & mark = ""
	)
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f);
		step->setSource(sad::pipeline::ST_SYSTEM);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! A system method to append process as a chained method call to container. 
	Note, that this method is reserved for
		renderer only
		\param[in] o object
		\param[in] f method to be called
		\param[in] g second method to be called
		\param[in] mark mark for a step if not empty
		\return reference to a step
	 */
	template<typename _Object, typename  _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * systemAppendProcess(
		_Object * o, 
		_FirstMethod f, 
		_SecondMethod g,
		const sad::String & mark = ""
	)
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f, g);
		step->setSource(sad::pipeline::ST_SYSTEM);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a function call as process into beginning of user actions, 
		before any user action new process with specified
		mark if not found
		\param[in] f function
		\param[in] mark a mark, with which process will be created
		\return created step
	 */
	template<typename _Callable>
	sad::pipeline::Step * prependProcess(_Callable f) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a method call as process into beginning of user actions,
		before any user action new process with specified
		mark if not found
		\param[in] o object
		\param[in] f method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _Method>
	sad::pipeline::Step * prependProcess(_Object * o, _Method f) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a chained method call as process into beginning of user actions,
		before any user action new process with specified
		mark if not found
		\param[in] o object
		\param[in] f first method
		\param[in] g second method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * prependProcess(
		_Object * o,
		_FirstMethod f, 
		_SecondMethod g
	) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f, g);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a function call as task into beginning of user actions, 
		before any user action new process with specified
		mark if not found
		\param[in] f function
		\param[in] mark a mark, with which process will be created
		\return created step
	 */
	template<typename _Callable>
	sad::pipeline::Step * prependTask(_Callable f) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a method call as task into beginning of user actions,
		before any user action new process with specified
		mark if not found
		\param[in] o object
		\param[in] f method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _Method>
	sad::pipeline::Step * prependTask(_Object * o, _Method f) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(o, f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a chained method call as task into beginning of user actions,
		before any user action new process with specified
		mark if not found
		\param[in] o object
		\param[in] f first method
		\param[in] g second method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * prependTask(
		_Object * o,
		_FirstMethod f, 
		_SecondMethod g
	) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(o, f, g);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_BEGIN, step);
	}
	/*! Inserts a function call as process into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] f function
		\param[in] mark a mark, with which process will be created
		\return created step
	 */
	template<typename _Callable>
	sad::pipeline::Step * appendProcess(_Callable f) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a method call as process into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] o object
		\param[in] f method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _Method>
	sad::pipeline::Step * appendProcess(_Object * o, _Method f) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a chained method call as process into into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] o object
		\param[in] f first method
		\param[in] g second method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * appendProcess(
		_Object * o,
		_FirstMethod f, 
		_SecondMethod g
	) 
	{
		sad::pipeline::Process * step = new sad::pipeline::Process(o, f, g);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a function call as task into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] f function
		\param[in] mark a mark, with which process will be created
		\return created step
	 */
	template<typename _Callable>
	sad::pipeline::Step * appendTask(_Callable f) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a method call as task into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] o object
		\param[in] f method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _Method>
	sad::pipeline::Step * appendTask(_Object * o, _Method f) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(o, f);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts a chained method call as task into into end of user actions, 
		after all of user actions is performed
		mark if not found
		\param[in] o object
		\param[in] f first method
		\param[in] g second method
		\param[in] mark a mark, with which process will be created
	 */
	template<typename _Object, typename _FirstMethod, typename _SecondMethod>
	sad::pipeline::Step * appendTask(
		_Object * o,
		_FirstMethod f, 
		_SecondMethod g
	) 
	{
		sad::pipeline::Task * step = new sad::pipeline::Task(o, f, g);
		step->setSource(sad::pipeline::ST_USER);
		return insertStep(sad::pipeline::PIT_END, step);
	}
	/*! Inserts step before specified step. If step with mark is not found, step will not be inserted
		\param[in] before a mark, which marks step, which our step will be inserted before
		\param[in] step a step to be inserted
		\param[in] mark a mark for our step
	 */
	inline
	sad::pipeline::Step * insertBefore(const sad::String & before, sad::pipeline::Step * step, const sad::String & mark = "")
	{
		step->setSource(sad::pipeline::ST_USER);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_BEFORE, before, step);
	}

	/*! Inserts step after specified step. If step with mark is not found, step will not be inserted
		\param[in] after a mark, which marks step, which our step will be inserted after
		\param[in] step a step to be inserted
		\param[in] mark a mark for our step
	 */
	inline
	sad::pipeline::Step * insertAfter(const sad::String & after, sad::pipeline::Step * step, const sad::String & mark = "")
	{
		step->setSource(sad::pipeline::ST_USER);
		if (mark.size())
		{
			step->mark(mark);
		}
		return insertStep(sad::pipeline::PIT_AFTER, after, step);
	}
	/*
	/*! Runs a pipeline loop
	 */
	void run();

	/*! Reimplemented. If user inserts step in runtime into end of scene rendering, we should add
		it immediately, because it won't hurt performance and allows to perform transition at end
		of frame and to not render next frame.
		\param[in] o inserted data
	 */
	virtual void add(const sad::pipeline::PipelineInsertionData & o);
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
		\param[in] step a step to be found
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

