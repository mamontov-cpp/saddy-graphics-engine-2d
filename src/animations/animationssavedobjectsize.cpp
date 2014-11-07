#include "animations/animationssavedobjectsize.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "p2d/body.h"
#include "p2d/collisionshape.h"

sad::animations::SavedObjectSize::SavedObjectSize(sad::db::Object* o) 
: sad::animations::SavedObjectState(o),
m_body(NULL), 
m_shape(NULL)
{
	m_oldarea = o->getProperty<sad::Rect2D>("area").value();
}



sad::animations::SavedObjectSize::~SavedObjectSize() 
{
	delete m_shape;
}

void sad::animations::SavedObjectSize::restore()
{
	m_object->setProperty("area", m_oldarea);
	if (m_body)
	{
		m_body->setShape(m_shape->clone());
	}
}

void sad::animations::SavedObjectSize::storeBodyState(sad::p2d::Body* b)
{
	delete m_shape;
	m_body = b;
	m_shape = m_body->currentShape()->clone();
	m_shape->move(sad::Point2D(0,0) - m_shape->center());
}
