#include "abstractscreenobject.h"
#include "screentemplate.h"
#include "scenelayerproperty.h"
#include "activityproperty.h"
#include "visibilityproperty.h"
#include "uidproperty.h"

SAD_DECLARE(AbstractScreenObject,sad::BasicNode)

AbstractScreenObject::AbstractScreenObject()
{
	m_scene = NULL;

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
