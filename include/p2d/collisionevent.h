/*! \file collisionevent.h
	\author HiddenSeeker

	Describes a collision event
 */
#pragma once

namespace p2d
{

struct BasicCollisionEvent
{
	virtual ~BasicCollisionEvent();
};


template<
typename T1,
typename T2
>
struct CollisionEvent: public BasicCollisionEvent
{
   T1 m_object_1;
   T2 m_object_2;
   double m_time;

   T1 & object1() { return m_object_1; }
   T2 & object2() { return m_object_2; }
   double time() const { return m_time; }
};

}
