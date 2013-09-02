#include <p2d/convexhull.h>
#include <p2d/collisionshape.h>
#include <extra/geometry2d.h>

p2d::ConvexHull::ConvexHull()
{

}

p2d::ConvexHull::ConvexHull(const hst::vector<p2d::Point> & set)
{
	m_set = p2d::graham_scan(set);
}

p2d::ConvexHull p2d::ConvexHull::uncheckedCreate(const hst::vector<p2d::Point> & set)
{
	p2d::ConvexHull  hull;
	hull.m_set = set;
	return hull;
}

p2d::ConvexHull p2d::ConvexHull::getUnion(const p2d::ConvexHull & o1, const p2d::ConvexHull & o2)
{
	hst::vector<p2d::Point> set = o1.m_set;
	set << o2.m_set;
	return p2d::ConvexHull(set);
}

size_t p2d::ConvexHull::sides() const
{
	if (m_set.size() < 2) return 0; 
	return m_set.size();
}

size_t p2d::ConvexHull::points() const
{
	return m_set.size();
}

p2d::Cutter2D p2d::ConvexHull::side(int number) const
{
	assert(number > -1 && number < sides() );
	if (number == points() - 1)
	{
		return p2d::Cutter2D(m_set[number], m_set[0]);
	}
	return p2d::Cutter2D(m_set[number], m_set[number + 1]);
}

p2d::Vector  p2d::ConvexHull::normal(int number) const
{
	assert(number > -1 && number < sides() );
	p2d::Cutter2D c = side(number);
	p2d::Point center = (c.p1() + c.p2()) / 2.0;
	p2d::Vector sidevector = c.p2() - c.p1();
	p2d::Vector result = p2d::ortho(sidevector, p2d::OVI_DEG_90);	
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
			p2d::Point point_to_center = m_set[point] - center;
			double projection = p2d::scalar(point_to_center, result);
			if (projection > 0)
			{
				notanormal = true;
			}
		}
	}
	if (notanormal)
		result = p2d::ortho( sidevector, OVI_DEG_270);
	return result;
}

p2d::Cutter1D p2d::ConvexHull::project(const p2d::Axle & axle) const
{
	return p2d::projectPointSet(m_set, m_set.size(), axle);
}

void p2d::ConvexHull::tryInsertAxle(hst::vector<p2d::Axle> & container, 
						            const p2d::Axle & axle) const
{
	bool found = false;
	for(size_t i = 0 ; (i < container.size()) && !found; i++)
	{
		if (equal(container[i], axle))
		{
			found = true;
		}
	}
	if (!found)
		container << axle;
}

void p2d::ConvexHull::appendAxisForSide(
										hst::vector<p2d::Axle> & container, 
										int number) const
{
	p2d::Cutter2D k = side(number);
	this->tryInsertAxle(container, p2d::axle(k.p1(), k.p2()));
	p2d::Axle ortho =  p2d::ortho(k.p2() - k.p1(), OVI_DEG_90);
	this->tryInsertAxle(container, ortho);
}

void p2d::ConvexHull::appendAxisForCollision(hst::vector<p2d::Axle> & container) const
{
	for(size_t i = 0; i < this->sides(); i++)
	{
		this->appendAxisForSide(container, i);
	}
}
bool p2d::ConvexHull::collides(const ConvexHull & c) const
{
	if (this->points() == 0 || c.points() == 0)
	{
		return false;
	}
	if (this->points() == 1 && c.points() == 1)
	{
		return equal(this->m_set[0], c.m_set[0]);
	}
	hst::vector<p2d::Axle> axles;
	this->appendAxisForCollision(axles);
	c.appendAxisForCollision(axles);

	bool collides = true;
	for(size_t i = 0 ; i < axles.size() && collides; i++)
	{
		p2d::Cutter1D myproject = this->project(axles[i]);
		p2d::Cutter1D cproject = c.project(axles[i]);
		bool axlecollides = p2d::collides(myproject, cproject);
		collides = collides && axlecollides;
	}

	return collides;
}


p2d::Vector p2d::ConvexHull::getSumOfNormalsFor(const p2d::Point & p) const
{
	p2d::Vector result(0,0);
	double nearest = 0;
	bool nearest_is_found = false;
	for(size_t i = 0; i < sides(); i++)
	{
		p2d::Cutter2D k = side(i);
		p2d::Point center = (k.p1() + k.p2()) / 2.0;
		double d = p.distanceTo(center);
		if (is_fuzzy_equal(d, nearest))
		{
			nearest_is_found = true;
			result += normal(i);
		} 
		else
		{
			if (d < nearest || !nearest_is_found)
			{
				nearest_is_found = true;
				nearest = d;
				result = normal(i);
			}
		}
	}
	if (non_fuzzy_zero( p2d::modulo(result) ))
	{
		result = p2d::unit(result);
	}
	return result;
}


p2d::Point p2d::ConvexHull::center() const
{
	p2d::Point result;
	for(size_t i = 0;  i < points(); i++)
	{
		result += m_set[i];
	}
	result /= points();
	return result;
}


void p2d::ConvexHull::buildHull()
{
	m_set = p2d::graham_scan(m_set);
}

void p2d::ConvexHull::insertPointsFromShape(p2d::CollisionShape * s)
{
	s->populatePoints(m_set);
}


