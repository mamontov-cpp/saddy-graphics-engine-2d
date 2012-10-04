#include "abstractobject.h"
#include "screentemplate.h"

SAD_DECLARE(AbstractObject,sad::BasicNode)


AbstractObject::~AbstractObject()
{
}


void AbstractObject::render()
{
	if (m_active && m_visible)
		_render();
}

ScreenTemplate * AbstractObject::screenTemplate()
{
	SerializableContainer * parent = this->parent();
	if (!parent)
		return NULL;
	return static_cast<ScreenTemplate *>(parent);
}

