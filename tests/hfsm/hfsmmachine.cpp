#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "hfsm/hfsmmachine.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


static int _triggered_count = 0;

void trigger()
{
    ++_triggered_count;
}


struct sadPPrimitive
{
public:

   void trigger()
   {
        ++_triggered_count;
   }
   
   void trigger_const() const
   {
        ++_triggered_count;
   }

} _sad_controls_primitive;


/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadHFSMTest : tpunit::TestFixture
{
 public:
   SadHFSMTest() : tpunit::TestFixture(
       TEST(SadHFSMTest::testCallbacks),
       TEST(SadHFSMTest::testRemove),
       TEST(SadHFSMTest::testHierarchical),
       TEST(SadHFSMTest::testEnterPrevious)
   ) {}


   sadPPrimitive * primitive()
   {
        return &_sad_controls_primitive;
   }

   sadPPrimitive * primitive_const() const
   {
        return &_sad_controls_primitive;
   }


   void trigger()
   {
        ++_triggered_count;
   }

   void trigger_const()
   {
        ++_triggered_count;
   }


   void testCallbacks()
   {
       sad::hfsm::Machine m;
       m.addState("test1");
       m.addState("test2");
    
       m.enterState("test1");

       _triggered_count = 0;

       m.state("test2")->addEnterHandler(::trigger);

       m.state("test2")->addEnterHandler(this, &SadHFSMTest::trigger);
       m.state("test2")->addEnterHandler(this, &SadHFSMTest::trigger_const);
       m.state("test2")->addEnterHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger);
       m.state("test2")->addEnterHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger_const);
       m.state("test2")->addEnterHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger);
       m.state("test2")->addEnterHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger_const);

       m.state("test1")->addLeaveHandler(::trigger);

       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::trigger);
       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::trigger_const);
       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger);
       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger_const);
       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger);
       m.state("test1")->addLeaveHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger_const);

       m.addTransition("test1", "test2", new sad::hfsm::Transition());

       m.transition("test1", "test2")->addHandler(::trigger);

       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::trigger);
       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::trigger_const);
       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger);
       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::primitive,  &sadPPrimitive::trigger_const);
       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger);
       m.transition("test1", "test2")->addHandler(this, &SadHFSMTest::primitive_const,  &sadPPrimitive::trigger_const);

        m.enterState("test2");

       ASSERT_TRUE(_triggered_count == 21);
   }

   void testRemove()
   {
        sad::hfsm::Machine m;
        m.addState("test");
        m.addState("test/1");
        m.addState("test/1/2");

        ASSERT_TRUE(m.state("test/1")->child("2") != nullptr);
        ASSERT_TRUE(m.state("test/1/2") != nullptr);

        m.removeState("test/1");

        ASSERT_TRUE(m.state("test/1/2") == nullptr);
        ASSERT_TRUE(m.state("test/1") == nullptr);
   }

   void testHierarchical()
   {
       sad::hfsm::Machine m;
       m.addState("test/1/2/3", new sad::hfsm::State(), true);
       m.state("test")->addEnterHandler(::trigger);
       m.state("test/1")->addEnterHandler(::trigger);
       m.state("test/1/2")->addEnterHandler(::trigger);
       m.state("test/1/2/3")->addEnterHandler(::trigger);

       _triggered_count = 0;

       m.enterState("test");
       m.enterState("test/1");
       m.enterState("test/1/2");
       m.enterState("test/1/2/3");

       ASSERT_TRUE(_triggered_count == 4);
   }

    void testEnterPrevious()
    {
        sad::hfsm::Machine m;
        m.addState("1", new sad::hfsm::State(), true);
        m.addState("2", new sad::hfsm::State(), true);

        m.enterState("1");
        m.enterState("2");
        m.enterState(m.previousState());
        ASSERT_TRUE(m.isInState("1"));
    }

} _sad_hfsm_machine_test;
