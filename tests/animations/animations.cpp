#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <label.h>
#include <sprite2d.h>
#include <animations/animationscolor.h>
#include <animations/animationsinstance.h>
#include <animations/animationsanimations.h>
#include <animations/animationssequential.h>
#include <db/dbtable.h>
#include <db/dbdatabase.h>
#include <sadsleep.h>
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
       TEST(AnimationsTest::testDatabase),
       TEST(AnimationsTest::testObjectStateAfterFinishedWithDelay)
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

        processes = anims.queryProcessesRelatedToObject(nullptr);
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
       try {
            mr1->addRef();
            mr1->add(mr2);
            mr2->add(mr1);
            ASSERT_TRUE( instances_count == 2);

            mr1->delRef();
            ASSERT_TRUE( instances_count == 0);
       } catch(std::logic_error e) {
           ASSERT_TRUE( true );
       }
   }
   
   void testAddRemove()
   {
       instances_count = 0;
       MarkedSequential* mr1 = new MarkedSequential();
       MarkedSequential* mr2 = new MarkedSequential();
       try {
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
       } catch(std::logic_error e) {
            ASSERT_TRUE( true );
       }
   }
   
   void testMultipleEdges()
   {
        try {
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
        } catch(std::logic_error e) {
            ASSERT_TRUE( true );
        }
   }
   
   void testComplex()
   {
        try {
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
        } catch(std::logic_error e) {
            ASSERT_TRUE( true );
        }
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

   void testObjectStateAfterFinishedWithDelay()
   {
       sad::Sprite2D* s = new sad::Sprite2D();
       s->addRef();
       s->setColor(sad::AColor(255, 255, 255, 0));

       sad::animations::Color* animation_color = new sad::animations::Color();
       animation_color->addRef();
       animation_color->setTime(500);
       animation_color->setLooped(false);
       animation_color->setMinColor(sad::AColor(128, 0, 0, 128));
       animation_color->setMaxColor(sad::AColor(128, 0, 0, 255));

       sad::animations::Instance* instance = new sad::animations::Instance();
       instance->addRef();
       instance->setAnimation(animation_color);
       instance->setObject(s);
       instance->disableStateRestoringOnFinish();
       instance->clearFinished();

       sad::animations::Animations* anims = new sad::animations::Animations();
       anims->add(instance);
       anims->process();
       ASSERT_TRUE(s->color().a() != 0)

       sad::sleep(200);

       anims->process();
       ASSERT_TRUE(s->color().a() > 128)
       ASSERT_TRUE(s->color().a() < 255)

       sad::sleep(600);

       anims->process();
       ASSERT_TRUE(s->color().a() == 255)

       delete anims;
       instance->delRef();
       animation_color->delRef();
       s->delRef();
   }

} _animations_test;

