#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/vector.h"
#pragma warning(pop)


/*!
 * Tests vector operations
 */
struct VectorTest : tpunit::TestFixture
{
 public:
   VectorTest() : tpunit::TestFixture(
       TEST(VectorTest::testModulo),
       TEST(VectorTest::testBasis),
       TEST(VectorTest::testUnit),
       TEST(VectorTest::testOrtho90),
       TEST(VectorTest::testOrtho270),
       TEST(VectorTest::testScalar)
   ) {}
   
   void testModulo()
   {
       ASSERT_FLOAT_EQUAL( sad::p2d::modulo( sad::p2d::Vector(0, 0) ), 0);
       ASSERT_FLOAT_EQUAL( sad::p2d::modulo( sad::p2d::Vector(0, 1) ), 1);
       ASSERT_FLOAT_EQUAL( sad::p2d::modulo( sad::p2d::Vector(M_SQRT1_2, M_SQRT1_2) ), 1);
   }

   void testBasis()
   {
       sad::p2d::Vector p = sad::p2d::basis();
       ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);
   }

   void testUnit()
   {
       sad::p2d::Vector p = sad::p2d::unit(sad::p2d::Vector(0,0));
       ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);
       p = sad::p2d::unit(sad::p2d::Vector(0, 2));
       ASSERT_FLOAT_EQUAL( p.x(),  0);
       ASSERT_FLOAT_EQUAL( p.y(),  1);
       p = sad::p2d::unit(sad::p2d::Vector(2, 0));
       ASSERT_FLOAT_EQUAL( p.x(),  1);
       ASSERT_FLOAT_EQUAL( p.y(),  0);
       p = sad::p2d::unit(sad::p2d::Vector(10, 10));
       ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);	   
   }

   void testOrtho90()
   {
       sad::p2d::Vector p;
       
       p = sad::p2d::ortho(sad::p2d::Vector(0,0), sad::p2d::OVI_DEG_90);
       ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  -M_SQRT1_2);

       p = sad::p2d::ortho(sad::p2d::Vector(10,10), sad::p2d::OVI_DEG_90);
       ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  -M_SQRT1_2);

       p = sad::p2d::ortho(sad::p2d::Vector(10,0), sad::p2d::OVI_DEG_90);
       ASSERT_FLOAT_EQUAL( p.x(),  0.0);
       ASSERT_FLOAT_EQUAL( p.y(),  -1.0);

       p = sad::p2d::ortho(sad::p2d::Vector(0,10), sad::p2d::OVI_DEG_90);
       ASSERT_FLOAT_EQUAL( p.x(),  1.0);
       ASSERT_FLOAT_EQUAL( p.y(),  0.0);
   }

   void testOrtho270()
   {
       sad::p2d::Vector p;
       
       p = sad::p2d::ortho(sad::p2d::Vector(0,0), sad::p2d::OVI_DEG_270);
       ASSERT_FLOAT_EQUAL( p.x(),  -M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);

       p = sad::p2d::ortho(sad::p2d::Vector(10,10), sad::p2d::OVI_DEG_270);
       ASSERT_FLOAT_EQUAL( p.x(),  -M_SQRT1_2);
       ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);

       p = sad::p2d::ortho(sad::p2d::Vector(10,0), sad::p2d::OVI_DEG_270);
       ASSERT_FLOAT_EQUAL( p.x(),  0.0);
       ASSERT_FLOAT_EQUAL( p.y(),  1.0);

       p = sad::p2d::ortho(sad::p2d::Vector(0,10), sad::p2d::OVI_DEG_270);
       ASSERT_FLOAT_EQUAL( p.x(),  -1.0);
       ASSERT_FLOAT_EQUAL( p.y(),  0.0);
   }

   void testScalar()
   {
        ASSERT_FLOAT_EQUAL(
            sad::p2d::scalar( sad::p2d::Vector(0,0), sad::p2d::Vector(0,0)), 0
        );
        ASSERT_FLOAT_EQUAL(
            sad::p2d::scalar( sad::p2d::Vector(10,0), sad::p2d::Vector(0,10)), 0
        );
        ASSERT_FLOAT_EQUAL(
            sad::p2d::scalar( sad::p2d::Vector(5,5), sad::p2d::Vector(5,5)), 50
        );
        ASSERT_FLOAT_EQUAL(
            sad::p2d::scalar( sad::p2d::Vector(4,3), sad::p2d::Vector(2,1)), 11
        );
   }

} test3;