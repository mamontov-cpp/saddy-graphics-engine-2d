#include "object.h"
#include "db/dberror.h"

sad::Object::~Object()
{

}

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

sad::InvalidCastException::InvalidCastException(const sad::String &fromname, const sad::String &toname)
{
    m_fromname = fromname;
    m_toname = toname;
    m_message = "Can\'t cast type \"";
    m_message << m_fromname;
    m_message << "\" to type\"";
    m_message << m_toname;
}

const sad::String & sad::InvalidCastException::fromName()
{
    return m_fromname;
}


const sad::String & sad::InvalidCastException::toName()
{
    return m_toname;
}

const sad::String & sad::InvalidCastException::message()
{
    return m_message;
}
