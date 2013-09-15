#include "abstractscreenobject.h"
#include "screentemplate.h"
#include "scenelayerproperty.h"
#include "activityproperty.h"
#include "visibilityproperty.h"
#include "uidproperty.h"
#include <unused.h>

DECLARE_SOBJ_INHERITANCE2(AbstractScreenObject,sad::BasicNode, SerializableObject)

AbstractScreenObject::AbstractScreenObject()
{
	m_scene = NULL;

	addProperty("name", new MappedField<hst::string>(&m_name, ""));
	addProperty("layer", new SceneLayerProperty());
	addProperty("uid", new UidProperty());
	addProperty("activity", new ActivityProperty(&m_active,true));
	addProperty("visibility", new VisibilityProperty(&m_visible,true));
}

AbstractScreenObject::~AbstractScreenObject()
{
}


void AbstractScreenObject::render()
{
	if (m_active && m_visible)
		_render();
}

ScreenTemplate * AbstractScreenObject::screenTemplate()
{
	SerializableContainer * parent = this->parent();
	if (!parent)
		return NULL;
	return static_cast<ScreenTemplate *>(parent);
}


bool AbstractScreenObject::rotatable()
{
	return true;
}

bool AbstractScreenObject::resizable()
{
	return true;
}

bool AbstractScreenObject::hasProperty(const hst::string & s)
{
	return this->getProperty(s) != NULL;
}


void AbstractScreenObject::setTop(UNUSED float x)
{

}

void AbstractScreenObject::setBottom(UNUSED  float x)
{

}

void AbstractScreenObject::setLeft(UNUSED  float x)
{

}

void AbstractScreenObject::setRight(UNUSED  float x)
{

}

hst::string AbstractScreenObject::type()
{
	return this->typeName();
}

hst::string AbstractScreenObject::description()
{
	if (m_name.length())
		return m_name;
	return _description();
}

void AbstractScreenObject::setRotatedRectangle(UNUSED const hRectF & rotatedrectangle, UNUSED float angle)
{

}
