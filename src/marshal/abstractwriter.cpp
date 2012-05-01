#include "marshal/abstractwriter.h"

serializable::AbstractWriter::~AbstractWriter()
{
}

bool serializable::AbstractWriter::write(SerializableContainer * container, ActionContext * context)
{
	context->pushAction(hst::string("serializable::AbstractWriter::write: writing data"));
	if (!(this->openStream()))
	{
		context->popAction();
		return false;
	}
	
	SerializableObject * current=container->begin();
	while(current)
	{
		SerializationEntry * entry = current->save(context);
		this->write(entry);
		delete entry;
		current = container->next();
	}

	this->closeStream();
	context->popAction();
	return true;
}

