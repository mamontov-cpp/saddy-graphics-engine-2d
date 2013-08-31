#include "p2d/collisiontest.h"
#include "extra/fuzzy_equal.h"

void p2d::CollisionTest::init()
{
	this->p2d::CollisionMultiMethod<bool>::init();
	add(p2d::CollisionTest::collidesRtoR);
	add(p2d::CollisionTest::collidesRtoC);
	add(p2d::CollisionTest::collidesRtoL);
	add(p2d::CollisionTest::collidesCtoC);
	add(p2d::CollisionTest::collidesCtoL);
	add(p2d::CollisionTest::collidesLtoL);

	add(p2d::CollisionTest::collidesBtoR);
	add(p2d::CollisionTest::collidesBtoC);
	add(p2d::CollisionTest::collidesBtoL);
	add(p2d::CollisionTest::collidesBtoB);
}


bool p2d::CollisionTest::collidesRtoR(p2d::Rectangle * p1, p2d::Rectangle * p2)
{
	p2d::ConvexHull c1 = p1->toHull();
	p2d::ConvexHull c2 = p2->toHull();
	return c1.collides(c2);
}

bool p2d::CollisionTest::collidesRtoC(p2d::Rectangle * p1, p2d::Circle * p2)
{
	hst::vector<p2d::Axle> axles;
	axles << p2d::axle(p1->point(0), p1->point(1));
	axles << p2d::axle(p1->point(1), p1->point(2));
	axles << p2d::axle(p1->point(2), p1->point(3));
	axles << p2d::axle(p1->point(3), p1->point(0));
	axles << p2d::axle(p1->point(0), p2->center());
	axles << p2d::axle(p1->point(1), p2->center());
	axles << p2d::axle(p1->point(2), p2->center());
	axles << p2d::axle(p1->point(3), p2->center());
	
	p2d::ConvexHull hull = p1->toHull();
	bool collides = true;
	for(size_t i = 0; i < axles.size() && collides;i++)
	{
		p2d::Cutter1D c1 = hull.project(axles[i]);
		p2d::Cutter1D c2 = p2->project(axles[i]);
		bool axlecollides = p2d::collides(c1, c2);
		collides = collides && axlecollides;
	}
	return collides;
}

bool p2d::CollisionTest::collidesRtoL(p2d::Rectangle * p1, p2d::Line * p2)
{
	p2d::ConvexHull hull1 = p1->toHull();
	p2d::ConvexHull hull2 = p2->toHull();
	return hull1.collides(hull2);
}

bool p2d::CollisionTest::collidesCtoC(p2d::Circle * p1, p2d::Circle * p2)
{
	double dist1 = p1->center().distanceTo(p2->center());
	double dist2 = p2->radius() + p2->radius();
	bool collides = (dist1 < dist2) || is_fuzzy_equal(dist1, dist2);
	return collides;
}

bool p2d::CollisionTest::collidesCtoL(p2d::Circle * p1, p2d::Line * p2)
{
	hst::vector<p2d::Axle> axles;
	axles << p2d::axle(p2->cutter().p1(), p2->cutter().p2());
	axles << p2d::ortho(axles[0], p2d::OVI_DEG_90);
	axles << p2d::axle(p1->center(), p2->cutter().p1());
	axles << p2d::ortho(axles[2], p2d::OVI_DEG_90);
	axles << p2d::axle(p1->center(), p2->cutter().p2());
	axles << p2d::ortho(axles[4], p2d::OVI_DEG_90);
	
	bool collides = true;
	for(size_t i = 0; i < axles.size() && collides;i++)
	{
		p2d::Cutter1D c1 = p1->project(axles[i]);
		p2d::Cutter1D c2 = p2->project(axles[i]);
		bool axlecollides = p2d::collides(c1, c2);
		collides = collides && axlecollides;
	}
	return collides;
}

bool p2d::CollisionTest::collidesLtoL(p2d::Line * p1, p2d::Line * p2)
{
	p2d::ConvexHull hull1 = p1->toHull();
	p2d::ConvexHull hull2 = p2->toHull();
	return hull1.collides(hull2);
}


bool p2d::CollisionTest::collidesBtoB(p2d::Bound * p1, p2d::Bound * p2)
{
	p2d::BoundType bt1 = p1->type();
	p2d::BoundType bt2 = p2->type();
	if (p1->isOrthogonal(p2) || bt1 == bt2)
	{
		return true;
	}
	// Check opposite
	if ((bt1 == BT_LEFT  &&  bt2 == BT_RIGHT)
		|| (bt1 == BT_DOWN  &&  bt2 == BT_UP))
	{
		return p1->position() >= p2->position();	
	}
	if ((bt1 == BT_RIGHT  &&  bt2 == BT_LEFT)
		|| (bt1 == BT_UP  &&  bt2 == BT_DOWN))
	{
		return p1->position() <= p2->position();	
	}
	return false;
}


bool p2d::CollisionTest::collidesBtoR(p2d::Bound * p1, p2d::Rectangle * p2)
{
	return p2d::CollisionTest::collidesBtoS(p1, p2);
}

bool p2d::CollisionTest::collidesBtoC(p2d::Bound * p1, p2d::Circle * p2)
{
	if (p1->type() == BT_LEFT)
	{
		double p = p2->center().x() - p2->radius();
		return p <= p1->position();
	}
	if (p1->type() == BT_RIGHT)
	{
		double p = p2->center().x() + p2->radius();
		return p >= p1->position();
	}
	if (p1->type() == BT_UP)
	{
		double p = p2->center().y() - p2->radius();
		return p >= p1->position();
	}
	if (p1->type() == BT_DOWN)
	{
		double p = p2->center().y() + p2->radius();
		return p <= p1->position();
	}
	return false;
}

bool p2d::CollisionTest::collidesBtoL(p2d::Bound * p1, p2d::Line * p2)
{
	return p2d::CollisionTest::collidesBtoS(p1, p2);
}

bool p2d::CollisionTest::collidesBtoS(p2d::Bound * p1, p2d::CollisionShape * p2)
{
	if (p1->type() == BT_LEFT)
	{
		p2d::Vector v(1, 0);
		p2d::Cutter1D c = p2->project(v);
		return c.p1() <= p1->position();
	}
	if (p1->type() == BT_RIGHT)
	{
		p2d::Vector v(1, 0);
		p2d::Cutter1D c = p2->project(v);
		return c.p2() >= p1->position();
	}
	if (p1->type() == BT_UP)
	{
		p2d::Vector v(0, 1);
		p2d::Cutter1D c = p2->project(v);
		return c.p2() >= p1->position();
	}
	if (p1->type() == BT_DOWN)
	{
		p2d::Vector v(0, 1);
		p2d::Cutter1D c = p2->project(v);
		return c.p2() <= p1->position();
	}
	return false;
}

