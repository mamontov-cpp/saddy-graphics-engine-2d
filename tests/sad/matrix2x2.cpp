#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "matrix2x2.h"
#include "extra/fuzzy_equal.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)

typedef sad::Matrix2x2<double> matrixf;
/*!
 * Tests object logic
 */
struct Matrix2x2Test : tpunit::TestFixture
{
 public:
   Matrix2x2Test() : tpunit::TestFixture(
	   TEST(Matrix2x2Test::testGetters),
	   TEST(Matrix2x2Test::testRotate90DegreesCounterclockwise),
	   TEST(Matrix2x2Test::testRotate90DegreesClockwise),
	   TEST(Matrix2x2Test::testRotate270DegreesCounterclockwise),
	   TEST(Matrix2x2Test::testRotate270DegreesClockwise),
	   TEST(Matrix2x2Test::testRotate45DegreesClockwise),
	   TEST(Matrix2x2Test::testRotate45DegreesCounterclockwise)
   ) {}
   
   void testGetters()
   {
		matrixf m(1, 2, 3, 4);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(0,0)], 1.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(0,1)], 2.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(1,0)], 3.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(1,1)], 4.0);
   }

   void testRotate90DegreesCounterclockwise()
   {
	   sad::Point2D p(1,0);
	   matrixf m = matrixf::counterclockwise(0.5 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), 1.0) );
   }

   void testRotate90DegreesClockwise()
   {
	   sad::Point2D p(1,0);
	   matrixf m = matrixf::clockwise(0.5 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), -1.0) );
   }

   void testRotate270DegreesClockwise()
   {
	   sad::Point2D p(-1,0);
	   matrixf m = matrixf::clockwise(1.5 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), -1.0) );
   }

   void testRotate270DegreesCounterclockwise()
   {
	   sad::Point2D p(-1,0);
	   matrixf m = matrixf::counterclockwise(1.5 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), 1.0) );
   }

   void testRotate45DegreesClockwise()
   {
	   sad::Point2D p(M_SQRT1_2, M_SQRT1_2);
	   matrixf m = matrixf::clockwise(0.25 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 1.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), 0.0) );
   }

   void testRotate45DegreesCounterclockwise()
   {
	   sad::Point2D p(M_SQRT1_2, M_SQRT1_2);
	   matrixf m = matrixf::counterclockwise(0.25 * M_PI);
	   sad::Point2D r = p * m;
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  sad::is_fuzzy_equal(r.y(), 1.0) );
   }

} test_matrix_2x2;

