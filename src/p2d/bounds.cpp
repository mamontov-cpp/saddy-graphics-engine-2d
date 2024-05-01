#include "p2d/bounds.h"

#include <log/log.h>

#include <limits>
#include <algorithm>

#undef min
#undef max
#include "opticksupport.h"

sad::p2d::CollisionShape * sad::p2d::Bound::clone(int count) const
{
    PROFILER_EVENT;
    sad::p2d::Bound * b = new sad::p2d::Bound[count]();
    std::fill_n(b, count, *this);
    return b;
}

sad::p2d::Point sad::p2d::Bound::center() const
{
    PROFILER_EVENT;
    return sad::p2d::Point();
}


void sad::p2d::Bound::rotate(double angle)
{
    PROFILER_EVENT;

}

void sad::p2d::Bound::move(const sad::p2d::Vector & d)
{
    PROFILER_EVENT;

}

sad::p2d::ConvexHull sad::p2d::Bound::toHull() const
{
    PROFILER_EVENT;
    return sad::p2d::ConvexHull();
}

sad::p2d::Cutter1D sad::p2d::Bound::project(const sad::p2d::Axle & a) const
{
    PROFILER_EVENT;
    return sad::p2d::Cutter1D();
}	


sad::p2d::Vector sad::p2d::Bound::normal()
{
    PROFILER_EVENT;
    sad::p2d::Vector result;
    switch(this->m_type)
    {
        case BoundType::BT_LEFT: { result = sad::p2d::Vector(1, 0); break; }
        case BoundType::BT_RIGHT: { result = sad::p2d::Vector(-1, 0); break; }
        case BoundType::BT_DOWN: { result = sad::p2d::Vector(0, 1); break; }
        case BoundType::BT_UP: { result = sad::p2d::Vector(0, -1); break; }
    }
    return result;
}


sad::p2d::InfiniteLine sad::p2d::Bound::boundingLine()
{
    PROFILER_EVENT;
    sad::p2d::InfiniteLine result;
    if (this->type() == sad::p2d::BoundType::BT_LEFT || this->type() == sad::p2d::BoundType::BT_RIGHT)
    {
        result = sad::p2d::InfiniteLine::appliedVector( 
            sad::p2d::Point(m_p, 0), 
            sad::p2d::Vector(0, 1) 
        );
    }
    else
    {
        result = sad::p2d::InfiniteLine::appliedVector( 
            sad::p2d::Point(0, m_p), 
            sad::p2d::Vector(1, 0) 
        );
    }
    return result;
}

size_t sad::p2d::Bound::sizeOfType() const
{
    PROFILER_EVENT;
    return sizeof(sad::p2d::Bound);
}

void sad::p2d::Bound::populatePoints(sad::Vector<sad::p2d::Point> & v) const
{
    PROFILER_EVENT;
    if (this->type() == sad::p2d::BoundType::BT_LEFT || this->type() == sad::p2d::BoundType::BT_RIGHT)
    {
        // Evade overflow
        v << sad::p2d::Point(m_p, std::numeric_limits<double>::max() * - 0.99999);
        v << sad::p2d::Point(m_p, std::numeric_limits<double>::max());
    }
    else
    {
        v << sad::p2d::Point(std::numeric_limits<double>::max() * - 0.99999, m_p);
        v << sad::p2d::Point(std::numeric_limits<double>::max(), m_p);
    }
}

void sad::p2d::Bound::normalToPointOnSurface(const sad::p2d::Point & p, sad::p2d::Vector & n)
{
    PROFILER_EVENT;
    // This function is duplicated for sake of optimization
    double x;
    double y;
    switch(this->m_type)
    {
        case sad::p2d::BoundType::BT_LEFT:  { x =  1; y =  0;  break; }
        case sad::p2d::BoundType::BT_RIGHT: { x = -1; y =  0;  break; }
        case sad::p2d::BoundType::BT_DOWN:  { x =  0; y =  1;  break; }
        case sad::p2d::BoundType::BT_UP:    { x =  0; y = -1;  break; }
    }
    n.setX(x);
    n.setY(y);
}

sad::String sad::p2d::Bound::dump() const
{
    PROFILER_EVENT;
    sad::String type;
    switch(this->m_type)
    {
        case sad::p2d::BoundType::BT_LEFT:  { type = "Left";  break; }
        case sad::p2d::BoundType::BT_RIGHT: { type = "Right";  break; }
        case sad::p2d::BoundType::BT_DOWN:  { type = "Bottom";  break; }
        case sad::p2d::BoundType::BT_UP:    { type = "Top";  break; }
    }
    return str(fmt::Format("{0} bound at {1}")
                            << type << m_p
              );
}

unsigned int sad::p2d::Bound::metaIndex()
{
    PROFILER_EVENT;
    return sad::p2d::Bound::globalMetaIndex();
}

unsigned int sad::p2d::Bound::globalMetaIndex()
{
    PROFILER_EVENT;
    return 3;
}

void sad::p2d::Bound::deleteBlock(sad::p2d::CollisionShape* block)
{
    PROFILER_EVENT;
    delete[] static_cast<sad::p2d::Bound*>(block);
}
