#include "hfsm/hfsmshared.h"

DECLARE_SOBJ(sad::hfsm::Shared);

sad::hfsm::Shared::Shared()
{

}

sad::hfsm::Shared::~Shared()
{

}

void sad::hfsm::Shared::setVariable(const sad::String& name, const sad::db::Variant& v)
{
    m_variables.insert(name, v);
}

sad::Maybe<sad::db::Variant> sad::hfsm::Shared::getVariable(const sad::String& name)
{
    if (m_variables.contains(name))
    {
        return sad::Maybe<sad::db::Variant>(m_variables[name]);
    }
    return sad::Maybe<sad::db::Variant>();
}
