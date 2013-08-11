/*! \file collisionevent.h
	\author HiddenSeeker

	Describes a collision event
 */
#pragma once

namespace p2d
{

class Body;

struct BasicCollisionEvent
{
	p2d::Body * m_object_1;
	p2d::Body * m_object_2;
	double m_time;

	inline BasicCollisionEvent(p2d::Body * o1, p2d::Body * o2, double time)
	{
		m_object_1 = o1;
		m_object_2 = o2;
		m_time = time;
	}

	virtual ~BasicCollisionEvent();
};


template<
typename T1,
typename T2
>
struct CollisionEvent: public BasicCollisionEvent
{
   T1 * m_object_1;
   T2 * m_object_2;
   double m_time;

   T1 & object1() { return *m_object_1; }
   T2 & object2() { return *m_object_2; }
   double time() const { return m_time; }
};

}
