#include "pipeline/pipelinestep.h"

sad::pipeline::Step::~Step()
{
    
}

void sad::pipeline::Step::mark(const sad::String & mark)
{
    m_mark.setValue(mark);
}

const sad::Maybe<sad::String> sad::pipeline::Step::mark() const
{
    return m_mark;
}

sad::pipeline::StepSource sad::pipeline::Step::source() const
{
    return m_source;
}

void sad::pipeline::Step::setSource(sad::pipeline::StepSource source)
{
    m_source = source;
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


void sad::pipeline::Step::changeObject(void * o)
{

}
