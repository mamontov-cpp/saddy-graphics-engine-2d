#include "p2d/multisamplingcollisiondetector.h"

p2d::MaybeTime p2d::MultisamplingCollisionDetector::collides(p2d::Body * b1, 
						         					         p2d::Body * b2, 
												             double limit)
{
	p2d::MaybeTime result;
	if (m_tester->invoke(b1->currentShape(), b2->currentShape())) 
	{
		return 0;
	}
	for(unsigned int  i = 0; i  < m_tests && !(result.exists()); i++)
	{
		p2d::CollisionShape * s1 = b1->Temporary + i;
		p2d::CollisionShape * s2 = b2->Temporary + i;
		if (m_tester->invoke(s1, s2))
		{
			result.setValue(limit / m_tests * (i+1));
		}
	}
	return result;
}

p2d::MultisamplingCollisionDetector::~MultisamplingCollisionDetector()
{
	delete m_tester;
}

int p2d::MultisamplingCollisionDetector::sampleCount() const
{
	return m_tests;
}

