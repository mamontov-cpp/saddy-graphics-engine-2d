#include "p2d/rectangle.h"
#include <geometry2d.h>
#include "log/log.h"

#include <algorithm>
#include "opticksupport.h"


const sad::Point2D & sad::p2d::Rectangle::point(int index) const
{
    PROFILER_EVENT;
    assert(index > -1 && index< 4);
    return m_rect[index];
}

sad::p2d::CollisionShape * sad::p2d::Rectangle::clone(int count) const
{
    PROFILER_EVENT;
    sad::p2d::Rectangle * b = new sad::p2d::Rectangle[count]();
    std::fill_n(b, count, *this);
    return b;
}


sad::p2d::Point sad::p2d::Rectangle::center() const
{
    PROFILER_EVENT;
    sad::Point2D c = m_rect[0]; 
    c += m_rect[2]; 
    c/=2;
    return c;
}

void sad::p2d::Rectangle::rotate(double angle)
{
    PROFILER_EVENT;
    if (fabs(angle) > S2D_FP_PRECISION)
    {
        sad::rotate(m_rect, (float)angle);
    }
}

void sad::p2d::Rectangle::move(const sad::p2d::Vector & d)
{
    PROFILER_EVENT;
    sad::moveBy(d, m_rect);
}


sad::p2d::ConvexHull sad::p2d::Rectangle::toHull() const
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> set;
    for(int i = 0 ; i < 4; i++)
    {
        set << m_rect[i];
    }
    return sad::p2d::ConvexHull(set);
}


sad::p2d::Cutter1D sad::p2d::Rectangle::project(const sad::p2d::Axle & a) const
{
    PROFILER_EVENT;
    return sad::p2d::projectPointSet(m_rect, 4, a);
}



sad::Vector<sad::p2d::Point> sad::p2d::Rectangle::points() const
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> points;
    for(int i = 0; i < 4; i++)
    {
        points << m_rect[i];
    }
    return points;
}

size_t sad::p2d::Rectangle::sizeOfType() const
{
    PROFILER_EVENT;
    return sizeof(sad::p2d::Rectangle);
}

void sad::p2d::Rectangle::populatePoints(sad::Vector<sad::p2d::Point> & v) const
{
    PROFILER_EVENT;
    for(int i = 0; i < 4; i++)
    {
        v << m_rect[i];
    }
}


void sad::p2d::Rectangle::normalToPointOnSurface(const sad::p2d::Point & p, 
                                                sad::p2d::Vector & n)
{
    PROFILER_EVENT;
    sad::p2d::ConvexHull h;
    h.insertPointsFromShape(this);
    h.buildHull();
    n = h.getSumOfNormalsFor(p);
}

sad::String sad::p2d::Rectangle::dump() const
{
    PROFILER_EVENT;
    return str(fmt::Format("Rectangle:\n[{0}, {1} - {2}, {3}]\n[{4}, {5} - {6}, {7}]\n")
                            << m_rect[0].x() << m_rect[0].y()
                            << m_rect[1].x() << m_rect[1].y()
                            << m_rect[2].x() << m_rect[2].y()
                            << m_rect[3].x() << m_rect[3].y()
              );
}


void sad::p2d::Rectangle::makeConvex()
{
    PROFILER_EVENT;
    sad::p2d::ConvexHull h;
    h.insertPointsFromShape(this);
    h.buildHull();
    for(unsigned int i = 0; i < h.set().size(); i++)
    {
        this->m_rect[i] = h.set()[i];
    }
}


void sad::p2d::Rectangle::resizeBy(const sad::p2d::Vector& v)
{
    PROFILER_EVENT;
    m_rect[0] += sad::p2d::Vector(-v.x(), -v.y());
    m_rect[1] += sad::p2d::Vector(v.x(), -v.y());
    m_rect[2] += sad::p2d::Vector(v.x(), v.y());
    m_rect[3] += sad::p2d::Vector(-v.x(), v.y());
}

unsigned int sad::p2d::Rectangle::metaIndex()
{
    PROFILER_EVENT;
    return sad::p2d::Rectangle::globalMetaIndex();
}

unsigned int sad::p2d::Rectangle::globalMetaIndex()
{
    PROFILER_EVENT;
    return 0;
}

void sad::p2d::Rectangle::deleteBlock(sad::p2d::CollisionShape* block)
{
    PROFILER_EVENT;
    delete[] static_cast<sad::p2d::Rectangle*>(block);
}
