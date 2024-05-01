#include "pipeline/pipelinestep.h"
#include "opticksupport.h"

sad::pipeline::Step::~Step()
{
    PROFILER_EVENT;
    
}

void sad::pipeline::Step::mark(const sad::String & mark)
{
    PROFILER_EVENT;
    m_mark.setValue(mark);
}

const sad::Maybe<sad::String> sad::pipeline::Step::mark() const
{
    PROFILER_EVENT;
    return m_mark;
}

sad::pipeline::StepSource sad::pipeline::Step::source() const
{
    PROFILER_EVENT;
    return m_source;
}

void sad::pipeline::Step::setSource(sad::pipeline::StepSource source)
{
    PROFILER_EVENT;
    m_source = source;
}

void sad::pipeline::Step::process()
{
    PROFILER_EVENT;
    if (m_enabled)
    {
        this->_process();
    }
}


void sad::pipeline::Step::enable()
{
    PROFILER_EVENT;
    m_enabled = true;
}

void  sad::pipeline::Step::disable()
{
    PROFILER_EVENT;
    m_enabled = false;
}

bool sad::pipeline::Step::enabled() const
{
    PROFILER_EVENT;
    return m_enabled;
}


void sad::pipeline::Step::changeObject(void * o)
{
    PROFILER_EVENT;

}
