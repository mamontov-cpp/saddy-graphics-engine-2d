#include "animations/animationssavedrenderingstringlimit.h"
#include "label.h"
#include "db/custom/customobject.h"

sad::animations::SavedRenderingStringLimit::SavedRenderingStringLimit(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    m_is_label = o->isInstanceOf("sad::Label");
}

sad::animations::SavedRenderingStringLimit::~SavedRenderingStringLimit()
{

}

void sad::animations::SavedRenderingStringLimit::restore()
{
    if (m_is_label)
    {
        static_cast<sad::Label*>(m_object)->clearRenderingStringLimit();
    }
    else
    {
        static_cast<sad::db::custom::Object*>(m_object)->clearRenderingStringLimit();
    }
}
