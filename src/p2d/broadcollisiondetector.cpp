#include "p2d/broadcollisiondetector.h"

p2d::MaybeTime p2d::BroadCollisionDetector::collides(p2d::Body * b1, 
													 p2d::Body * b2, 
												     double limit)
{
	p2d::ConvexHull  s11 = b1->at(0).toHull();
	p2d::ConvexHull  s12 = b1->at(limit).toHull();
	
	p2d::ConvexHull  s21 = b1->at(0).toHull();
	p2d::ConvexHull  s22 = b1->at(limit).toHull();
	
	p2d::ConvexHull s1  = p2d::ConvexHull::getUnion(s11, s12);
	p2d::ConvexHull s2  = p2d::ConvexHull::getUnion(s21, s22);

	p2d::MaybeTime result;
	if(s1.collides(s2))
	{
		result.setValue(limit / 2.0);
	}
	return result;
}


