#include "p2d/circletohulltransformer.h"
#include "p2d/circle.h"
#include "opticksupport.h"

sad::p2d::CircleToHullTransformer sad::p2d::CircleToHullTransformer::m_default(8);

sad::p2d::CircleToHullTransformer * sad::p2d::CircleToHullTransformer::ref()
{
    PROFILER_EVENT;
    return &p2d::CircleToHullTransformer::m_default;
}

sad::p2d::CircleToHullTransformer::~CircleToHullTransformer()
{
    PROFILER_EVENT;
}

sad::p2d::ConvexHull sad::p2d::CircleToHullTransformer::toHull(const sad::p2d::Circle * c)
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> set;
    populate(c, set);
    return sad::p2d::ConvexHull::uncheckedCreate(set);
}

void sad::p2d::CircleToHullTransformer::populate(
    const sad::p2d::Circle * c, 
    sad::Vector<sad::p2d::Point> &  v
) const
{
    PROFILER_EVENT;
    sad::p2d::Point center = c->center();
    double r = c->radius();
    for(int i = 0;i < m_sides; i++)
    {
        double parts = M_PI * 2 / m_sides;
        double angle = parts * i;
        v <<  center + sad::Point2D(cos(angle), sin(angle)) * r;  
    }
}

