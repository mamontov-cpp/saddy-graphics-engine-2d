#include "object.h"
#include "db/dberror.h"

sad::Object::~Object()
= default;

sad::ClassMetaData * sad::Object::globalMetaData() 
{
    bool created = false;
    return sad::ClassMetaDataContainer::ref()->get("sad::Object", created);
}

sad::ClassMetaData * sad::Object::metaData() const
{
    return sad::Object::globalMetaData();
}

const sad::String & sad::Object::className() const
{
    return this->metaData()->name();
}

bool sad::Object::isInstanceOf(const sad::String& name)
{
    return this->metaData()->canBeCastedTo(name);	
}

const sad::String& sad::Object::serializableName() const //-V524
{
    return this->metaData()->name();
}

sad::InvalidCastException::InvalidCastException(const sad::String &from_name, const sad::String &to_name)
{
    m_from_name = from_name;
    m_to_name = to_name;
    m_message = "Can\'t cast type \"";
    m_message << m_from_name;
    m_message << "\" to type\"";
    m_message << m_to_name;
}

const sad::String & sad::InvalidCastException::fromName() const
{
    return m_from_name;
}


const sad::String & sad::InvalidCastException::toName() const
{
    return m_to_name;
}

const sad::String & sad::InvalidCastException::message() const
{
    return m_message;
}
