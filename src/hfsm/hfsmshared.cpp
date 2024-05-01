#include "hfsm/hfsmshared.h"
#include "opticksupport.h"

DECLARE_SOBJ(sad::hfsm::Shared);

sad::hfsm::Shared::Shared()
{
    PROFILER_EVENT;

}

sad::hfsm::Shared::~Shared()
{
    PROFILER_EVENT;

}

void sad::hfsm::Shared::setVariable(const sad::String& name, const sad::db::Variant& v)
{
    PROFILER_EVENT;
    m_variables.insert(name, v);
}

sad::Maybe<sad::db::Variant> sad::hfsm::Shared::getVariable(const sad::String& name)
{
    PROFILER_EVENT;
    if (m_variables.contains(name))
    {
        return sad::Maybe<sad::db::Variant>(m_variables[name]);
    }
    return sad::Maybe<sad::db::Variant>();
}
