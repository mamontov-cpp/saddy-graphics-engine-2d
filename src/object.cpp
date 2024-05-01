#include "object.h"
#include "db/dberror.h"
#include "opticksupport.h"

sad::Object::~Object()
= default;

sad::ClassMetaData * sad::Object::globalMetaData() 
{
    PROFILER_EVENT;
    bool created = false;
    return sad::ClassMetaDataContainer::ref()->get("sad::Object", created);
}

sad::ClassMetaData * sad::Object::metaData() const
{
    PROFILER_EVENT;
    return sad::Object::globalMetaData();
}

const sad::String & sad::Object::className() const
{
    PROFILER_EVENT;
    return this->metaData()->name();
}

bool sad::Object::isInstanceOf(const sad::String& name)
{
    PROFILER_EVENT;
    return this->metaData()->canBeCastedTo(name);	
}

const sad::String& sad::Object::serializableName() const //-V524
{
    PROFILER_EVENT;
    return this->metaData()->name();
}

sad::InvalidCastException::InvalidCastException(const sad::String &from_name, const sad::String &to_name)
{
    PROFILER_EVENT;
    m_from_name = from_name;
    m_to_name = to_name;
    m_message = "Can\'t cast type \"";
    m_message << m_from_name;
    m_message << "\" to type\"";
    m_message << m_to_name;
}

const sad::String & sad::InvalidCastException::fromName() const
{
    PROFILER_EVENT;
    return m_from_name;
}


const sad::String & sad::InvalidCastException::toName() const
{
    PROFILER_EVENT;
    return m_to_name;
}

const sad::String & sad::InvalidCastException::message() const
{
    PROFILER_EVENT;
    return m_message;
}
