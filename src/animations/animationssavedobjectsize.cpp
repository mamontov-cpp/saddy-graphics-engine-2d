#include "animations/animationssavedobjectsize.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "p2d/body.h"
#include "p2d/collisionshape.h"

sad::animations::SavedObjectSize::SavedObjectSize(sad::db::Object* o) 
: sad::animations::SavedObjectState(o),
m_body(nullptr), 
m_shape(nullptr)
{
    m_old_area = o->getProperty<sad::Rect2D>("area").value();
    m_old_center = (m_old_area[0] + m_old_area[2]) / 2.0;
}



sad::animations::SavedObjectSize::~SavedObjectSize() 
{
    delete m_shape;
    if (m_body)
    {
        m_body->delRef();
    }
}

void sad::animations::SavedObjectSize::restore()
{
    sad::Rect2D area =  m_object->getProperty<sad::Rect2D>("area").value();
    const sad::Point2D center = (area[0] + area[2]) / 2.0;
    sad::Rect2D new_area = m_old_area;
    sad::moveBy(center - m_old_center, new_area);
    m_object->setProperty("area", new_area);
    if (m_body)
    {
        m_body->setShape(m_shape->clone());
    }
}

void sad::animations::SavedObjectSize::storeBodyState(sad::p2d::Body* b)
{
    if (m_body)
    {
        m_body->delRef();
    }
    delete m_shape;
    m_body = b;
    m_body->addRef();
    m_shape = m_body->currentShape()->clone();
    m_shape->move(sad::Point2D(0,0) - m_shape->center());
}
