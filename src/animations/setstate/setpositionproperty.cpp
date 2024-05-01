#include "animations/setstate/setpositionproperty.h"

#include "geometry2d.h"

#include "db/save.h"
#include "db/load.h"
#include "opticksupport.h"


sad::animations::setstate::SetPositionProperty::SetPositionProperty(sad::db::Object* o) : m_o(o)
{
    PROFILER_EVENT;
    
}

sad::animations::setstate::AbstractSetStateCommand* 
sad::animations::setstate::SetPositionProperty::clone() const
{
    PROFILER_EVENT;
    return new sad::animations::setstate::SetPositionProperty(m_o);
}

sad::animations::setstate::SetPositionProperty::~SetPositionProperty()
{
    PROFILER_EVENT;
    
}

void sad::animations::setstate::SetPositionProperty::call(const sad::Point2D& a)
{
    PROFILER_EVENT;
    sad::Rect2D area = m_o->getProperty<sad::Rect2D>("area").value();
    sad::Point2D center = (area[0] + area[2]) / 2.0;

    sad::moveBy(a - center, area);
    m_o->setProperty("area", area);
}
