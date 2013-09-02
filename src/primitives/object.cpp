#include "object.h"

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

const hst::string & sad::Object::name() const
{
	return this->metaData()->name();
}


sad::InvalidCastException::InvalidCastException(const hst::string &fromname, const hst::string &toname)
{
	m_fromname = fromname;
	m_toname = toname;
	m_message = "Can\'t cast type \"";
	m_message << m_fromname;
	m_message << "\" to type\"";
	m_message << m_toname;
}

const hst::string & sad::InvalidCastException::fromName()
{
	return m_fromname;
}


const hst::string & sad::InvalidCastException::toName()
{
	return m_toname;
}

const hst::string & sad::InvalidCastException::message()
{
	return m_message;
}
