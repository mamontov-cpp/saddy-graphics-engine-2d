#include "marshal/serializablecontainer.h"



SerializableContainer::~SerializableContainer()
{
}

void SerializableContainer::add(SerializableObject * obj)
{
	m_container<<obj;
	obj->setParent(this);
}

void SerializableContainer::remove(SerializableObject * obj)
{
	m_container.remove(obj);
	obj->setParent(NULL);
}

SerializableObject * SerializableContainer::begin()
{
	if (m_container.count()==0)
		return NULL;

	m_iterator = 1;
	return m_container[0];
}


SerializableObject * SerializableContainer::next()
{
	if ((unsigned int)m_iterator>=m_container.count())
		return NULL;
	
	return m_container[m_iterator++];
}


