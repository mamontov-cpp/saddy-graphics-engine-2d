#include "marshal/serializableobject.h"
#include "log/log.h"
#include <assert.h>


DECLARE_SOBJ(SerializableObject);

sad::String SerializableObject::type()
{
	return this->name();
}


void SerializableObject::addProperty(const sad::String & name, AbstractProperty * prop)
{
	assert( !m_properties.contains(name) ); 
	prop->setParent(this);
	m_properties.insert(name,prop);
}


AbstractProperty * SerializableObject::getProperty(const sad::String & name)
{
	if (m_properties.contains(name))
		return m_properties[name];
	return NULL;
}

SerializationEntry * SerializableObject::save()
{
	SL_SCOPE("SerializableObject::save()");

	SerializationEntry * result = new SerializationEntry();
	result->Name = this->type();
	for (sad::Hash<sad::String, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
	{
		if (it.value()->saveable())
		{
		 result->PropertiesName<<it.key();
		 result->PropertiesValue<<it.value()->save();
		}
	}

	return result;
}


void SerializableObject::load(SerializationEntry * entry)
{
	SL_SCOPE("SerializableObject::load()");
	assert(entry->Name == this->type());	
	for (unsigned int i=0;i<entry->PropertiesName.count();i++)
	{
		AbstractProperty * pop = this->getProperty(entry->PropertiesName[i]);
		pop->load(entry->PropertiesValue[i]);
	}
}

void SerializableObject::resolveDeferred()
{
	SL_SCOPE("SerializableObject::resolveDeferred()");
	for (sad::Hash<sad::String, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
	{
	 it.value()->resolveDeferred();
	}
}

SerializableObject::~SerializableObject()
{
 for (sad::Hash<sad::String, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
 {
	 delete it.value();
 }
}

SerializableObject* SerializableFactory::produce(const sad::String & obj) 
{
	return NULL;
}

SerializableFactory::~SerializableFactory()
{
}


