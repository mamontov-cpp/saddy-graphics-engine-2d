#include <p2d/convexhull.h>
#include <p2d/collisionshape.h>
#include <p2d/infiniteline.h>

#include <geometry2d.h>
#include "opticksupport.h"

sad::p2d::ConvexHull::ConvexHull()
{
    PROFILER_EVENT;

}

sad::p2d::ConvexHull::ConvexHull(const sad::Vector<sad::p2d::Point> & set)
{
    PROFILER_EVENT;
    m_set = sad::p2d::graham_scan(set);
}

sad::p2d::ConvexHull sad::p2d::ConvexHull::uncheckedCreate(
    const sad::Vector<sad::p2d::Point> & set
)
{
    PROFILER_EVENT;
    sad::p2d::ConvexHull  hull;
    hull.m_set = set;
    return hull;
}

sad::p2d::ConvexHull sad::p2d::ConvexHull::getUnion(
    const sad::p2d::ConvexHull & o1, 
    const sad::p2d::ConvexHull & o2
)
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> set = o1.m_set;
    set << o2.m_set;
    return sad::p2d::ConvexHull(set);
}

size_t sad::p2d::ConvexHull::sides() const
{
    PROFILER_EVENT;
    if (m_set.size() < 2) return 0; 
    return m_set.size();
}

size_t sad::p2d::ConvexHull::points() const
{
    PROFILER_EVENT;
    return m_set.size();
}

sad::p2d::Cutter2D sad::p2d::ConvexHull::side(int number) const
{
    PROFILER_EVENT;
    assert(number > -1 && number < sides() );
    if (number == points() - 1)
    {
        return sad::p2d::Cutter2D(m_set[number], m_set[0]);
    }
    return sad::p2d::Cutter2D(m_set[number], m_set[number + 1]);
}

sad::p2d::Vector  sad::p2d::ConvexHull::normal(int number) const
{
    PROFILER_EVENT;
    assert(number > -1 && number < sides() );
    sad::p2d::Cutter2D c = side(number);
    sad::p2d::Point center = (c.p1() + c.p2()) / 2.0;
    sad::p2d::Vector sidevector = c.p2() - c.p1();
    sad::p2d::Vector result = sad::p2d::ortho(sidevector, sad::p2d::OrthoVectorIndex::OVI_DEG_90);	
    size_t count = points();
    bool notanormal = false;
    for(size_t point = 0; (point < count) && !notanormal; point++)
    {
        bool is_not_side = (point != number && point != number + 1);
        if (number == point - 1)
        {
            is_not_side = (point != number && point != 0);
        }
        if (is_not_side)
        {
            sad::p2d::Point point_to_center = m_set[point] - center;
            double projection = sad::p2d::scalar(point_to_center, result);
            if (projection > 0)
            {
                notanormal = true;
            }
        }
    }
    if (notanormal)
        result = sad::p2d::ortho( sidevector, sad::p2d::OrthoVectorIndex::OVI_DEG_270);
    return result;
}

sad::p2d::Cutter1D sad::p2d::ConvexHull::project(const sad::p2d::Axle & axle) const
{
    PROFILER_EVENT;
    return sad::p2d::projectPointSet(m_set, m_set.size(), axle);
}

void sad::p2d::ConvexHull::tryInsertAxle(sad::Vector<sad::p2d::Axle> & container, 
                                         const sad::p2d::Axle & axle) const
{
    PROFILER_EVENT;
    bool found = false;
    for(size_t i = 0 ; (i < container.size()) && !found; i++)
    {
        if (sad::equal(container[i], axle))
        {
            found = true;
        }
    }
    if (!found)
        container << axle;
}

void sad::p2d::ConvexHull::appendAxisForSide(
    sad::Vector<sad::p2d::Axle> & container, 
    int number
) const
{
    PROFILER_EVENT;
    sad::p2d::Cutter2D k = side(number);
    this->tryInsertAxle(container, sad::p2d::axle(k.p1(), k.p2()));
    sad::p2d::Axle ortho =  sad::p2d::ortho(k.p2() - k.p1(), sad::p2d::OrthoVectorIndex::OVI_DEG_90);
    this->tryInsertAxle(container, ortho);
}

void sad::p2d::ConvexHull::appendAxisForCollision(
    sad::Vector<sad::p2d::Axle> & container
) const
{
    PROFILER_EVENT;
    for(size_t i = 0; i < this->sides(); i++)
    {
        this->appendAxisForSide(container, i);
    }
}
bool sad::p2d::ConvexHull::collides(const sad::p2d::ConvexHull & c) const
{
    PROFILER_EVENT;
    if (this->points() == 0 || c.points() == 0)
    {
        return false;
    }
    if (this->points() == 1 && c.points() == 1)
    {
        return sad::equal(this->m_set[0], c.m_set[0]);
    }
    sad::Vector<sad::p2d::Axle> axles;
    this->appendAxisForCollision(axles);
    c.appendAxisForCollision(axles);

    bool collides = true;
    for(size_t i = 0 ; i < axles.size() && collides; i++)
    {
        sad::p2d::Cutter1D myproject = this->project(axles[i]);
        sad::p2d::Cutter1D cproject = c.project(axles[i]);
        bool axlecollides = sad::p2d::collides(myproject, cproject);
        collides = collides && axlecollides;
    }

    return collides;
}


sad::p2d::Vector sad::p2d::ConvexHull::getSumOfNormalsFor(const sad::p2d::Point & p) const
{
    PROFILER_EVENT;
    sad::p2d::Vector result(0,0);
    double nearest = 0;
    bool nearest_is_found = false;
    for(size_t i = 0; i < sides(); i++)
    {
        sad::p2d::Cutter2D k = side(i);
        sad::p2d::Vector  n = normal(i);
        double d = std::max( sad::p2d::scalar(p - k.p1(), n), sad::p2d::scalar(p - k.p2(), n) );
        // Find a distance between side and projection on its point
        if (sad::is_fuzzy_equal(d, nearest))
        {
            nearest_is_found = true;
            result += n;
        } 
        else
        {
            if (d > nearest || !nearest_is_found)
            {
                nearest_is_found = true;
                nearest = d;
                result = n;
            }
        }
    }
    if (sad::non_fuzzy_zero( sad::p2d::modulo(result) ))
    {
        result = sad::p2d::unit(result);
    }
    return result;
}


sad::p2d::Point sad::p2d::ConvexHull::center() const
{
    PROFILER_EVENT;
    sad::p2d::Point result;
    for(size_t i = 0;  i < points(); i++)
    {
        result += m_set[i];
    }
    result /= points();
    return result;
}


void sad::p2d::ConvexHull::buildHull()
{
    PROFILER_EVENT;
    m_set = sad::p2d::graham_scan(m_set);
}

void sad::p2d::ConvexHull::insertPointsFromShape(sad::p2d::CollisionShape * s)
{
    PROFILER_EVENT;
    s->populatePoints(m_set);
}

