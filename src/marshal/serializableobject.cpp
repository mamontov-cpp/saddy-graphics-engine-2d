#include "marshal/serializableobject.h"
#include "log/log.h"
#include "db/dbvariant.h"
#include <assert.h>


DECLARE_SOBJ(SerializableObject);

sad::String SerializableObject::type()
{
	return this->className();
}


void SerializableObject::addProperty(const sad::String & name, sad::db::Property * prop)
{
	assert( !m_properties.contains(name) ); 
	m_properties.insert(name,prop);
}


sad::db::Property * SerializableObject::getProperty(const sad::String & name)
{
	if (m_properties.contains(name))
		return m_properties[name];
	return NULL;
}

SerializationEntry * SerializableObject::saveToEntry()
{
	SL_SCOPE("SerializableObject::save()");

	SerializationEntry * result = new SerializationEntry();
	result->Name = this->type();
	sad::db::Variant v;
	for (sad::Hash<sad::String, sad::db::Property *>::iterator it = m_properties.begin();
														       it != m_properties.end();
															   it++)
	{
		result->PropertiesName << it.key();		
		it.value()->get((sad::db::Object *)this, v);
		result->PropertiesValue << v.save().serialize();
	}

	return result;
}


bool SerializableObject::load(SerializationEntry * entry)
{
	SL_SCOPE("SerializableObject::load()");
	assert(entry->Name == this->type());	
	bool result = true;
	/*
	for (unsigned int i=0;i<entry->PropertiesName.count();i++)
	{
		sad::db::Property * pop = this->getProperty(entry->PropertiesName[i]);
		sad::db::Variant v  = pop->getValue();
		bool ok = v.load(picojson::parse_string(entry->PropertiesValue[i]));
		if (ok)
		{
			pop->setValue(v);
		}
		result = result && ok;
	}
	*/
	return result;
}



SerializableObject::~SerializableObject()
{
	for (sad::Hash<sad::String, sad::db::Property *>::iterator it = m_properties.begin();
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


