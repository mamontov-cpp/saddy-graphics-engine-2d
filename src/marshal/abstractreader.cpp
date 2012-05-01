#include "marshal/abstractreader.h"


bool serializable::AbstractReader::read(SerializableContainer * container, ActionContext * context)
{
	context->pushAction(hst::string("serializable::AbstractReader::read: reading data"));
	if (!(this->openStream()))
	{
		context->popAction();
		return false;
	}
	findReadingPoint();
	SerializationEntry * entry = this->nextEntry();
	while(entry)
	{
		SerializableObject * obj = m_factory->produce(entry->Name);
		//If object was created successfully load it
		if (obj)
		{
		 obj->load(entry,context);
		 container->add(obj);
		}

		delete entry;
		findReadingPoint();
		entry=this->nextEntry();
	}
	
	this->closeStream();
	context->popAction();
	return true;
}

serializable::AbstractReader::~AbstractReader()
{
	delete m_factory;
}