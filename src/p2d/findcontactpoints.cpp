#include "p2d/findcontactpoints.h"
#include "extra/geometry2d.h"
#include <limits>

#undef max
#undef min

using namespace std;

#define MESSAGE (const char*)"Cannot determine contact points for given arguments"

p2d::CannotDetermineContactPoints::CannotDetermineContactPoints(): std::logic_error(MESSAGE)
{

}


void p2d::FindContactPoints::reverse(p2d::SetOfPointsPair & pairs)
{
	p2d::swap(pairs);
}

void p2d::FindContactPoints::init()
{
	add(p2d::FindContactPoints::getRtoR);
	add(p2d::FindContactPoints::getRtoL);
	add(p2d::FindContactPoints::getRtoC);
	add(p2d::FindContactPoints::getCtoC);
	add(p2d::FindContactPoints::getLtoC);
	add(p2d::FindContactPoints::getLtoL);
	add(p2d::FindContactPoints::getBtoB);
	add(p2d::FindContactPoints::getBtoP<p2d::Rectangle>);
	add(p2d::FindContactPoints::getBtoP<p2d::Line>);
	add(p2d::FindContactPoints::getBtoC);
}

p2d::SetOfPointsPair p2d::FindContactPoints::getRtoR(
		 p2d::Rectangle * s1, 
		 const p2d::Vector & v1,
		 p2d::Rectangle * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

p2d::SetOfPointsPair p2d::FindContactPoints::getRtoC(
		 p2d::Rectangle * s1, 
		 const p2d::Vector & v1,
		 p2d::Circle * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1->toHull(), v1, s2, v2);
}

p2d::SetOfPointsPair p2d::FindContactPoints::getRtoL(
		 p2d::Rectangle * s1, 
		 const p2d::Vector & v1,
		 p2d::Line * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

p2d::SetOfPointsPair p2d::FindContactPoints::getCtoC(
		 p2d::Circle * s1, 
		 const p2d::Vector & v1,
		 p2d::Circle * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1, v1, s2, v2);
}



p2d::SetOfPointsPair p2d::FindContactPoints::getLtoC(
		 p2d::Line * s1, 
		 const p2d::Vector & v1,
		 p2d::Circle * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1->toHull(), v1, s2, v2);
}

p2d::SetOfPointsPair p2d::FindContactPoints::getLtoL(
		 p2d::Line * s1, 
		 const p2d::Vector & v1,
		 p2d::Line * s2,
		 const p2d::Vector & v2
)
{
	return p2d::FindContactPoints::exec(s1->toHull(), v1, s2->toHull(), v2);
}

void p2d::insertUnique(
	p2d::SetOfPointsPair & set, 
	const p2d::Point & p1,
	const p2d::Point & p2
)
{
	p2d::insertUnique(set, p2d::PointsPair(p1, p2));
}

void p2d::insertUnique(
	p2d::SetOfPointsPair & set, 
	const p2d::PointsPair & pair
)
{
	bool found = false;
	for(size_t i = 0; i < set.size(); i++)
	{
		if (equal(set[i].p1(), pair.p1()) && equal(set[i].p2(), pair.p2()))
		{
			found = true;
		}
	}
	if (!found)
	{
		set << pair;
	}
}

void p2d::merge(p2d::SetOfPointsPair & set1, const p2d::SetOfPointsPair & set2)
{
	for (size_t i = 0 ; i < set2.size(); i++)
	{
		p2d::insertUnique(set1, set2[i]);
	}
}

void p2d::filterOptimalSet(p2d::SetOfPointsPair & set, const p2d::Vector & v)
{
	double vm = p2d::modulo(v);
	if (vm == 0)
		set.clear();
	// A vector of times for each pair
	hst::deque<double> ts;
	double min = std::numeric_limits<double>::max();
	for(int i = 0; i < set.size(); i++)
	{
		double t = p2d::scalar(set[i].p2() - set[i].p1(), v); 
		if (t < min) min = t;
		ts << t;
	}
	for(int i = 0; i < set.size(); i++)
	{
		if (!is_fuzzy_equal(ts[i], min))
		{
			ts.removeAt(i);
			set.removeAt(i);
			--i;
		}
	}

}

p2d::SetOfPointsPair p2d::FindContactPoints::exec(
		 const p2d::ConvexHull & c1, 
		 const p2d::Vector & v1,
		 const p2d::ConvexHull & c2,
		 const p2d::Vector & v2
)
{
	p2d::Vector v = v1 - v2;
	p2d::SetOfPointsPair result;
	bool xlessthanzero = (v.x() < 0 && non_fuzzy_zero(v.x()));
	bool ylessthanzero = (v.y() < 0 && non_fuzzy_zero(v.y()));
	if (xlessthanzero && ylessthanzero)
	{
		return result;
	}
	for(int i = 0 ; i < c1.sides(); i++)
	{
		for(int j = 0; j < c2.sides(); j++)
		{
			p2d::Cutter2D s1 = c1.side(i);
			p2d::Cutter2D s2 = c2.side(j);
			p2d::MaybePoint tmp;
			tmp = p2d::intersection(s1.p1(), v, s2);
			if (tmp.exists())  p2d::insertUnique(result, s1.p1(), tmp.data()); 
			tmp = p2d::intersection(s1.p2(), v, s2);
			if (tmp.exists())  p2d::insertUnique(result, s1.p2(), tmp.data());
			tmp = p2d::intersection(s2.p1(), v, s1);
			if (tmp.exists())  p2d::insertUnique(result, tmp.data(), s2.p1());
			tmp = p2d::intersection(s2.p2(), v, s1);
			if (tmp.exists())  p2d::insertUnique(result, tmp.data(), s2.p2());
		}
	}
	p2d::filterOptimalSet(result, v);
	return result;
}

p2d::Point p2d::intersectionWithNormalFrom(
	const p2d::Point & p,
	const p2d::Cutter2D & c
)
{
	p2d::InfiniteLine l1 = p2d::InfiniteLine::fromCutter(c);
	p2d::Vector v = p2d::ortho(l1.direction(), p2d::OVI_DEG_90);
	p2d::InfiniteLine l2 = p2d::InfiniteLine::appliedVector(p, v);
	return l1.intersection(l2).data();
}

hst::vector<p2d::Point> p2d::intersection(
	const p2d::InfiniteLine & l,
	const Circle * ci
)
{
	hst::vector<p2d::Point> result;
	double R = ci->radius();
	if (is_fuzzy_zero(l.kx()) && non_fuzzy_zero(l.ky()))
	{
		double y = - l.b() /l.ky();
		double dy = (y - ci->center().y());
		if (dy * dy <= R * R)
		{
			double sqD = sqrt(R * R - dy * dy);
			double x1 = ci->center().x() + sqD;
			double x2 = ci->center().x() - sqD;
			result << p2d::Point(x1, y);
			result << p2d::Point(x2, y);
		}
	}
	if (non_fuzzy_zero(l.kx()) && is_fuzzy_zero(l.ky()))
	{
		double x = - l.b() /l.kx();
		double dx = (x - ci->center().x());
		if (dx * dx <= R * R)
		{
			double sqD = sqrt(R * R - dx * dx);
			double y1 = ci->center().y() + sqD;
			double y2 = ci->center().y() - sqD;
			result << p2d::Point(x, y1);
			result << p2d::Point(x, y2);
		}
	}
	if (non_fuzzy_zero(l.kx()) && non_fuzzy_zero(l.ky()))
	{
		p2d::Point c = ci->center();
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
			result << p2d::Point(x1, y1);
			result << p2d::Point(x2, y2);
		}
	}
	return result;
}

p2d::SetOfPointsPair p2d::findContacts(
	const p2d::Cutter2D & c,
	const p2d::Vector & v,
	const Circle * ci
)
{
	p2d::SetOfPointsPair result;
	if (is_fuzzy_zero(p2d::modulo(v))) 
		return result;
	p2d::Point O = p2d::intersectionWithNormalFrom(ci->center(), c);
	p2d::MaybePoint  K;
	p2d::Point O1;
	if (equal(O, ci->center()) == false)
	{
		p2d::InfiniteLine CO = p2d::InfiniteLine::fromCutter(p2d::Cutter2D(ci->center(), O));
	
		hst::vector<p2d::Point> tmppoints = p2d::intersection(CO, ci);
		p2d::InfiniteLine C1C2 = p2d::InfiniteLine::fromCutter(c);
		hst::vector<p2d::Point> Ks;
		double min = std::numeric_limits<double>::max();
		int mini = 0;
		for(int i = 0; i < tmppoints.size();i ++)
		{
			p2d::InfiniteLine O1V = p2d::InfiniteLine::appliedVector(tmppoints[i], v);
			p2d::MaybePoint F =  C1C2.intersection(O1V);
			if (F.exists()) 
			{
				Ks << F.data(); 
				double sc = p2d::scalar(tmppoints[i] - F.data(), v);
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
		p2d::InfiniteLine line = p2d::InfiniteLine::appliedVector(ci->center(), v); 
		hst::vector<p2d::Point> points = p2d::intersection(line, ci);
		if (points.size() == 2)
		{
			O1 = O;
			double t1 = p2d::scalar(points[0] - O, v);
			double t2 = p2d::scalar(points[1] - O, v);
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
		if (projectionIsWithin(K.data(), c.p1(), c.p2()))
		{
			double dx = O1.x() - ci->center().x();
			double dy = O1.y() - ci->center().y();
			// If O1 belongs to circle
			if (is_fuzzy_equal(dx* dx + dy * dy, ci->radius() * ci->radius()))
			{
				result << p2d::PointsPair(K.data(), O1);	
			} 
			else
			{
				result << p2d::PointsPair(O1, K.data());
			}
		}
		{
			p2d::InfiniteLine l = p2d::InfiniteLine::appliedVector(c.p1(), v);
			hst::vector<p2d::Point> pts = p2d::intersection(l, ci);
			for(int i = 0; i < pts.size(); i++)
			{
				result << p2d::PointsPair(c.p1(), pts[i]);
			}
		}
		{
			p2d::InfiniteLine l = p2d::InfiniteLine::appliedVector(c.p2(), v);
			hst::vector<p2d::Point> pts = p2d::intersection(l, ci);
			for(int i = 0; i < pts.size(); i++)
			{
				result << p2d::PointsPair(c.p2(), pts[i]);
			}
		}
	}
	// If found two points, find one with minimal distance
	p2d::filterOptimalSet(result, v);
	return result;
}


p2d::SetOfPointsPair p2d::FindContactPoints::exec(
		 const p2d::ConvexHull  & c1, 
		 const p2d::Vector & v1,
		 const p2d::Circle * c2,
		 const p2d::Vector & v2
)
{
	p2d::Vector v = v1 - v2;
	p2d::SetOfPointsPair result;
	bool xlessthanzero = (v.x() < 0 && non_fuzzy_zero(v.x()));
	bool ylessthanzero = (v.y() < 0 && non_fuzzy_zero(v.y()));
	if (xlessthanzero && ylessthanzero)
	{
		return result;
	}
	for(int i = 0 ; i < c1.sides(); i++)
	{
		p2d::Cutter2D c1i = c1.side(i);
		p2d::SetOfPointsPair set = p2d::findContacts(c1i, v, c2);
		p2d::merge(result, set);
	}
	p2d::filterOptimalSet(result, v);
	return result;
}


p2d::SetOfPointsPair p2d::FindContactPoints::exec(
		 const p2d::Circle * c1, 
		 const p2d::Vector & v1,
		 const p2d::Circle * c2,
		 const p2d::Vector & v2
)
{
	p2d::SetOfPointsPair result;
	p2d::Vector v = v1 - v2;
	bool xlessthanzero = (v.x() < 0 && non_fuzzy_zero(v.x()));
	bool ylessthanzero = (v.y() < 0 && non_fuzzy_zero(v.y()));
	bool iszero = is_fuzzy_zero(p2d::modulo(v));
	if ((xlessthanzero && ylessthanzero) || iszero)
	{
		return result;
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
		if (DI < 0) return result;
		double sqDI = sqrt(DI);
		t = (-B - sqDI) / 2.0 / A;
	}

	p2d::Point O1 = c1->center() + v * t;
	p2d::InfiniteLine line = p2d::InfiniteLine::fromCutter(p2d::Cutter2D(c2->center(), O1));
	hst::vector<p2d::Point> points = p2d::intersection(line, c2);
	// Select nearest to c1center
	p2d::Point min;
	bool minexists = false;
	for(int i = 0; i < points.size(); i++)
	{
		if (min.distanceTo(O1) > points[i].distanceTo(O1) || !minexists)
		{
			minexists = true;
			min = points[i];
		}
	}
	p2d::Point c2contact = min;
	p2d::Point c1contact = min - v * t;
	result << p2d::PointsPair(c1contact, c2contact);
	return result;
}

p2d::SetOfPointsPair p2d::FindContactPoints::getBtoB(
		 p2d::Bound * s1, 
		 const p2d::Vector & v1,
		 p2d::Bound * s2,
		 const p2d::Vector & v2
	 )
{
	// In this cases, bounds are always colliding, so we should test a bounds for collision
	if (s1->isOrthogonal(s2) || s1->type() == s2->type())
	{
		throw p2d::CannotDetermineContactPoints();
	}

	p2d::SetOfPointsPair result;
	double minv = std::numeric_limits<double>::min();
	double maxv = std::numeric_limits<double>::max();

	if ( (s1->type() == p2d::BT_LEFT && s2->type() == p2d::BT_RIGHT)
		|| (s1->type() == p2d::BT_RIGHT && s2->type() == p2d::BT_LEFT))
	{
		result << p2d::PointsPair(p2d::Point(s1->position(), minv), p2d::Point(s2->position(), minv));	
		result << p2d::PointsPair(p2d::Point(s1->position(), maxv), p2d::Point(s2->position(), maxv));	
	}

	if ( (s1->type() == p2d::BT_UP && s2->type() == p2d::BT_DOWN)
		|| (s1->type() == p2d::BT_UP && s2->type() == p2d::BT_DOWN))
	{
		result << p2d::PointsPair(p2d::Point(minv, s1->position()), p2d::Point(minv, s2->position()));	
		result << p2d::PointsPair(p2d::Point(maxv, s1->position()), p2d::Point(maxv, s2->position()));	
	}
	
	return result;
}


bool p2d::hasPair(const p2d::SetOfPointsPair & set,
		     	  double x1, double y1,
				  double x2, double y2)
{
	p2d::Point p1(x1, y1);
	p2d::Point p2(x2, y2);
	bool exists = false;
	for(size_t i = 0; i < set.size(); i++)
	{
		if (equal(set[i].p1(), p1) && equal(set[i].p2(), p2))
		{
			exists = true;
		}
	}
	return exists;
}



template<typename _Polygon>
p2d::SetOfPointsPair p2d::FindContactPoints::getBtoP(
		 p2d::Bound * s1,
		 const p2d::Vector & v1,
		 _Polygon * s2,
		 const p2d::Vector & v2
)
{
	p2d::Vector v = v2 - v1;

	hst::vector<p2d::Point> p2 = s2->points();
	p2d::SetOfPointsPair result;

	p2d::Vector directionvector = s1->normal() * (-1);
	p2d::InfiniteLine bound = s1->boundingLine();
	
	double vp = p2d::scalar(v, directionvector);
	// If cannot find, result is empty
	if (is_fuzzy_zero(vp))
		return result;
	// If moving in opposite direction, it means it moving away from collisio, so contact
	// points cannot be determined
	if (vp < 0)
		throw p2d::CannotDetermineContactPoints();

	// Find points with minimum scalar productions on vector
	hst::vector<p2d::Point> minpoints;
	double minprojection = std::numeric_limits<double>::max();
	for(size_t i = 0 ; i < p2.size(); i++)
	{
		double projection = p2d::scalar(directionvector, p2[i]);
		if (is_fuzzy_equal(projection, minprojection))
		{
			minpoints << p2[i];
		}
		else
		{
			if (projection < minprojection)
			{
				minpoints.clear();
				minpoints << p2[i];
				minprojection = projection;
			}
		}
	}

	for(size_t i = 0; i < minpoints.size(); i++)
	{
		p2d::InfiniteLine line = p2d::InfiniteLine::appliedVector(minpoints[i], v);
		p2d::MaybePoint intersection = line.intersection(bound);
		result << p2d::PointsPair(intersection.data(), minpoints[i]);
	}
	return result;
}

p2d::SetOfPointsPair p2d::FindContactPoints::getBtoC(
		 p2d::Bound * s1,
		 const p2d::Vector & v1,
		 p2d::Circle * s2,
		 const p2d::Vector & v2	
	 )
{
	p2d::Vector v = v2 - v1;

	p2d::SetOfPointsPair result;

	p2d::Vector directionvector = s1->normal() * (-1);
	p2d::InfiniteLine bound = s1->boundingLine();
	
	double vp = p2d::scalar(v, directionvector);
	// If cannot find, result is empty
	if (is_fuzzy_zero(vp))
		return result;
	// If moving in opposite direction, it means it moving away from collisio, so contact
	// points cannot be determined
	if (vp < 0)
		throw p2d::CannotDetermineContactPoints();

	p2d::Point circlecontactpoint = s2->center() + directionvector * s2->radius();
	p2d::InfiniteLine line = p2d::InfiniteLine::appliedVector(circlecontactpoint, v);
	p2d::MaybePoint intersection = line.intersection(bound);
	result << p2d::PointsPair(intersection.data(), circlecontactpoint);
	return result;
}
