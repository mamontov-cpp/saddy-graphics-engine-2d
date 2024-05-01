#include "p2d/simplecollisiondetector.h"
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::p2d::SimpleCollisionDetector, sad::p2d::CollisionDetector);


sad::p2d::MaybeTime sad::p2d::SimpleCollisionDetector::collides(sad::p2d::Body * b1, 
                                                                sad::p2d::Body * b2, 
                                                                double limit)
{
    PROFILER_EVENT;
    double time = m_moment * limit;
    // A zero contains cache data from  bodies
    sad::p2d::CollisionShape * s1 = b1->Temporary;
    sad::p2d::CollisionShape * s2 = b2->Temporary;
    sad::p2d::MaybeTime result;
    if(m_tester->invoke(s1, s2))
    {
        result.setValue(time);
    }
    return result;
}

sad::p2d::SimpleCollisionDetector::~SimpleCollisionDetector()
{
    PROFILER_EVENT;
    delete m_tester;
}
