/*! \file pipelineprocess.h
	\author HiddenSeeker

	Describes a process, which should be performed every time inside of pipeline loop
 */
#pragma once
#include "pipelinestep.h"
#include "pipelinedelegate.h"

namespace sad
{

namespace pipeline
{

/*! Defines a basic pipeline process, which should be performed every time
	insided of pipeline loop
 */
class AbstractProcess: public sad::pipeline::Step
{
public:
	inline AbstractProcess() : sad::pipeline::Step()
	{
	}
	/*! An abstract process should never be destroyed after processing
		\return false
	 */
	virtual bool shouldBeDestroyedAfterProcessing();
};

}

}