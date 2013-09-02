#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
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
	   ASSERT_FLOAT_EQUAL( p2d::modulo( p2d::Vector(0, 0) ), 0);
	   ASSERT_FLOAT_EQUAL( p2d::modulo( p2d::Vector(0, 1) ), 1);
	   ASSERT_FLOAT_EQUAL( p2d::modulo( p2d::Vector(M_SQRT1_2, M_SQRT1_2) ), 1);
   }

   void testBasis()
   {
	   p2d::Vector p = p2d::basis();
	   ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);
   }

   void testUnit()
   {
	   p2d::Vector p = p2d::unit(p2d::Vector(0,0));
	   ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);
	   p = p2d::unit(p2d::Vector(0, 2));
	   ASSERT_FLOAT_EQUAL( p.x(),  0);
	   ASSERT_FLOAT_EQUAL( p.y(),  1);
	   p = p2d::unit(p2d::Vector(2, 0));
	   ASSERT_FLOAT_EQUAL( p.x(),  1);
	   ASSERT_FLOAT_EQUAL( p.y(),  0);
	   p = p2d::unit(p2d::Vector(10, 10));
	   ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);	   
   }

   void testOrtho90()
   {
	   p2d::Vector p;
	   
	   p = p2d::ortho(p2d::Vector(0,0), p2d::OVI_DEG_90);
	   ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  -M_SQRT1_2);

	   p = p2d::ortho(p2d::Vector(10,10), p2d::OVI_DEG_90);
	   ASSERT_FLOAT_EQUAL( p.x(),  M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  -M_SQRT1_2);

	   p = p2d::ortho(p2d::Vector(10,0), p2d::OVI_DEG_90);
	   ASSERT_FLOAT_EQUAL( p.x(),  0.0);
	   ASSERT_FLOAT_EQUAL( p.y(),  -1.0);

	   p = p2d::ortho(p2d::Vector(0,10), p2d::OVI_DEG_90);
	   ASSERT_FLOAT_EQUAL( p.x(),  1.0);
	   ASSERT_FLOAT_EQUAL( p.y(),  0.0);
   }

   void testOrtho270()
   {
	   p2d::Vector p;
	   
	   p = p2d::ortho(p2d::Vector(0,0), p2d::OVI_DEG_270);
	   ASSERT_FLOAT_EQUAL( p.x(),  -M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);

	   p = p2d::ortho(p2d::Vector(10,10), p2d::OVI_DEG_270);
	   ASSERT_FLOAT_EQUAL( p.x(),  -M_SQRT1_2);
	   ASSERT_FLOAT_EQUAL( p.y(),  M_SQRT1_2);

	   p = p2d::ortho(p2d::Vector(10,0), p2d::OVI_DEG_270);
	   ASSERT_FLOAT_EQUAL( p.x(),  0.0);
	   ASSERT_FLOAT_EQUAL( p.y(),  1.0);

	   p = p2d::ortho(p2d::Vector(0,10), p2d::OVI_DEG_270);
	   ASSERT_FLOAT_EQUAL( p.x(),  -1.0);
	   ASSERT_FLOAT_EQUAL( p.y(),  0.0);
   }

   void testScalar()
   {
		ASSERT_FLOAT_EQUAL(
			p2d::scalar( p2d::Vector(0,0), p2d::Vector(0,0)), 0
		);
		ASSERT_FLOAT_EQUAL(
			p2d::scalar( p2d::Vector(10,0), p2d::Vector(0,10)), 0
		);
		ASSERT_FLOAT_EQUAL(
			p2d::scalar( p2d::Vector(5,5), p2d::Vector(5,5)), 50
		);
		ASSERT_FLOAT_EQUAL(
			p2d::scalar( p2d::Vector(4,3), p2d::Vector(2,1)), 11
		);
   }

} test3;