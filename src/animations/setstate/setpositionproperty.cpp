#include "animations/setstate/setpositionproperty.h"

#include "geometry2d.h"

#include "db/save.h"
#include "db/load.h"


sad::animations::setstate::SetPositionProperty::SetPositionProperty(sad::db::Object* o) : m_o(o)
{
    
}

sad::animations::setstate::AbstractSetStateCommand* 
sad::animations::setstate::SetPositionProperty::clone() const
{
    return new sad::animations::setstate::SetPositionProperty(m_o);
}

sad::animations::setstate::SetPositionProperty::~SetPositionProperty()
{
    
}

void sad::animations::setstate::SetPositionProperty::call(const sad::Point2D& a)
{
    sad::Rect2D area = m_o->getProperty<sad::Rect2D>("area").value();
    sad::Point2D center = (area[0] + area[2]) / 2.0;

    sad::moveBy(a - center, area);
    m_o->setProperty("area", area);
}

