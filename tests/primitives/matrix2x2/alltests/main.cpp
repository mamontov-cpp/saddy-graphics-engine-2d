#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "primitives/matrix2x2.h"
#include "extra/fuzzy_equal.h"
#pragma warning(pop)

typedef hst::matrix2x2<double> matrixf;
/*!
 * Tests object logic
 */
struct Matrix2x2Test : tpunit::TestFixture
{
 public:
   Matrix2x2Test() : tpunit::TestFixture(
	   TEST(Matrix2x2Test::test_getters),
	   TEST(Matrix2x2Test::test_rotate_90_degrees_counterclockwise),
	   TEST(Matrix2x2Test::test_rotate_90_degrees_clockwise),
	   TEST(Matrix2x2Test::test_rotate_270_degrees_counterclockwise),
	   TEST(Matrix2x2Test::test_rotate_270_degrees_clockwise),
	   TEST(Matrix2x2Test::test_rotate_45_degrees_clockwise),
	   TEST(Matrix2x2Test::test_rotate_45_degrees_counterclockwise)
   ) {}
   
   void test_getters()
   {
		matrixf m(1, 2, 3, 4);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(0,0)], 1.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(0,1)], 2.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(1,0)], 3.0);
		ASSERT_FLOAT_EQUAL(m[matrixf::index(1,1)], 4.0);
   }

   void test_rotate_90_degrees_counterclockwise()
   {
	   hPointF p(1,0);
	   matrixf m = matrixf::counterclockwise(0.5 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), 1.0) );
   }

   void test_rotate_90_degrees_clockwise()
   {
	   hPointF p(1,0);
	   matrixf m = matrixf::clockwise(0.5 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), -1.0) );
   }

   void test_rotate_270_degrees_clockwise()
   {
	   hPointF p(-1,0);
	   matrixf m = matrixf::clockwise(1.5 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), -1.0) );
   }

   void test_rotate_270_degrees_counterclockwise()
   {
	   hPointF p(-1,0);
	   matrixf m = matrixf::counterclockwise(1.5 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), 1.0) );
   }

   void test_rotate_45_degrees_clockwise()
   {
	   hPointF p(M_SQRT1_2, M_SQRT1_2);
	   matrixf m = matrixf::clockwise(0.25 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 1.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), 0.0) );
   }

   void test_rotate_45_degrees_counterclockwise()
   {
	   hPointF p(M_SQRT1_2, M_SQRT1_2);
	   matrixf m = matrixf::counterclockwise(0.25 * M_PI);
	   hPointF r = p * m;
	   ASSERT_TRUE(  is_fuzzy_equal(r.x(), 0.0) );
	   ASSERT_TRUE(  is_fuzzy_equal(r.y(), 1.0) );
   }

} test1;

int main(int argc, char ** argv)
{
   /**
    * Run all of the registered tpunit++ tests. Returns 0 if
    * all tests are successful, otherwise returns the number
    * of failing assertions.
    */
   int result = tpunit::Tests::Run();
   return result;
}