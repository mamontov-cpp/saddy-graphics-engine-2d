#include "marshal/serializableobject.h"
#include <assert.h>

void SerializableObject::addProperty(const hst::string & name, AbstractProperty * prop)
{
	assert( !m_properties.contains(name) ); 
	prop->setParent(this);
	m_properties.insert(name,prop);
}


AbstractProperty * SerializableObject::getProperty(const hst::string & name)
{
	if (m_properties.contains(name))
		return m_properties[name];
	return NULL;
}

SerializationEntry * SerializableObject::save(ActionContext * context)
{
	SerializationEntry * result = new SerializationEntry();
	result->Name = this->type();
	context->pushAction(hst::string("SerializableObject(")+
						serializable::toString(this) + 
						hst::string(")::save: saving object"));
	for (hst::hash<hst::string, AbstractProperty *>::iterator it = m_properties.begin();
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
	context->pushAction(hst::string("SerializableObject(")+
						serializable::toString(this) + 
						hst::string(")::load: loading object"));
	
	for (unsigned int i=0;i<entry->PropertiesName.count();i++)
	{
		AbstractProperty * pop = this->getProperty(entry->PropertiesName[i]);
		pop->load(entry->PropertiesValue[i],context);
	}

	context->popAction();
}

void SerializableObject::resolveDeferred(ActionContext * context)
{
	for (hst::hash<hst::string, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
	{
	 it.value()->resolveDeferred(context);
	}
}

SerializableObject::~SerializableObject()
{
 for (hst::hash<hst::string, AbstractProperty *>::iterator it = m_properties.begin();
														   it != m_properties.end();it++)
 {
	 delete it.value();
 }
}

SerializableObject* SerializableFactory::produce(const hst::string & obj) 
{
	return NULL;
}

SerializableFactory::~SerializableFactory()
{
}


