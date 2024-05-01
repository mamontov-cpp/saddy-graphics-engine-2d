#include "p2d/infiniteline.h"
#include <geometry2d.h>
#include "opticksupport.h"



sad::p2d::InfiniteLine sad::p2d::InfiniteLine::fromCutter(const sad::p2d::Cutter2D & c)
{
    PROFILER_EVENT;
    double dx = c.p2().x() - c.p1().x();
    double dy = c.p2().y() - c.p1().y();
    if (sad::is_fuzzy_zero(dy))
    {
        return sad::p2d::InfiniteLine(0, 1, - (c.p1().y()));
    } 
    else
    {
        double a = dx/dy;
        double kx = 1 / sqrt(1 + a * a);
        double ky = -a * kx;
        double b =  0  - kx * c.p1().x() - ky * c.p1().y();
        return sad::p2d::InfiniteLine(kx, ky, b);
    }
}

sad::p2d::InfiniteLine sad::p2d::InfiniteLine::appliedVector(const sad::p2d::Point & p, 
                                                             const sad::p2d::Vector & v)
{
    PROFILER_EVENT;
    sad::p2d::Vector u = p2d::unit(v);
    if (sad::is_fuzzy_zero(u.x()))
    {
        return sad::p2d::InfiniteLine(1, 0, -p.x());
    }
    if (sad::is_fuzzy_zero(u.y()))
    {
        return sad::p2d::InfiniteLine(0, 1, -p.y());
    }
    double kx = 1.0 / u.x();
    double ky = -1.0 / u.y();
    double b = p.y() / u.y() - p.x() / u.x();
    return sad::p2d::InfiniteLine(kx, ky, b);
}


bool sad::p2d::InfiniteLine::isSame(const sad::p2d::InfiniteLine & a) const
{
    PROFILER_EVENT;
    double determinant =	m_kx * a.m_ky - m_ky * a.m_kx;
    bool result = false;
    if (sad::is_fuzzy_zero(determinant))
    {
        double fdet = m_kx * a.m_b - m_b * a.m_kx;
        double sdet = m_ky * a.m_b - m_b * a.m_ky;
        result = sad::is_fuzzy_zero(fdet) && sad::is_fuzzy_zero(sdet);
    }
    return result;
}


bool sad::p2d::InfiniteLine::isCollinear(const sad::p2d::InfiniteLine & a) const
{
    PROFILER_EVENT;
    double determinant =	m_kx * a.m_ky - m_ky * a.m_kx;
    bool result = false;
    if (sad::is_fuzzy_zero(determinant))
    {
        double fdet = m_kx * a.m_b - m_b * a.m_kx;
        double sdet = m_ky * a.m_b - m_b * a.m_ky;
        result = !sad::is_fuzzy_zero(fdet) || !sad::is_fuzzy_zero(sdet);
    }
    return result;
}


sad::p2d::Point sad::p2d::InfiniteLine::randomPoint() const
{
    PROFILER_EVENT;
    sad::p2d::Point result;
    if (sad::non_fuzzy_zero(m_kx) || sad::non_fuzzy_zero(m_ky))
    {
        if (sad::is_fuzzy_zero(m_ky))
        {
            result = sad::p2d::Point(-m_b/-m_kx, 0);
        }
        else
        {
            result = sad::p2d::Point(0, -m_b/m_ky);
        }		
    }
    return result;
}

sad::p2d::MaybePoint sad::p2d::InfiniteLine::intersection(const sad::p2d::InfiniteLine & a) 
const
{
    PROFILER_EVENT;
    sad::p2d::MaybePoint result;
    bool same = isSame(a);
    bool iscollinear = isCollinear(a);
    bool haskzero = sad::is_fuzzy_zero(m_kx) && sad::is_fuzzy_zero(m_ky);
    bool hasbzero = sad::is_fuzzy_zero(m_b);
    if (same || (!iscollinear && haskzero && hasbzero))
    {
        result.setValue(randomPoint());
    } 
    else
    {
        if (!isCollinear(a) && !haskzero)
        {
            if (sad::is_fuzzy_zero(m_ky))
            {
                double x =  - m_b / m_kx;
                // If a.m_ky is zero than they are collinear, so
                // we can divide here
                double y =  (- a.m_b - a.m_kx * x) / a.m_ky;
                result.setValue(p2d::Point(x, y));
            }
            else
            {
                double nx = a.m_b - a.m_ky /m_ky * m_b;
                double dx = a.m_kx - a.m_ky /m_ky * m_kx;
                double x = - nx / dx; 
                double y =  (- m_b - m_kx * x) / m_ky;
                result.setValue(p2d::Point(x, y));				
            }
        }
    }
    return result;
}

sad::p2d::MaybePoint sad::p2d::InfiniteLine::intersection(const p2d::Cutter2D & a) const
{
    PROFILER_EVENT;
    sad::p2d::MaybePoint result;
    sad::p2d::InfiniteLine l  = sad::p2d::InfiniteLine::fromCutter(a);
    if (isSame(l))
    {
        result.setValue(a.p1());
    }
    else
    {
        sad::p2d::MaybePoint tmp = intersection(l);
        if (tmp.exists())
        {
            if (sad::projectionIsWithin(tmp.value(), a.p1(), a.p2()))
            {
                result = tmp;
            }
        }
    }
    return result;
}

bool sad::p2d::InfiniteLine::hasPoint(const p2d::Point & p) const
{
    PROFILER_EVENT;
    return sad::is_fuzzy_zero(m_kx * p.x() + m_ky * p.y() + m_b);
}


sad::p2d::MaybePoint sad::p2d::intersection(const sad::p2d::Cutter2D & a, 
                                            const sad::p2d::Cutter2D & b)
{
    PROFILER_EVENT;
    sad::p2d::InfiniteLine a1 = sad::p2d::InfiniteLine::fromCutter(a);
    sad::p2d::InfiniteLine a2 = sad::p2d::InfiniteLine::fromCutter(b);
    sad::p2d::MaybePoint  result;
    sad::p2d::MaybePoint  tmp = a1.intersection(a2);
    if (tmp.exists())
    {
        sad::p2d::Point p = tmp.value();
        bool isw1 = sad::projectionIsWithin(p, a.p1(), a.p2());
        bool isw2 = sad::projectionIsWithin(p, b.p1(), b.p2());
        if (isw1 && isw2)
        {
            result.setValue(p);
        }
    }
    return result;
}

sad::p2d::MaybePoint sad::p2d::intersection(
    const sad::p2d::Point & x, 
    const sad::p2d::Vector & v, 
    const sad::p2d::Cutter2D & c
)
{
    PROFILER_EVENT;
    sad::p2d::InfiniteLine l1 = sad::p2d::InfiniteLine::appliedVector(x, v);
    sad::p2d::InfiniteLine l2 = sad::p2d::InfiniteLine::fromCutter(c);
    sad::p2d::MaybePoint result;
    if (l1.isSame(l2))
    {
        double d1 = x.distance(c.p1());
        double d2 = x.distance(c.p2());
        if (d1 < d2) 
            result.setValue(c.p1()); 
        else 
            result.setValue(c.p2());
    }
    else
    {
        result = l1.intersection(c); 
    }
    return result;
}

sad::p2d::Vector sad::p2d::InfiniteLine::direction() const
{
    PROFILER_EVENT;
    assert( sad::non_fuzzy_zero(m_kx) || sad::non_fuzzy_zero(m_ky) );
    if (sad::is_fuzzy_zero(m_kx))
    {
        return p2d::Vector(1, 0);
    }
    if (sad::is_fuzzy_zero(m_ky))
    {
        return p2d::Vector(0, 1);
    }
    return p2d::unit(p2d::Vector(1.0 / m_kx, -1.0 / m_ky));
}
