#include "marshal/abstractwriter.h"
#include "log/log.h"

serializable::AbstractWriter::~AbstractWriter()
{
}

bool serializable::AbstractWriter::write(SerializableContainer * container)
{
	// Log entering reader
	std::ostringstream str;
	str << container;
	SL_SCOPE(fmt::Format("serializable::AbstractWriter::write({0})") << str.str());
	
	if (!(this->openStream()))
	{
		return false;
	}
	
	SerializableObject * current=container->begin();
	while(current)
	{
		SerializationEntry * entry = current->saveToEntry();
		this->write(entry);
		delete entry;
		current = container->next();
	}

	this->closeStream();
	return true;
}

