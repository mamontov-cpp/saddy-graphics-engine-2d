#include "p2d/simplecollisiondetector.h"
#include "p2d/broadcollisiondetector.h"
#include "p2d/collisiontest.h"
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::p2d::BroadCollisionDetector, sad::p2d::CollisionDetector);


sad::p2d::BroadCollisionDetector::BroadCollisionDetector(
    sad::p2d::CollisionTest * test
) : m_tester(test)
{	
    m_detector = new p2d::SimpleCollisionDetector();
}

sad::p2d::BroadCollisionDetector::~BroadCollisionDetector()
{
    PROFILER_EVENT;
    delete m_tester;
    delete m_detector;
}


sad::p2d::MaybeTime sad::p2d::BroadCollisionDetector::collides(
    sad::p2d::Body * b1, 
    sad::p2d::Body * b2, 
    double limit
)
{
    PROFILER_EVENT;
    sad::p2d::MaybeTime result;
    
    // If we determining a collision with bounds, we could taks simple approach
    // and don't care about a travelled space, because if body collided with bound one,
    // it will collide further, if force won't change direction inside of one step
    if (b1->currentShape()->metaIndex() == 3)
    {
        sad::p2d::Bound * bound = static_cast<sad::p2d::Bound*>(b1->Temporary);
        if (p2d::CollisionTest::collidesBtoS(bound, b2->Temporary))
            result.setValue(limit);
        return result;
    }


    if (b2->currentShape()->metaIndex() == 3)
    {
        sad::p2d::Bound * bound = static_cast<sad::p2d::Bound*>(b2->Temporary);
        if (p2d::CollisionTest::collidesBtoS(bound, b1->Temporary))
            result.setValue(limit );
        return result;
    }

    // Those are simply heuristic numbers, which is quite useful to reduce
    // load for sake of faster work
    sad::p2d::Axle a(1, 1);
    sad::p2d::Cutter1D cutter1 = b1->currentShape()->project(a);
    sad::p2d::Cutter1D cutter2 = b2->currentShape()->project(a);
    double length1 = cutter1.p2() - cutter1.p1();
    double length2 = cutter2.p2() - cutter2.p1();

    if (limit < 1.0 
        && sad::p2d::modulo(b1->tangentialVelocity()) < length1
        && sad::p2d::modulo(b2->tangentialVelocity()) < length2)
    {
        return m_detector->collides(b1, b2, limit);
    }


    // Build two hulls for determining a collision
    sad::p2d::ConvexHull s1;
    s1.insertPointsFromShape(b1->currentShape());
    s1.insertPointsFromShape( b1->Temporary );
    s1.buildHull();

    sad::p2d::ConvexHull s2;
    s2.insertPointsFromShape(b2->currentShape());
    s2.insertPointsFromShape( b2->Temporary );
    s2.buildHull();

    if(s1.collides(s2))
    {
        result.setValue(limit / 2.0);
    }
    return result;
}

