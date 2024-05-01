#include "animations/animationssavedrenderingstringlimit.h"
#include "label.h"
#include "db/custom/customobject.h"
#include "opticksupport.h"

sad::animations::SavedRenderingStringLimit::SavedRenderingStringLimit(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    PROFILER_EVENT;
    m_is_label = o->isInstanceOf("sad::Label");
}

sad::animations::SavedRenderingStringLimit::~SavedRenderingStringLimit()
{
    PROFILER_EVENT;

}

void sad::animations::SavedRenderingStringLimit::restore()
{
    PROFILER_EVENT;
    if (m_is_label)
    {
        static_cast<sad::Label*>(m_object)->clearRenderingStringLimit();
    }
    else
    {
        static_cast<sad::db::custom::Object*>(m_object)->clearRenderingStringLimit();
    }
}
