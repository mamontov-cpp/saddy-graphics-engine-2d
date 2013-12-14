#include "pipeline/pipeline.h"

sad::pipeline::Pipeline::Pipeline()
{

}

void sad::pipeline::Pipeline::run()
{
	this->performQueuedActions();
	this->lockChanges();

	invokeSteps(m_system_steps_before_user);
	invokeSteps(m_user_steps);
	invokeSteps(m_system_steps_after_user);

	this->unlockChanges();
	this->performQueuedActions();
}

sad::pipeline::Pipeline::~Pipeline()
{
	this->clearNow();
}


void sad::pipeline::Pipeline::invokeSteps(sad::pipeline::Pipeline::StepsList & steps)
{
	for(unsigned int i = 0; i < steps.size(); i++)
	{
		steps[i]->process();
		if (steps[i]->shouldBeDestroyedAfterProcessing()) 
		{
			delete steps[i];
			steps.removeAt(i);
			--i;
		}
	}
}

void sad::pipeline::Pipeline::addNow(PipelineInsertionData o)
{
	switch(o.p1())
	{
		case sad::pipeline::PIT_BEGIN: 
		{
			if (o.p3()->source() == sad::pipeline::ST_SYSTEM)
				m_system_steps_before_user.insert(o.p3(), 0);
			else
				m_user_steps.insert(o.p3(), 0);
			break;
		}
		case sad::pipeline::PIT_END:
		{
			if (o.p3()->source() == sad::pipeline::ST_SYSTEM)
				m_system_steps_after_user << o.p3();
			else
				m_user_steps << o.p3();
			break;
		}
		case sad::pipeline::PIT_SYSTEM_BEFORE_FIRST_USER_ACTION:
		{
			m_system_steps_before_user << o.p3();
			break;
		}
		case sad::pipeline::PIT_SYSTEM_AFTER_LAST_USER_ACTION:
		{
			m_system_steps_after_user << o.p3();
			break;
		}
		case sad::pipeline::PIT_BEFORE:
		{
			sad::Pair<>
		}
	};
}