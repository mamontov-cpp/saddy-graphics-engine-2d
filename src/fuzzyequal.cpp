#include <fuzzyequal.h>

#include <cmath>
#include "opticksupport.h"

bool sad::is_fuzzy_equal(double x1, double x2, double precision)
{
    PROFILER_EVENT;
    return fabs(x1 - x2) < precision;
}

bool sad::is_fuzzy_zero(double x, double precision)
{
    PROFILER_EVENT;
    return fabs(x) < precision;
}

bool sad::non_fuzzy_zero(double x, double precision)
{
    PROFILER_EVENT;
    return !sad::is_fuzzy_zero(x, precision);
}

bool sad::equal(const sad::Point2D & p1, const sad::Point2D & p2, float precision)
{
    PROFILER_EVENT;
    bool e1 = sad::is_fuzzy_equal(p1.x(), p2.x(), precision);
    bool e2 = sad::is_fuzzy_equal(p1.y(), p2.y(), precision);
    return e1 && e2; 
}

bool sad::equal(const sad::Point3D & p1, const sad::Point3D & p2, float precision)
{
    PROFILER_EVENT;
    bool e1 = sad::is_fuzzy_equal(p1.x(), p2.x(), precision);
    bool e2 = sad::is_fuzzy_equal(p1.y(), p2.y(), precision);
    bool e3 = sad::is_fuzzy_equal(p1.z(), p2.z(), precision);
    return e1 && e2 && e3; 
}

bool sad::equal(const sad::Rect2D & p1, const sad::Rect2D & p2, float precision)
{
    PROFILER_EVENT;
    bool ok = true;
    for(int i = 0; i < 4; i++)
    {
        ok = ok && equal(p1[i], p2[i], precision);
    }
    return ok;
}

bool sad::equal(
    const sad::Rect<sad::Point3D> & p1, 
    const sad::Rect<sad::Point3D> & p2, 
    float precision
)
{
    PROFILER_EVENT;
    bool ok = true;
    for(int i = 0; i < 4; i++)
    {
        ok = ok && equal(p1[i], p2[i], precision);
    }
    return ok;
}
