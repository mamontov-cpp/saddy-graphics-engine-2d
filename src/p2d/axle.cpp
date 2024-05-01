#include "p2d/axle.h"
#include "p2d/collides1d.h"
#include "p2d/point.h"
#include "opticksupport.h"

sad::p2d::Axle sad::p2d::axle(const sad::Point2D & p1, const sad::Point2D & p2)
{
    PROFILER_EVENT;
    return sad::p2d::unit(p2-p1);
}

sad::p2d::Cutter1D sad::p2d::project(const sad::p2d::Cutter2D & c, 
                                const sad::p2d::Axle & a)
{
    PROFILER_EVENT;
    double x1 = p2d::scalar(c.p1(), a);
    double x2 = p2d::scalar(c.p2(), a);
    return p2d::Cutter1D(x1, x2);
}

sad::p2d::Cutter1D sad::p2d::project(const sad::Point2D & p1,
                                     const sad::Point2D & p2,
                                     const p2d::Axle & a)
{
    PROFILER_EVENT;
    p2d::Cutter2D c(p1, p2);
    return p2d::project(c, a);
}

bool sad::p2d::collides(const sad::p2d::Cutter1D & c1, 
                   const sad::p2d::Cutter1D & c2)
{
    PROFILER_EVENT;
    return sad::p2d::collides1D(c1.p1(), c1.p2(), c2.p1(), c2.p2());
}


sad::p2d::PointsPair sad::p2d::swap(const sad::p2d::PointsPair & c)
{
    PROFILER_EVENT;
    return p2d::PointsPair(c.p2(), c.p1());
}

void sad::p2d::swap(sad::Vector<sad::p2d::PointsPair> & pairs)
{
    PROFILER_EVENT;
    for(size_t i = 0 ; i < pairs.size(); i++)
    {
        pairs[i] = p2d::swap(pairs[i]);
    }
}


sad::p2d::Cutter2D sad::p2d::cutter(double x1, double y1, double x2, double y2)
{
    PROFILER_EVENT;
    return sad::p2d::Cutter2D(sad::p2d::Point(x1, y1), sad::p2d::Point(x2, y2));
}
