#include "refcountable.h"
#include "opticksupport.h"

sad::RefCountable::RefCountable() : m_references(0)
{
    PROFILER_EVENT;
    
}

void sad::RefCountable::addRef()  
{
    PROFILER_EVENT;
    ++m_references;
}

void sad::RefCountable::delRef()  
{
    PROFILER_EVENT;
    --m_references; 
    if (m_references == 0) 
        delete this; 
}

void sad::RefCountable::delRefInstance(sad::RefCountable* rc)
{
    PROFILER_EVENT;
    if (rc)
    {
        rc->delRef();
    }
}

int sad::RefCountable::refsCount() const
{
    PROFILER_EVENT;
    return m_references;
}

sad::RefCountable::~RefCountable() = default;
