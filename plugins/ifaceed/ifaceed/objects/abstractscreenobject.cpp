#include "abstractscreenobject.h"
#include "screentemplate.h"

#include <db/dbfield.h>
#include <db/dbmethodpair.h>

#include <unused.h>

DECLARE_SOBJ_INHERITANCE2(AbstractScreenObject,sad::SceneNode, SerializableObject)

AbstractScreenObject::AbstractScreenObject()
{
	m_scene = NULL;

	addProperty("name", new sad::db::Field<AbstractScreenObject, sad::String>(&AbstractScreenObject::m_name));
	addProperty("layer", new sad::db::MethodPair<AbstractScreenObject, unsigned int>(&AbstractScreenObject::cachedLayer, &AbstractScreenObject::setCachedLayer) );
	//addProperty("uid", new UidProperty());
	addProperty("activity", new sad::db::MethodPair<AbstractScreenObject, bool>(&AbstractScreenObject::active, &AbstractScreenObject::setActive) );
	addProperty("visibility", new sad::db::MethodPair<AbstractScreenObject, bool>(&AbstractScreenObject::visible, &AbstractScreenObject::setVisible) );
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

bool AbstractScreenObject::hasProperty(const sad::String & s)
{
	return this->getProperty(s) != NULL;
}




sad::String AbstractScreenObject::type()
{
	return this->typeName();
}

sad::String AbstractScreenObject::description()
{
	if (m_name.length())
		return m_name;
	return _description();
}

void AbstractScreenObject::setRotatedRectangle(UNUSED const sad::Rect2D & rotatedrectangle, UNUSED float angle)
{

}
