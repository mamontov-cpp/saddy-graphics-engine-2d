#include "animations/animationssavedobjectposition.h"

#include "sadrect.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "p2d/body.h"
#include "p2d/collisionshape.h"
#include "opticksupport.h"

sad::animations::SavedObjectPosition::SavedObjectPosition(sad::db::Object* o) 
: sad::animations::SavedObjectState(o),
m_body(nullptr)
{
    PROFILER_EVENT;
    sad::Rect2D old_area = o->getProperty<sad::Rect2D>("area").value();
    m_old_center = (old_area[0] + old_area[2]) / 2.0;
}



sad::animations::SavedObjectPosition::~SavedObjectPosition() 
{
    PROFILER_EVENT;
    if (m_body)
    {
        m_body->delRef();
    }
}

void sad::animations::SavedObjectPosition::restore()
{
    PROFILER_EVENT;
    sad::Rect2D area =  m_object->getProperty<sad::Rect2D>("area").value();
    const sad::Point2D center = (area[0] + area[2]) / 2.0;

    sad::Rect2D new_area = area;
    sad::moveBy(m_old_center - center, new_area);
    m_object->setProperty("area", new_area);

    if (m_body)
    {
        m_body->shedulePosition(m_old_position);
    }
}

void sad::animations::SavedObjectPosition::storeBodyState(sad::p2d::Body* b)
{
    PROFILER_EVENT;
    if (m_body)
    {
        m_body->delRef();
    }
    m_body = b;
    m_old_position = m_body->position();
    m_body->addRef();
}
