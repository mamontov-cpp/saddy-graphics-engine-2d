#include "pipeline/pipelinestep.h"

void sad::pipeline::Step::mark(const sad::String & mark)
{
	m_mark.setValue(mark);
}

const sad::Maybe<sad::String> sad::pipeline::Step::mark() const
{
	return m_mark;
}

void sad::pipeline::Step::process()
{
	if (m_enabled)
	{
		this->_process();
	}
}


void sad::pipeline::Step::enable()
{
	m_enabled = true;
}

void  sad::pipeline::Step::disable()
{
	m_enabled = false;
}
