#include "p2d/simplecollisiondetector.h"

p2d::MaybeTime p2d::SimpleCollisionDetector::collides(p2d::Body * b1, 
						         					  p2d::Body * b2, 
												      double limit)
{
	double time = m_moment * limit;
	p2d::CollisionShape & s1 = b1->at(time);
	p2d::CollisionShape & s2 = b2->at(time);
	p2d::MaybeTime result;
	if(m_tester->invoke(&s1, &s2))
	{
		result.setValue(time);
	}
	return result;
}

p2d::SimpleCollisionDetector::~SimpleCollisionDetector()
{
	delete m_tester;
}

