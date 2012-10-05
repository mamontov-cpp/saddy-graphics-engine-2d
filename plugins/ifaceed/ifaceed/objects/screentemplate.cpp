#include "screentemplate.h"
#include "abstractscreenobject.h"

AbstractScreenObject * ScreenTemplate::templateBegin()
{
	SerializableObject* obj = this->begin();
	if (!obj) return NULL;
	return static_cast<AbstractScreenObject *>(obj);
}

AbstractScreenObject * ScreenTemplate::templateNext()
{
	SerializableObject* obj = this->next();
	if (!obj) return NULL;
	return static_cast<AbstractScreenObject *>(obj);
}

void ScreenTemplate::add(AbstractScreenObject * obj)
{
	this->SerializableContainer::add((SerializableObject*)obj);
	obj->addRef();
}

void ScreenTemplate::remove(AbstractScreenObject * obj)
{
	this->SerializableContainer::remove((SerializableObject*)obj);
	obj->delRef();
}

ScreenTemplate::~ScreenTemplate()
{
	AbstractScreenObject * obj = this->templateBegin();
	while(obj)
	{
		obj->delRef();
		obj = this->templateNext();
	}
}
