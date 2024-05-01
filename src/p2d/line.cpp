#include "p2d/line.h"
#include "matrix2x2.h"
#include "log/log.h"
#include <algorithm>
#include "opticksupport.h"


sad::p2d::CollisionShape * sad::p2d::Line::clone(int count) const
{
    PROFILER_EVENT;
    sad::p2d::Line * b = new sad::p2d::Line[count]();
    std::fill_n(b, count, *this);
    return b;
}


sad::p2d::Point sad::p2d::Line::center() const
{
    PROFILER_EVENT;
    sad::p2d::Point r = m_c.p1();
    r += m_c.p2();
    r /= 2.0;
    return r;
}

void sad::p2d::Line::rotate(double angle)
{
    PROFILER_EVENT;
    sad::p2d::Point center = this->center();	
    sad::p2d::Matrix2x2 m = sad::p2d::Matrix2x2::counterclockwise(angle);
    sad::p2d::Point x1 = m *(this->m_c.p1() - center ) + center;
    sad::p2d::Point x2 = m *(this->m_c.p2() - center ) + center;
    setCutter(sad::p2d::Cutter2D(x1 , x2) );
}

void sad::p2d::Line::move(const sad::p2d::Vector & d)
{
    PROFILER_EVENT;
    m_c._1() += d;
    m_c._2() += d;
}

sad::p2d::ConvexHull sad::p2d::Line::toHull() const
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> set;
    set << m_c.p1();
    set << m_c.p2();

    return sad::p2d::ConvexHull(set);
}

sad::p2d::Cutter1D sad::p2d::Line::project(const p2d::Axle & a) const
{
    PROFILER_EVENT;
    double a2 = sad::p2d::scalar(a, a);
    double p1 = sad::p2d::scalar(m_c.p1(), a) / a2;
    double p2 = sad::p2d::scalar(m_c.p2(), a) / a2;
    if (p1 > p2)
        std::swap(p1, p2);
    return sad::p2d::Cutter1D(p1, p2);
}

sad::Vector<sad::p2d::Point> sad::p2d::Line::points() const
{
    PROFILER_EVENT;
    sad::Vector<sad::p2d::Point> result;
    result << m_c.p1();
    result << m_c.p2();
    return result;
}



size_t sad::p2d::Line::sizeOfType() const
{
    PROFILER_EVENT;
    return sizeof(p2d::Line);
}

void sad::p2d::Line::populatePoints(sad::Vector<p2d::Point> & v) const
{
    PROFILER_EVENT;
    v << m_c.p1();
    v << m_c.p2();
}

void sad::p2d::Line::normalToPointOnSurface(const p2d::Point & p, p2d::Vector & n)
{
    PROFILER_EVENT;
    n = m_c.p2() - m_c.p1();
    sad::p2d::mutableUnit(n);
    sad::p2d::mutableNormalizedOrtho(n, p2d::OrthoVectorIndex::OVI_DEG_90);
}



sad::String sad::p2d::Line::dump() const
{
    PROFILER_EVENT;
    return str(fmt::Format("Line at ({0}, {1}) - ({2}, {3})")
                            << m_c.p1().x() << m_c.p1().y()
                            << m_c.p2().x() << m_c.p2().y()
              );
}


void sad::p2d::Line::resizeBy(const sad::p2d::Vector& v)
{
    PROFILER_EVENT;
    sad::p2d::Vector n = m_c.p2() - m_c.p1();
    sad::p2d::mutableUnit(n);
    double length = sad::p2d::scalar(n, v);
    m_c._2() += n * length;
    m_c._1() -= n * length;
}

unsigned int sad::p2d::Line::metaIndex()
{
    PROFILER_EVENT;
    return sad::p2d::Line::globalMetaIndex();
}

unsigned int sad::p2d::Line::globalMetaIndex()
{
    PROFILER_EVENT;
    return 2;
}

void sad::p2d::Line::deleteBlock(sad::p2d::CollisionShape* block)
{
    PROFILER_EVENT;
    delete[] static_cast<sad::p2d::Line*>(block);
}
