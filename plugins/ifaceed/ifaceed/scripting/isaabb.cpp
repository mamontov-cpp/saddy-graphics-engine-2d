#include "isaabb.h"

#include <geometry2d.h>

scripting::IsAABB::IsAABB()
{
    
}

scripting::IsAABB::~IsAABB()
{
    
}

sad::Maybe<QString> scripting::IsAABB::check(const sad::Rect2D& r)
{
    sad::Maybe<QString> result;
    if (!sad::isAABB(r))
    {
        result.setValue("set(): argument 3 is not axis-aligned box");
    }
    return result;
}
