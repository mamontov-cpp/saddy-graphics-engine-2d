#include "p2d/simplecollisiondetector.h"
#include "p2d/broadcollisiondetector.h"
#include "p2d/collisiontest.h"


p2d::BroadCollisionDetector::BroadCollisionDetector(p2d::CollisionTest * test) : m_tester(test)
{	
	m_detector = new p2d::SimpleCollisionDetector();
}

p2d::BroadCollisionDetector::~BroadCollisionDetector()
{
	delete m_tester;
	delete m_detector;
}


p2d::MaybeTime p2d::BroadCollisionDetector::collides(p2d::Body * b1, 
													 p2d::Body * b2, 
												     double limit)
{
	p2d::MaybeTime result;
	
	// If we determining a collision with bounds, we could taks simple approach
	// and don't care about a travelled space, because if body collided with bound one,
	// it will collide further, if force won't change direction inside of one step
	if (b1->currentShape()->metaData()->privateIndex() == 3)
	{
		p2d::Bound * bound = static_cast<p2d::Bound*>(b1->Temporary);
		if (p2d::CollisionTest::collidesBtoS(bound, b2->Temporary))
			result.setValue(limit);
		return result;
	}


	if (b2->currentShape()->metaData()->privateIndex() == 3)
	{
		p2d::Bound * bound = static_cast<p2d::Bound*>(b2->Temporary);
		if (p2d::CollisionTest::collidesBtoS(bound, b1->Temporary))
			result.setValue(limit );
		return result;
	}

	// Those are simply heuristic numbers, which is quite useful to reduce
	// load for sake of faster work
	p2d::Axle a(1, 1);
	p2d::Cutter1D cutter1 = b1->currentShape()->project(a);
	p2d::Cutter1D cutter2 = b2->currentShape()->project(a);
	double length1 = cutter1.p2() - cutter1.p1();
	double length2 = cutter2.p2() - cutter2.p1();

	if (limit < 1.0 
		&& p2d::modulo(b1->tangentialVelocity()) < length1
		&& p2d::modulo(b2->tangentialVelocity()) < length2)
	{
		return m_detector->collides(b1, b2, limit);
	}


	// Build two hulls for determining a collision
	p2d::ConvexHull s1;
	s1.insertPointsFromShape(b1->currentShape());
	s1.insertPointsFromShape( b1->Temporary );
	s1.buildHull();

	p2d::ConvexHull s2;
	s2.insertPointsFromShape(b2->currentShape());
	s2.insertPointsFromShape( b2->Temporary );
	s2.buildHull();

	if(s1.collides(s2))
	{
		result.setValue(limit / 2.0);
	}
	return result;
}


