#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "sadptrhash.h"
#pragma warning(pop)


class IncrementsInDestructor
{
 private:
	 int * m_counter;
 public:
	 IncrementsInDestructor(int * c) { m_counter = c;}
	 ~IncrementsInDestructor() {  *m_counter += 1; }
};
/*!
 * Tests object logic
 */
struct PtrHashTest : tpunit::TestFixture
{
 public:
   PtrHashTest() : tpunit::TestFixture(
	   TEST(PtrHashTest::testDestructor)	   
   ) {}
   
   void testDestructor()
   {
		int counter = 0;
		int count = 10;
		{
			sad::PtrHash<int, IncrementsInDestructor> tested;
			for(int i = 0; i < count; i++)
			{
				tested.insert(i, new IncrementsInDestructor(&counter));
			}
		}
		ASSERT_EQUAL(counter, count);
   }

} test_ptr_hash;
