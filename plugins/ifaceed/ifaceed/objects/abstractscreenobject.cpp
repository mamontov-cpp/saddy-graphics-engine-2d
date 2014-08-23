#include "abstractscreenobject.h"
#include "screentemplate.h"

#include <db/dbfield.h>
#include <db/dbmethodpair.h>

#include <db/load.h>
#include <db/save.h>

#include <unused.h>

DECLARE_SOBJ_INHERITANCE(AbstractScreenObject,sad::SceneNode)

AbstractScreenObject::AbstractScreenObject()
{
	m_scene = NULL;

	addProperty("name", new sad::db::Field<AbstractScreenObject, sad::String>(&AbstractScreenObject::m_name));
	addProperty("layer", new sad::db::MethodPair<AbstractScreenObject, unsigned int>(&AbstractScreenObject::cachedLayer, &AbstractScreenObject::setCachedLayer) );
	addProperty("uid", new sad::db::MethodPair<AbstractScreenObject, sad::String>(&AbstractScreenObject::uid, &AbstractScreenObject::setUID) );
	addProperty("activity", new sad::db::MethodPair<AbstractScreenObject, bool>(&AbstractScreenObject::active, &AbstractScreenObject::setActive) );
	addProperty("visibility", new sad::db::MethodPair<AbstractScreenObject, bool>(&AbstractScreenObject::visible, &AbstractScreenObject::setVisible) );
}

AbstractScreenObject::~AbstractScreenObject()
{
}

const sad::String & AbstractScreenObject::uid() const
{
	if (this->screenTemplate()) 
	{
		AbstractScreenObject * me = const_cast<AbstractScreenObject*>(this);
		me->m_uid = this->screenTemplate()->uid(me);	 
	}
	return m_uid;
}

void AbstractScreenObject::setUID(const sad::String & uid)
{
	this->m_uid = uid;
	if (this->screenTemplate())
	{
		this->screenTemplate()->setUid(this, m_uid);
	}
}

void AbstractScreenObject::render()
{
	_render();
}

ScreenTemplate * AbstractScreenObject::screenTemplate() const
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
	return this->SerializableObject::getProperty(s) != NULL;
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
