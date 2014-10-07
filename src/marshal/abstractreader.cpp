#include "marshal/abstractreader.h"
#include "log/log.h"

bool serializable::AbstractReader::read(SerializableContainer * container)
{
	// Log entering reader
	std::ostringstream str;
	str << container;
	SL_SCOPE(fmt::Format("serializable::AbstractReader::read({0})") << str.str());
	
	if (!(this->openStream()))
	{
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
		 obj->load(entry);
		 container->add(obj);
		}

		delete entry;
		findReadingPoint();
		entry=this->nextEntry();
	}
	

	SerializableObject * current = container->begin();

	this->closeStream();
	return true;
}

serializable::AbstractReader::~AbstractReader()
{
	delete m_factory;
}
