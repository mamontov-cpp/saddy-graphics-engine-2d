#include <p2d/circle.h>
#include <p2d/circletohulltransformer.h>

#include <log/log.h>

#include <algorithm>
#include "opticksupport.h"


sad::p2d::CollisionShape * sad::p2d::Circle::clone(int count) const
{
    PROFILER_EVENT;
    sad::p2d::Circle * b = new sad::p2d::Circle[count]();
    std::fill_n(b, count, *this);
    return b;
}

const sad::p2d::Point & sad::p2d::Circle::centerRef() const
{
    PROFILER_EVENT;
    return m_center;
}

sad::p2d::Point sad::p2d::Circle::center() const
{
    PROFILER_EVENT;
    return m_center;
}

void sad::p2d::Circle::rotate(double angle)
{
    PROFILER_EVENT;
}


void sad::p2d::Circle::move(const sad::p2d::Vector & d)
{
    PROFILER_EVENT;
    m_center += d;
}


sad::p2d::ConvexHull sad::p2d::Circle::toHull() const
{
    PROFILER_EVENT;
    sad::p2d::CircleToHullTransformer * t = this->m_transformer;
    if (t == nullptr) t = sad::p2d::CircleToHullTransformer::ref();
    return t->toHull(this);
}

sad::p2d::Cutter1D sad::p2d::Circle::project(const p2d::Axle & a) const
{
    PROFILER_EVENT;
    double a2 = sad::p2d::scalar(a, a);
    double rcenter = sad::p2d::scalar(m_center, a) / a2;
    return sad::p2d::Cutter1D(rcenter - m_radius, rcenter + m_radius);
}


size_t sad::p2d::Circle::sizeOfType() const
{
    PROFILER_EVENT;
    return sizeof(sad::p2d::Circle);
}

void sad::p2d::Circle::populatePoints(sad::Vector<p2d::Point> & v) const
{
    PROFILER_EVENT;
    sad::p2d::CircleToHullTransformer * t = this->m_transformer;
    if (t == nullptr) t = sad::p2d::CircleToHullTransformer::ref();
    t->populate(this, v);
}


void sad::p2d::Circle::normalToPointOnSurface(const sad::p2d::Point & p, sad::p2d::Vector & n)
{
    PROFILER_EVENT;
    n = p;
    n -= m_center;
    sad::p2d::mutableUnit(n);
}

sad::String sad::p2d::Circle::dump() const
{
    PROFILER_EVENT;
    return str(fmt::Format("Circle with center ({0},{1}) and radius {2}")
                            << m_center.x() << m_center.y() << m_radius
              );
}


void sad::p2d::Circle::resizeBy(const sad::p2d::Vector& v)
{
    PROFILER_EVENT;
    double modulo = sad::p2d::modulo(v);
    if (v.x() > 0 || v.y() > 0)
    {
        m_radius += modulo;
    }
    else
    {
        m_radius -= modulo;
    }
}

unsigned int sad::p2d::Circle::metaIndex()
{
    PROFILER_EVENT;
    return sad::p2d::Circle::globalMetaIndex();
}

unsigned int sad::p2d::Circle::globalMetaIndex()
{
    PROFILER_EVENT;
    return 1;
}

void sad::p2d::Circle::deleteBlock(sad::p2d::CollisionShape* block)
{
    PROFILER_EVENT;
    delete[] static_cast<sad::p2d::Circle*>(block);
}
