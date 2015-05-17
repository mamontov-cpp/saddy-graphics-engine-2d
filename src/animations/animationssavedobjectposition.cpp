#include "animations/animationssavedobjectposition.h"

#include "sadrect.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "p2d/body.h"
#include "p2d/collisionshape.h"

sad::animations::SavedObjectPosition::SavedObjectPosition(sad::db::Object* o) 
: sad::animations::SavedObjectState(o),
m_body(NULL)
{
	sad::Rect2D oldarea = o->getProperty<sad::Rect2D>("area").value();
	m_oldcenter = (oldarea[0] + oldarea[2]) / 2.0;
}



sad::animations::SavedObjectPosition::~SavedObjectPosition() 
{
    if (m_body)
    {
        m_body->delRef();
    }
}

void sad::animations::SavedObjectPosition::restore()
{
	sad::Rect2D area =  m_object->getProperty<sad::Rect2D>("area").value();
	sad::Point2D center = (area[0] + area[2]) / 2.0;

	sad::Rect2D newarea = area;
	sad::moveBy(m_oldcenter - center, newarea);
	m_object->setProperty("area", newarea);

	if (m_body)
	{
		m_body->shedulePosition(m_oldposition);
	}
}

void sad::animations::SavedObjectPosition::storeBodyState(sad::p2d::Body* b)
{
    if (m_body)
    {
        m_body->delRef();
    }
	m_body = b;
	m_oldposition = m_body->position();
    m_body->addRef();
}
