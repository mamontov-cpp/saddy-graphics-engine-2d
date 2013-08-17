#include "p2d/multisamplingcollisiondetector.h"

p2d::MaybeTime p2d::MultisamplingCollisionDetector::collides(p2d::Body * b1, 
						         					         p2d::Body * b2, 
												             double limit)
{
	double slice = limit / m_tests;
	p2d::MaybeTime result;
	for(int  i = 1; i  <= m_tests && !(result.exists()); i++)
	{
		double time = slice * i ;
		p2d::CollisionShape & s1 = b1->at(time);
		p2d::CollisionShape & s2 = b2->at(time);
		if (m_tester->invoke(&s1, &s2))
		{
			result.setValue(time);
		}
	}
	return result;
}

p2d::MultisamplingCollisionDetector::~MultisamplingCollisionDetector()
{
	delete m_tester;
}

