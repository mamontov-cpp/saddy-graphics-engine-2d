#include "p2d/collisiontest.h"
#include "p2d/collides1d.h"
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

// A function, which performs fast collision operations
static inline bool fast_collide(
								p2d::CollisionShape * s1, p2d::CollisionShape * s2,
								void (**callbacks)(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &),
								int  size
								)
{
	p2d::Axle axle;
	bool collides = true;
	for(size_t i = 0; i < 4 && collides;i++)
	{
		callbacks[i](s1, s2, axle);
		p2d::Cutter1D c1 = s1->project(axle);
		p2d::Cutter1D c2 = s2->project(axle);
		bool axlecollides = p2d::collides(c1, c2);
		collides = collides && axlecollides;
	}
	return collides;
}


// Defines compile-time generators for callbacks, for lazy axle-computation
template<typename _Class1, typename _Class2>
struct AxleFor
{
	template< const p2d::Point & (_Class1::*p1)(int) const,
			  int num1,
			  int num2
			>
	static void p1callsi(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(num1), (c1->*p1)(num2) );
	}

	template< const hPointF & (_Class1::*p1)(int) const,
			  int num1,
			  int num2
			>
	static void hp1callsi(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(num1), (c1->*p1)(num2) );
	}

	template< const p2d::Point & (_Class2::*p1)(int) const,
			  int num1,
			  int num2
			>
	static void p2callsi(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c2->*p1)(num1), (c2->*p1)(num2) );
	}

	template< const hPointF & (_Class2::*p1)(int) const,
			  int num1,
			  int num2
			>
	static void hp2callsi(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c2->*p1)(num1), (c2->*p1)(num2) );
	}

	template< const p2d::Point & (_Class1::*p1)(int) const,
			  int num1,
			  const p2d::Point & (_Class2::*p2)() const
			>
	static void p1ip2(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(num1), (c2->*p2)() );
	}

	template< const hPointF & (_Class1::*p1)(int) const,
			  int num1,
			  const p2d::Point & (_Class2::*p2)() const
			>
	static void hp1ip2(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(num1), (c2->*p2)() );
	}

	template<
			  const hPointF & (_Class2::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void p2calls(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c2->*p1)(), (c2->*p2)() );
	}

	template<
			  const hPointF & (_Class2::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void op2calls(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::ortho(p2d::axle( (c2->*p1)(), (c2->*p2)() ), p2d::OVI_DEG_90);
	}


	template<
			  const hPointF & (_Class2::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void p1calls(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(), (c1->*p2)() );
	}

	template<
			  const hPointF & (_Class2::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void op1calls(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::ortho(p2d::axle( (c1->*p1)(), (c1->*p2)() ), p2d::OVI_DEG_90);
	}

	template<
		      const p2d::Point & (_Class1::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void p1hp2(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::axle( (c1->*p1)(), (c2->*p2)() );
	}

	template<
		      const p2d::Point & (_Class1::*p1)() const,
			  const hPointF & (_Class2::*p2)() const			  
			>
	static void op1hp2(p2d::CollisionShape * s1, p2d::CollisionShape * s2, p2d::Axle & axle)
	{
		_Class1 * c1 = static_cast<_Class1 *>(s1);
		_Class2 * c2 = static_cast<_Class2 *>(s2);
		axle = p2d::ortho(p2d::axle( (c1->*p1)(), (c2->*p2)() ), p2d::OVI_DEG_90);
	}

};

static void (*r_to_r_callbacks[4])(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &) =
{
	AxleFor<p2d::Rectangle, p2d::Rectangle>::hp1callsi<&p2d::Rectangle::point, 0, 1>,
	AxleFor<p2d::Rectangle, p2d::Rectangle>::hp1callsi<&p2d::Rectangle::point, 1, 2>,
	AxleFor<p2d::Rectangle, p2d::Rectangle>::hp2callsi<&p2d::Rectangle::point, 0, 1>,
	AxleFor<p2d::Rectangle, p2d::Rectangle>::hp2callsi<&p2d::Rectangle::point, 1, 2>
};


bool p2d::CollisionTest::collidesRtoR(p2d::Rectangle * p1, p2d::Rectangle * p2)
{
	return fast_collide(p1, p2, r_to_r_callbacks, 4);
}

static void (*r_to_c_callbacks[6])(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &) =
{
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1callsi<&p2d::Rectangle::point, 0, 1>,
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1callsi<&p2d::Rectangle::point, 1, 2>,
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1ip2<&p2d::Rectangle::point, 0, &p2d::Circle::centerRef>,
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1ip2<&p2d::Rectangle::point, 1, &p2d::Circle::centerRef>,
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1ip2<&p2d::Rectangle::point, 2, &p2d::Circle::centerRef>,
	AxleFor<p2d::Rectangle, p2d::Circle>::hp1ip2<&p2d::Rectangle::point, 3, &p2d::Circle::centerRef>
};

bool p2d::CollisionTest::collidesRtoC(p2d::Rectangle * p1, p2d::Circle * p2)
{
	return fast_collide(p1, p2, r_to_c_callbacks, 6);
}

static void (*r_to_l_callbacks[4])(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &) =
{
	AxleFor<p2d::Rectangle, p2d::Line>::hp1callsi<&p2d::Rectangle::point, 0, 1>,
	AxleFor<p2d::Rectangle, p2d::Line>::hp1callsi<&p2d::Rectangle::point, 1, 2>,
	AxleFor<p2d::Rectangle, p2d::Line>::p2calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Rectangle, p2d::Line>::op2calls<&p2d::Line::p1, &p2d::Line::p2>
};

bool p2d::CollisionTest::collidesRtoL(p2d::Rectangle * p1, p2d::Line * p2)
{
	return fast_collide(p1, p2, r_to_l_callbacks, 4);
}

bool p2d::CollisionTest::collidesCtoC(p2d::Circle * p1, p2d::Circle * p2)
{
	// A small optimization, since distance runs slow
	double dist2 = p1->radius() + p2->radius();
	if ( fabs(p1->centerRef().x() - p2->centerRef().x()) > dist2)
	{
		return false;
	}
	if (fabs(p1->centerRef().y() - p2->centerRef().y()) > dist2)
	{
		return false;
	}
	double dist1 = p2d::distance(p1->centerRef(), p2->centerRef());
	// The precision was found empirical, because sometimes object collides in
	// very strange phase
	bool collides = (dist1 < dist2) || sad::is_fuzzy_equal(dist1, dist2, 1.0E-6);
	return collides;
}

static void (*c_to_l_callbacks[6])(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &) =
{
	AxleFor<p2d::Circle, p2d::Line>::p2calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Circle, p2d::Line>::op2calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Circle, p2d::Line>::p1hp2<&p2d::Circle::centerRef, &p2d::Line::p1>,
	AxleFor<p2d::Circle, p2d::Line>::p1hp2<&p2d::Circle::centerRef, &p2d::Line::p2>,
	AxleFor<p2d::Circle, p2d::Line>::op1hp2<&p2d::Circle::centerRef, &p2d::Line::p1>,
	AxleFor<p2d::Circle, p2d::Line>::op1hp2<&p2d::Circle::centerRef, &p2d::Line::p2>,
};

bool p2d::CollisionTest::collidesCtoL(p2d::Circle * p1, p2d::Line * p2)
{
	return fast_collide(p1, p2, c_to_l_callbacks, 6);
}


static void (*l_to_l_callbacks[4])(p2d::CollisionShape *, p2d::CollisionShape *, p2d::Axle &) =
{
	AxleFor<p2d::Line, p2d::Line>::p2calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Line, p2d::Line>::op2calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Line, p2d::Line>::p1calls<&p2d::Line::p1, &p2d::Line::p2>,
	AxleFor<p2d::Line, p2d::Line>::op1calls<&p2d::Line::p1, &p2d::Line::p2>
};
bool p2d::CollisionTest::collidesLtoL(p2d::Line * p1, p2d::Line * p2)
{
	return fast_collide(p1, p2, l_to_l_callbacks, 4);
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
		double p = p2->center().y() + p2->radius();
		return p >= p1->position();
	}
	if (p1->type() == BT_DOWN)
	{
		double p = p2->center().y() - p2->radius();
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
		return c.p1() <= p1->position();
	}
	return false;
}

