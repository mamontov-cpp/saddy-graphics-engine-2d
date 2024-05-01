#include "p2d/multisamplingcollisiondetector.h"
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::p2d::MultisamplingCollisionDetector, sad::p2d::CollisionDetector);


sad::p2d::MaybeTime sad::p2d::MultisamplingCollisionDetector::collides(sad::p2d::Body * b1, 
                                                                       sad::p2d::Body * b2, 
                                                                       double limit)
{
    PROFILER_EVENT;
    sad::p2d::MaybeTime result;
    if (m_tester->invoke(b1->currentShape(), b2->currentShape())) 
    {
        return static_cast<double>(0);
    }
    for(unsigned int  i = 0; i  < m_tests && !(result.exists()); i++)
    {
        size_t size_of_type_1 = b1->currentShape()->sizeOfType();
        size_t size_of_type_2 = b2->currentShape()->sizeOfType();

        unsigned char* shape_ptr_1 = reinterpret_cast<unsigned char*>(b1->Temporary) + i * size_of_type_1;
        unsigned char* shape_ptr_2 = reinterpret_cast<unsigned char*>(b2->Temporary) + i * size_of_type_2;

        sad::p2d::CollisionShape* s1 = reinterpret_cast<sad::p2d::CollisionShape*>(shape_ptr_1);
        sad::p2d::CollisionShape* s2 = reinterpret_cast<sad::p2d::CollisionShape*>(shape_ptr_2);
        if (m_tester->invoke(s1, s2))
        {
            result.setValue(limit / m_tests * (i+1));
        }
    }
    return result;
}

sad::p2d::MultisamplingCollisionDetector::~MultisamplingCollisionDetector()
{
    PROFILER_EVENT;
    delete m_tester;
}

int sad::p2d::MultisamplingCollisionDetector::sampleCount() const
{
    PROFILER_EVENT;
    return m_tests;
}
