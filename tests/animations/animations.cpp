#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <label.h>
#include <animations/animationsinstance.h>
#include <animations/animationsanimations.h>
#include <animations/animationssequential.h>
#include <db/dbtable.h>
#include <db/dbdatabase.h>
#pragma warning(pop)

int instances_count = 0;
/*! A marked sequential object test
 */
class MarkedSequential: public sad::animations::Sequential
{
public:
	MarkedSequential() { instances_count += 1; }
	~MarkedSequential() { instances_count -= 1; } 
};



struct AnimationsTest : tpunit::TestFixture
{
 public:
   AnimationsTest() : tpunit::TestFixture(
       TEST(AnimationsTest::testQueryInstancesByObject),
	   TEST(AnimationsTest::testSimpleSequentialGraph),
	   TEST(AnimationsTest::testSimpleCircularGraph),
	   TEST(AnimationsTest::testAddRemove),
	   TEST(AnimationsTest::testMultipleEdges),
	   TEST(AnimationsTest::testComplex),
	   TEST(AnimationsTest::testDatabase)
   ) {}
   

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
   void testQueryInstancesByObject()
   {
        sad::Label a;
        sad::animations::Instance i1;
        sad::animations::Instance i2;
        a.addRef();
        i1.addRef();
        i2.addRef();
        i1.setObject(&a);
        i2.setObject(&a);

        sad::animations::Animations anims;
        anims.add(&i1);
        anims.add(&i2);
        sad::Vector<sad::animations::Process*> processes = anims.queryProcessesRelatedToObject(&a);
        ASSERT_TRUE( processes.size() == 2);

        processes = anims.queryProcessesRelatedToObject(NULL);
        ASSERT_TRUE( processes.size() == 0);
   }
   
   void testSimpleSequentialGraph()
   {
	   instances_count = 0;
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   mr1->addRef();
	   mr1->add(mr2);
	   ASSERT_TRUE( instances_count == 2);
	   
	   mr1->delRef();
	   ASSERT_TRUE( instances_count == 0);	   
   }
   
   void testSimpleCircularGraph()
   {
	   instances_count = 0;
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   mr1->addRef();
	   mr1->add(mr2);
	   mr2->add(mr1);
	   ASSERT_TRUE( instances_count == 2);
	   
	   mr1->delRef();
	   ASSERT_TRUE( instances_count == 0);
   }
   
   void testAddRemove()
   {
	   instances_count = 0;
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   mr1->addRef();
	   mr2->addRef();
	   mr1->add(mr2);
	   mr2->add(mr1);
	   mr1->remove(0);
	   mr2->remove(0);
	   ASSERT_TRUE( instances_count == 2);
	   
	   mr1->delRef();
	   mr2->delRef();
	   ASSERT_TRUE( instances_count == 0);	   
   }
   
   void testMultipleEdges()
   {
	   instances_count = 0;
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   mr1->addRef();
	   mr1->add(mr2);
	   mr1->add(mr2);
	   mr2->add(mr1);
	   ASSERT_TRUE( instances_count == 2);
	   
	   mr1->delRef();
	   ASSERT_TRUE( instances_count == 0);	   
   }
   
   void testComplex()
   {
	   instances_count = 0;
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   MarkedSequential* mr3 = new MarkedSequential();
	   MarkedSequential* mr4 = new MarkedSequential();	   
	   mr1->addRef();
	   mr1->add(mr2);
	   mr1->add(mr3);
	   mr2->add(mr4);
	   mr3->add(mr4);
	   mr4->add(mr1);
	   ASSERT_TRUE( instances_count == 4);
	   
	   mr1->delRef();
	   ASSERT_TRUE( instances_count == 0);	
   }
   
   void testDatabase()
   {
	   instances_count = 0;
	   sad::db::Database* db = new sad::db::Database();
	   db->addRef();
	   MarkedSequential* mr1 = new MarkedSequential();
	   MarkedSequential* mr2 = new MarkedSequential();
	   sad::db::Table* table = new sad::db::Table();
	   db->addTable("animations", table);
	   table->add(mr1);
	   table->add(mr2);
	   mr1->setTable(table);
	   mr2->setTable(table);
	   ASSERT_TRUE( mr1->MajorId != 0);	
	   ASSERT_TRUE( mr2->MajorId != 0);		   
	   mr1->add(mr2->MajorId);
	   mr2->add(mr1->MajorId);
	   mr1->animation(0);
	   mr2->animation(0);	   
	   ASSERT_TRUE( instances_count == 2);	

	   db->delRef();
	   
	   ASSERT_TRUE( instances_count == 0);	
   }

} _animations_test;

