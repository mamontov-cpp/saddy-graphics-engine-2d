#include "marshal/serializableobject.h"
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

SerializationEntry * SerializableObject::save(ActionContext * context)
{
	SerializationEntry * result = new SerializationEntry();
	result->Name = this->type();
	context->pushAction(sad::String("SerializableObject(")+
						serializable::toString(this) + 
						sad::String(")::save: saving object"));
	for (sad::Hash<sad::String, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
	{
		if (it.value()->saveable())
		{
		 result->PropertiesName<<it.key();
		 result->PropertiesValue<<it.value()->save(context);
		}
	}
	context->popAction();
	return result;
}


void SerializableObject::load(SerializationEntry * entry,ActionContext * context)
{
	assert(entry->Name == this->type());
	context->pushAction(sad::String("SerializableObject(")+
						serializable::toString(this) + 
						sad::String(")::load: loading object"));
	
	for (unsigned int i=0;i<entry->PropertiesName.count();i++)
	{
		AbstractProperty * pop = this->getProperty(entry->PropertiesName[i]);
		pop->load(entry->PropertiesValue[i],context);
	}

	context->popAction();
}

void SerializableObject::resolveDeferred(ActionContext * context)
{
	for (sad::Hash<sad::String, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
	{
	 it.value()->resolveDeferred(context);
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


