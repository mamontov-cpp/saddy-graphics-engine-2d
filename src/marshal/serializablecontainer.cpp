#include "marshal/serializablecontainer.h"



SerializableContainer::~SerializableContainer()
{
}



void SerializableContainer::add(SerializableObject * obj)
{
	obj->setParent(this);
}

void SerializableContainer::remove(SerializableObject * obj)
{
	obj->setParent(NULL);
}
