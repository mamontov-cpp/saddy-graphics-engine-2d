#include "p2d/findcontactpoints.h"
#include <sadlinkedlist.h>
#include <geometry2d.h>
#include <limits>

#undef max
#undef min


#define EXCEPTION_MESSAGE (const char*)"Cannot determine contact points for given arguments"

sad::p2d::CannotDetermineContactPoints::CannotDetermineContactPoints()
: std::logic_error(EXCEPTION_MESSAGE)
{

}


void sad::p2d::FindContactPoints::reverse(sad::p2d::SetOfPointsPair & pairs)
{
    sad::p2d::swap(pairs);
}

void sad::p2d::FindContactPoints::init()
{
    this->sad::p2d::CollisionMultiMethodWithArg<sad::p2d::SetOfPointsPair, 
                                                sad::p2d::Vector
                                               >::init();
    add(sad::p2d::FindContactPoints::getRtoR);
    add(sad::p2d::FindContactPoints::getRtoL);
    add(sad::p2d::FindContactPoints::getRtoC);
    add(sad::p2d::FindContactPoints::getCtoC);
    add(sad::p2d::FindContactPoints::getLtoC);
    add(sad::p2d::FindContactPoints::getLtoL);
    add(sad::p2d::FindContactPoints::getBtoB);
    add(sad::p2d::FindContactPoints::getBtoP<sad::p2d::Rectangle>);
    add(sad::p2d::FindContactPoints::getBtoP<sad::p2d::Line>);
    add(sad::p2d::FindContactPoints::getBtoC);
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getRtoR(
         sad::p2d::Rectangle * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Rectangle * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getRtoC(
         sad::p2d::Rectangle * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Circle * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1->toHull(), v1, s2, v2);
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getRtoL(
         sad::p2d::Rectangle * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Line * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getCtoC(
         sad::p2d::Circle * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Circle * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1, v1, s2, v2);
}



sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getLtoC(
         sad::p2d::Line * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Circle * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1->toHull(), v1, s2, v2);
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getLtoL(
         sad::p2d::Line * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Line * s2,
         const sad::p2d::Vector & v2
)
{
    return sad::p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

void sad::p2d::insertUnique(
    sad::p2d::SetOfPointsPair & set, 
    const sad::p2d::Point & p1,
    const sad::p2d::Point & p2
)
{
    sad::p2d::insertUnique(set, sad::p2d::PointsPair(p1, p2));
}

void sad::p2d::insertUnique(
    sad::p2d::SetOfPointsPair & set, 
    const sad::p2d::PointsPair & pair
)
{
    bool found = false;
    for(size_t i = 0; i < set.size(); i++)
    {
        if (sad::equal(set[i].p1(), pair.p1()) && sad::equal(set[i].p2(), pair.p2()))
        {
            found = true;
        }
    }
    if (!found)
    {
        set << pair;
    }
}

void sad::p2d::merge(sad::p2d::SetOfPointsPair & set1, const sad::p2d::SetOfPointsPair & set2)
{
    for (size_t i = 0 ; i < set2.size(); i++)
    {
        sad::p2d::insertUnique(set1, set2[i]);
    }
}

void sad::p2d::filterOptimalSet(sad::p2d::SetOfPointsPair & set, const sad::p2d::Vector & v)
{
    double vm = sad::p2d::modulo(v);
    if (fabs(vm) < 0.0001)
        set.clear();
    // A vector of times for each pair
    sad::LinkedList<double> ts;
    double min = std::numeric_limits<double>::max();
    for(size_t i = 0; i < set.size(); i++)
    {
        double t = sad::p2d::scalar(set[i].p2() - set[i].p1(), v); 
        if (t < min) min = t;
        ts << t;
    }
    for(size_t i = 0; i < set.size(); i++)
    {
        if (!sad::is_fuzzy_equal(ts[i], min))
        {
            ts.removeAt(i);
            set.removeAt(i);
            --i;
        }
    }

}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::exec(
         const sad::p2d::ConvexHull & c1, 
         const sad::p2d::Vector & v1,
         const sad::p2d::ConvexHull & c2,
         const sad::p2d::Vector & v2
)
{
    sad::p2d::Vector v = v1 - v2;
    sad::p2d::SetOfPointsPair result;
    bool xlessthanzero = (v.x() < 0 && sad::non_fuzzy_zero(v.x()));
    bool ylessthanzero = (v.y() < 0 && sad::non_fuzzy_zero(v.y()));
    /*
    if (xlessthanzero && ylessthanzero)
    {
        return result;
    }
    */
    for(size_t i = 0 ; i < c1.sides(); i++)
    {
        for(size_t j = 0; j < c2.sides(); j++)
        {
            sad::p2d::Cutter2D s1 = c1.side(i);
            sad::p2d::Cutter2D s2 = c2.side(j);
            sad::p2d::MaybePoint tmp;
            tmp = sad::p2d::intersection(s1.p1(), v, s2);
            if (tmp.exists())  sad::p2d::insertUnique(result, s1.p1(), tmp.value()); 
            tmp = sad::p2d::intersection(s1.p2(), v, s2);
            if (tmp.exists())  sad::p2d::insertUnique(result, s1.p2(), tmp.value());
            tmp = sad::p2d::intersection(s2.p1(), v, s1);
            if (tmp.exists())  sad::p2d::insertUnique(result, tmp.value(), s2.p1());
            tmp = sad::p2d::intersection(s2.p2(), v, s1);
            if (tmp.exists())  sad::p2d::insertUnique(result, tmp.value(), s2.p2());
        }
    }
    sad::p2d::filterOptimalSet(result, v);
    return result;
}

sad::p2d::Point sad::p2d::intersectionWithNormalFrom(
    const sad::p2d::Point & p,
    const sad::p2d::Cutter2D & c
)
{
    sad::p2d::InfiniteLine l1 = sad::p2d::InfiniteLine::fromCutter(c);
    sad::p2d::Vector v = sad::p2d::ortho(l1.direction(), sad::p2d::OVI_DEG_90);
    sad::p2d::InfiniteLine l2 = sad::p2d::InfiniteLine::appliedVector(p, v);
    return l1.intersection(l2).value();
}

sad::Vector<sad::p2d::Point> sad::p2d::intersection(
    const sad::p2d::InfiniteLine & l,
    const sad::p2d::Circle * ci
)
{
    sad::Vector<sad::p2d::Point> result;
    double R = ci->radius();
    if (sad::is_fuzzy_zero(l.kx()) && sad::non_fuzzy_zero(l.ky()))
    {
        double y = - l.b() /l.ky();
        double dy = (y - ci->center().y());
        if (dy * dy <= R * R)
        {
            double sqD = sqrt(R * R - dy * dy);
            double x1 = ci->center().x() + sqD;
            double x2 = ci->center().x() - sqD;
            result << sad::p2d::Point(x1, y);
            result << sad::p2d::Point(x2, y);
        }
    }
    if (sad::non_fuzzy_zero(l.kx()) && sad::is_fuzzy_zero(l.ky()))
    {
        double x = - l.b() /l.kx();
        double dx = (x - ci->center().x());
        if (dx * dx <= R * R)
        {
            double sqD = sqrt(R * R - dx * dx);
            double y1 = ci->center().y() + sqD;
            double y2 = ci->center().y() - sqD;
            result << sad::p2d::Point(x, y1);
            result << sad::p2d::Point(x, y2);
        }
    }
    if (sad::non_fuzzy_zero(l.kx()) && sad::non_fuzzy_zero(l.ky()))
    {
        sad::p2d::Point c = ci->center();
        double xc = c.x();
        double yc = c.y();
        double kx = l.kx();
        double ky = l.ky();
        double b = l.b();
        double A = 1 + (kx * kx) / (ky * ky);
        double B = -2 * xc 
                 + 2 * b * kx / (ky  * ky)
                 + 2 * yc * kx / ky;
        double C = xc * xc 
                 + 2 * yc * b / ky
                 + b * b / (ky * ky)
                 + yc * yc
                 - R * R;
        
        double D = B * B - 4 * A * C;
        if (D >= 0)
        {
            double sqD = sqrt(D);
            double x1 = (-B + sqD) / A / 2.0;
            double y1 = -b / ky - kx * x1 / ky;
            double x2 = (-B - sqD) / A / 2.0;
            double y2 = -b / ky - kx * x2 / ky;
            result << sad::p2d::Point(x1, y1);
            result << sad::p2d::Point(x2, y2);
        }
    }
    return result;
}

sad::p2d::SetOfPointsPair sad::p2d::findContacts(
    const sad::p2d::Cutter2D & c,
    const sad::p2d::Vector & v,
    const sad::p2d::Circle * ci
)
{
    sad::p2d::SetOfPointsPair result;
    if (sad::is_fuzzy_zero(sad::p2d::modulo(v))) 
        return result;
    sad::p2d::Point O = sad::p2d::intersectionWithNormalFrom(ci->center(), c);
    sad::p2d::MaybePoint  K;
    sad::p2d::Point O1;
    if (sad::equal(O, ci->center()) == false)
    {
        sad::p2d::InfiniteLine CO = sad::p2d::InfiniteLine::fromCutter(
                                                            sad::p2d::Cutter2D(ci->center(), O)
                                                           );
    
        sad::Vector<sad::p2d::Point> tmppoints = sad::p2d::intersection(CO, ci);
        sad::p2d::InfiniteLine C1C2 = sad::p2d::InfiniteLine::fromCutter(c);
        sad::Vector<sad::p2d::Point> Ks;
        double min = std::numeric_limits<double>::max();
        int mini = 0;
        for(size_t i = 0; i < tmppoints.size();i ++)
        {
            sad::p2d::InfiniteLine O1V = sad::p2d::InfiniteLine::appliedVector(tmppoints[i], v);
            sad::p2d::MaybePoint F =  C1C2.intersection(O1V);
            if (F.exists()) 
            {
                Ks << F.value(); 
                double sc = sad::p2d::scalar(tmppoints[i] - F.value(), v);
                if (sc < min)
                {
                    mini = i;
                    min = sc;
                }
            }
            else
            {
                tmppoints.removeAt(i);
                --i;
            }
        }
        if (tmppoints.size() == 0) return result;
        O1 = tmppoints[mini];
        K.setValue(Ks[mini]);
    } 
    else
    {
        sad::p2d::InfiniteLine line = sad::p2d::InfiniteLine::appliedVector(ci->center(), v); 
        sad::Vector<sad::p2d::Point> points = sad::p2d::intersection(line, ci);
        if (points.size() == 2)
        {
            O1 = O;
            double t1 = sad::p2d::scalar(points[0] - O, v);
            double t2 = sad::p2d::scalar(points[1] - O, v);
            if (t1 < t2)
            {
                K.setValue(points[0]);
            }
            else
            {
                K.setValue(points[1]);
            }
        
        }
    }
    if (K.exists())
    {
        if (sad::projectionIsWithin(K.value(), c.p1(), c.p2()))
        {
            double dx = O1.x() - ci->center().x();
            double dy = O1.y() - ci->center().y();
            // If O1 belongs to circle
            if (sad::is_fuzzy_equal(dx* dx + dy * dy, ci->radius() * ci->radius()))
            {
                result << sad::p2d::PointsPair(K.value(), O1);	
            } 
            else
            {
                result << sad::p2d::PointsPair(O1, K.value());
            }
        }
        {
            sad::p2d::InfiniteLine l = sad::p2d::InfiniteLine::appliedVector(c.p1(), v);
            sad::Vector<sad::p2d::Point> pts = sad::p2d::intersection(l, ci);
            for(size_t i = 0; i < pts.size(); i++)
            {
                result << sad::p2d::PointsPair(c.p1(), pts[i]);
            }
        }
        {
            sad::p2d::InfiniteLine l = sad::p2d::InfiniteLine::appliedVector(c.p2(), v);
            sad::Vector<sad::p2d::Point> pts = sad::p2d::intersection(l, ci);
            for(size_t i = 0; i < pts.size(); i++)
            {
                result << sad::p2d::PointsPair(c.p2(), pts[i]);
            }
        }
    }
    // If found two points, find one with minimal distance
    sad::p2d::filterOptimalSet(result, v);
    return result;
}


sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::exec(
         const sad::p2d::ConvexHull  & c1, 
         const sad::p2d::Vector & v1,
         const sad::p2d::Circle * c2,
         const sad::p2d::Vector & v2
)
{
    sad::p2d::SetOfPointsPair result;
    sad::p2d::Vector v = v1 - v2;
    sad::p2d::Point  dp = c2->center() - c1.center();
    double projection = sad::p2d::scalar(dp, v);
    if (sad::is_fuzzy_zero(projection))
    {
        return result;
    }
    for(size_t i = 0 ; i < c1.sides(); i++)
    {
        sad::p2d::Cutter2D c1i = c1.side(i);
        sad::p2d::SetOfPointsPair set = sad::p2d::findContacts(c1i, v, c2);
        sad::p2d::merge(result, set);
    }
    sad::p2d::filterOptimalSet(result, v);
    return result;
}


sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::exec(
         const sad::p2d::Circle * c1, 
         const sad::p2d::Vector & v1,
         const sad::p2d::Circle * c2,
         const sad::p2d::Vector & v2
)
{
    sad::p2d::SetOfPointsPair result;
    sad::p2d::Vector v = v1 - v2;
    bool xlessthanzero = (v.x() < 0 && sad::non_fuzzy_zero(v.x()));
    bool ylessthanzero = (v.y() < 0 && sad::non_fuzzy_zero(v.y()));
    bool iszero = sad::is_fuzzy_zero(sad::p2d::modulo(v));
    if ((xlessthanzero && ylessthanzero) || iszero)
    {
        //return result;
    }
    
    double x1 = c1->center().x();
    double y1 = c1->center().y();
    double R1  = c1->radius();
    double x2 = c2->center().x();
    double y2 = c2->center().y();
    double R2  = c2->radius();
    
    double dx = x1 - x2;
    double dy = y1 - y2;
    double D2 = (R1 + R2) * (R1 + R2);

    double t = 0;
    {
        double A  = v.x() * v.x() + v.y() * v.y();
        double B = 2 * (dx * v.x() + dy * v.y());
        double C = dx * dx + dy * dy - D2;
        double DI = B * B - 4 * A * C;
        // Not a quadratic equation
        if (sad::is_fuzzy_zero(A) && sad::is_fuzzy_zero(B) && !sad::is_fuzzy_zero(C))
        {
            return result;
        }
        if (DI < 0) return result;
        double sqDI = sqrt(DI);
        double t1 = (-B - sqDI) / 2.0 / A;
        double t2 = (-B + sqDI) / 2.0 / A;

        // Pick a correct toi
        t = std::min(t1, t2);
    }
    
    x1 += v1.x() * t;
    y1 += v1.y() * t;

    x2 += v2.x() * t;
    y2 += v2.y() * t;

    // Compute contact point at TOI
    // If y1 != y2
    sad::p2d::Point commoncontact;
    double R1pR2 = (x2 * x2 - x1 * x1) + (y2 * y2 - y1 * y1);
    double DR =  ( R1pR2 - (R2 * R2 - R1 * R1) ) / 2;
        
    bool dynull = fabs(y1 - y2) < 1.0E-7;

    double x = 0, y = 0;
    double * py1 = &y1, * py2 = &y2;
    double * px1 = &x1, * px2 = &x2;
    double * px = &x,  * py = &y;
    if (dynull)
    {
        std::swap(px1, py1);
        std::swap(px2, py2);
        std::swap(px, py);

    }

    double A = DR / (*py1-*py2);
    double B = (*px1 - *px2) / (*py1 - *py2);

    double K1 = (1 + B * B);
    double K2 = 2 * ((A + *py1) * B - *px1);
    double K3 = (*px1) * (*px1) + (*py1 + A) * (*py1 + A) - R1 * R1;

    // BUG: Ignore it, so we can work around
    //double DI = K2 * K2 - 4 * K1 *K3;
    // This precision is taken empirically
    //if (DI < 0 && fabs(DI) > 1E-5)
    //	return result;

    *px = -K2 / K1 / 2;
    *py = -A - B * (*px);
    commoncontact.setX(x);
    commoncontact.setY(y);
    // Compute current contact points
    result << sad::p2d::PointsPair( commoncontact - v1 * t, commoncontact - v2 * t);
    return result;
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getBtoB(
         sad::p2d::Bound * s1, 
         const sad::p2d::Vector & v1,
         sad::p2d::Bound * s2,
         const sad::p2d::Vector & v2
)
{
    // In this cases, bounds are always colliding, so we should test a bounds for collision
    if (s1->isOrthogonal(s2) || s1->type() == s2->type())
    {
        throw sad::p2d::CannotDetermineContactPoints();
    }

    sad::p2d::SetOfPointsPair result;
    double minv = std::numeric_limits<double>::min();
    double maxv = std::numeric_limits<double>::max();

    sad::p2d::Vector v = v2 - v1;
    if ( (s1->type() == sad::p2d::BT_LEFT && s2->type() == sad::p2d::BT_RIGHT)
        || (s1->type() == sad::p2d::BT_RIGHT && s2->type() == sad::p2d::BT_LEFT))
    {
        if (sad::non_fuzzy_zero(v.x()))
        {
         result << sad::p2d::PointsPair(sad::p2d::Point(s1->position(), minv), 
                                        sad::p2d::Point(s2->position(), minv));	
         result << sad::p2d::PointsPair(sad::p2d::Point(s1->position(), maxv), 
                                        sad::p2d::Point(s2->position(), maxv));	
        }
    }

    if ( (s1->type() == sad::p2d::BT_UP && s2->type() == sad::p2d::BT_DOWN)
        || (s2->type() == sad::p2d::BT_UP && s1->type() == sad::p2d::BT_DOWN))
    {
        if (sad::non_fuzzy_zero(v.y()))
        {
         result << sad::p2d::PointsPair(sad::p2d::Point(minv, s1->position()), 
                                        sad::p2d::Point(minv, s2->position()));	
         result << sad::p2d::PointsPair(sad::p2d::Point(maxv, s1->position()), 
                                        sad::p2d::Point(maxv, s2->position()));	
        }
    }
    
    return result;
}


bool sad::p2d::hasPair(const sad::p2d::SetOfPointsPair & set,
                       double x1, double y1,
                       double x2, double y2)
{
    sad::p2d::Point p1(x1, y1);
    sad::p2d::Point p2(x2, y2);
    bool exists = false;
    for(size_t i = 0; i < set.size(); i++)
    {
        if (sad::equal(set[i].p1(), p1) && sad::equal(set[i].p2(), p2))
        {
            exists = true;
        }
    }
    return exists;
}



template<typename _Polygon>
sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getBtoP(
         sad::p2d::Bound * s1,
         const sad::p2d::Vector & v1,
         _Polygon * s2,
         const sad::p2d::Vector & v2
)
{
    sad::p2d::Vector v = v2 - v1;

    sad::Vector<sad::p2d::Point> p2 = s2->points();
    sad::p2d::SetOfPointsPair result;

    sad::p2d::Vector normal = s1->normal();
    sad::p2d::Vector directionvector = normal * (-1);
    sad::p2d::InfiniteLine bound = s1->boundingLine();
    
    double vp = sad::p2d::scalar(v, directionvector);
    // If cannot find, result is empty
    if (sad::is_fuzzy_zero(vp))
        return result;
    // If moving in opposite direction, it means it moving away from collisio, so contact
    // points cannot be determined
    if (vp < 0)
        throw sad::p2d::CannotDetermineContactPoints();

    // Find points with minimum scalar productions on vector
    sad::Vector<sad::p2d::Point> maxpoints;
    double maxprojection = - (std::numeric_limits<double>::max() - 1.0);
    for(size_t i = 0 ; i < p2.size(); i++)
    {
        double projection = sad::p2d::scalar(directionvector, p2[i]);
        if (sad::is_fuzzy_equal(projection, maxprojection))
        {
            maxpoints << p2[i];
        }
        else
        {
            if (projection > maxprojection)
            {
                maxpoints.clear();
                maxpoints << p2[i];
                maxprojection = projection;
            }
        }
    }

    for(size_t i = 0; i < maxpoints.size(); i++)
    {
        sad::p2d::InfiniteLine line = sad::p2d::InfiniteLine::appliedVector(maxpoints[i], v);
        sad::p2d::MaybePoint intersection = line.intersection(bound);
        result << sad::p2d::PointsPair(intersection.value(), maxpoints[i]);
    }
    return result;
}

sad::p2d::SetOfPointsPair sad::p2d::FindContactPoints::getBtoC(
         sad::p2d::Bound * s1,
         const sad::p2d::Vector & v1,
         sad::p2d::Circle * s2,
         const sad::p2d::Vector & v2	
)
{
    sad::p2d::Vector v = v2 - v1;

    sad::p2d::SetOfPointsPair result;

    sad::p2d::Vector normal = s1->normal();
    sad::p2d::Vector directionvector = normal * (-1);
    sad::p2d::InfiniteLine bound = s1->boundingLine();
    
    double vp = sad::p2d::scalar(v, directionvector);
    // If cannot find, result is empty
    if (sad::is_fuzzy_zero(vp))
        return result;
    // If moving in opposite direction, it means it moving away from collisio, so contact
    // points cannot be determined
    if (vp < 0)
        throw sad::p2d::CannotDetermineContactPoints();

    sad::p2d::Point circlecontactpoint = s2->center() + directionvector * s2->radius();
    sad::p2d::InfiniteLine line = sad::p2d::InfiniteLine::appliedVector(circlecontactpoint, v);
    sad::p2d::MaybePoint intersection = line.intersection(bound);
    result << sad::p2d::PointsPair(intersection.value(), circlecontactpoint);
    return result;
}
