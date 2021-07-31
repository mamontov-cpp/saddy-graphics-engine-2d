#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "input/controls.h"
#include "keymouseconditions.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


static int _triggered_count = 0;

void trigger_no_arg()
{
    ++_triggered_count;
}

void trigger_arg(const sad::input::QuitEvent & q)
{
    ++_triggered_count;
}


struct sadControlsTestPrimitive
{
public:
    void trigger_no_arg()
   {
        ++_triggered_count;
   }

   void trigger_arg(const sad::input::QuitEvent & q)
   {
        ++_triggered_count;
   }
   
   void trigger_no_arg_const() const
   {
        ++_triggered_count;
   }

   void trigger_arg_const(const sad::input::QuitEvent & q) const
   {
        ++_triggered_count;
   }

} _sad_controls_primitive;


/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadControlsTest : tpunit::TestFixture
{
 public:
   SadControlsTest() : tpunit::TestFixture(
       TEST(SadControlsTest::testCallbacks),
       TEST(SadControlsTest::testWorkflow),
       TEST(SadControlsTest::testKeyChangeForKeyHoldCondition),
       TEST(SadControlsTest::testSpecialKeyChangeForSpecialKeyHoldPosition),
       TEST(SadControlsTest::testButtonChangeForMouseButtonCondition)
   ) {}


   sadControlsTestPrimitive * primitive()
   {
        return &_sad_controls_primitive;
   }

   sadControlsTestPrimitive * primitive_const() const
   {
        return &_sad_controls_primitive;
   }


   void trigger_no_arg()
   {
        ++_triggered_count;
   }

   void trigger_arg(const sad::input::QuitEvent & q)
   {
        ++_triggered_count;
   }
   
   void trigger_no_arg_const() const
   {
        ++_triggered_count;
   }

   void trigger_arg_const(const sad::input::QuitEvent & q) const
   {
        ++_triggered_count;
   }

   void testCallbacks()
   {
       sad::input::Controls c;
     
       c.add(*sad::input::EventType::ET_Quit, ::trigger_no_arg);
       c.add(*sad::input::EventType::ET_Quit, ::trigger_arg);
       c.add(::trigger_arg);
      
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::trigger_no_arg);
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::trigger_arg);
       c.add(this, &SadControlsTest::trigger_arg);

       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::trigger_no_arg_const);
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::trigger_arg_const);
       c.add(this, &SadControlsTest::trigger_arg_const);

       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg),
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_no_arg),
       c.add(this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg),

       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg),
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_no_arg),
       c.add(this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg),

       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg_const),
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_no_arg_const),
       c.add(this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg_const),

       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg_const),
       c.add(*sad::input::EventType::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_no_arg_const),
       c.add(this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg_const),


       _triggered_count = 0;

       c.startReceivingEvents();
       c.postEvent(sad::input::EventType::ET_Quit, sad::input::QuitEvent());
       c.finishReceivingEvents();

       ASSERT_TRUE(_triggered_count == 21);
   }

   void testWorkflow()
   {
        sad::input::Controls c;
        _triggered_count = 0;
        c.startReceivingEvents();
        sad::input::AbstractHandler* h = new sad::input::FreeFunctionHandler<sad::input::QuitEvent>(::trigger_arg);
        c.add(*sad::input::EventType::ET_Quit, h);
        // Handler will not be added
        c.postEvent(sad::input::EventType::ET_Quit, sad::input::QuitEvent());
        ASSERT_TRUE(_triggered_count == 0);
        // Here handler will be added
        c.finishReceivingEvents();
        
        c.startReceivingEvents();
        // Here handler will not be removed
        c.remove(h);
        c.postEvent(sad::input::EventType::ET_Quit, sad::input::QuitEvent());
        ASSERT_TRUE(_triggered_count == 1);
        // Here handler will be removed
        c.finishReceivingEvents();

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_Quit, sad::input::QuitEvent());
        c.finishReceivingEvents();
        ASSERT_TRUE(_triggered_count == 1);
   }

   void testKeyChangeForKeyHoldCondition()
   {
       sad::input::Controls c;
        _triggered_count = 0;
        c.startReceivingEvents();
        
        sad::input::AbstractHandler* h = new sad::input::FreeFunctionHandler<sad::input::QuitEvent>(::trigger_arg);
        sad::input::HandlerTypeAndConditions conditions;
        sad::KeyHoldCondition* condition = new sad::KeyHoldCondition(sad::KeyboardKey::Z);
        c.startReceivingEvents();
        conditions.set1(sad::input::EventType::ET_KeyPress);
        conditions._2() << condition;
        c.add(conditions, h);
        c.finishReceivingEvents();

        sad::input::KeyPressEvent ev;
        ev.Key = sad::KeyboardKey::Z;
        sad::input::KeyPressEvent ev2;
        ev2.Key = sad::KeyboardKey::C;

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        c.finishReceivingEvents();

        ASSERT_TRUE( _triggered_count == 1);

        condition->setKey(sad::KeyboardKey::X);

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        c.finishReceivingEvents();

        ASSERT_TRUE( _triggered_count == 1);

        c.startReceivingEvents();
        ev.Key = sad::KeyboardKey::X;
        c.finishReceivingEvents();

        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        
        ASSERT_TRUE( _triggered_count == 2);       
   }

   void testSpecialKeyChangeForSpecialKeyHoldPosition()
   {
       sad::input::Controls c;
        _triggered_count = 0;
        c.startReceivingEvents();
        
        sad::input::AbstractHandler* h = new sad::input::FreeFunctionHandler<sad::input::QuitEvent>(::trigger_arg);
        sad::input::HandlerTypeAndConditions conditions;
        sad::SpecialKeyHoldCondition* condition = new sad::SpecialKeyHoldCondition(sad::SpecialKey::HoldsControl);
        conditions.set1(sad::input::EventType::ET_KeyPress);
        conditions._2() << condition;
        c.add(conditions, h);
        c.finishReceivingEvents();

        sad::input::KeyPressEvent ev;
        ev.CtrlHeld = true;
        sad::input::KeyPressEvent ev2;
        ev2.AltHeld = true;

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        c.finishReceivingEvents();
   
        ASSERT_TRUE( _triggered_count == 1);

        condition->setKey(sad::SpecialKey::HoldsShift);

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        c.finishReceivingEvents();
   
        ASSERT_TRUE( _triggered_count == 1);

        ev.CtrlHeld = false;
        ev.ShiftHeld = true;

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_KeyPress, ev);
        c.postEvent(sad::input::EventType::ET_KeyPress, ev2);
        c.finishReceivingEvents();

        ASSERT_TRUE( _triggered_count == 2);  
   }

   void testButtonChangeForMouseButtonCondition()
   {
       sad::input::Controls c;
        _triggered_count = 0;
        c.startReceivingEvents();
        
        sad::input::AbstractHandler* h = new sad::input::FreeFunctionHandler<sad::input::QuitEvent>(::trigger_arg);
        sad::input::HandlerTypeAndConditions conditions;
        sad::MouseButtonHoldCondition* condition = new sad::MouseButtonHoldCondition(sad::MouseButton::MouseLeft);
        conditions.set1(sad::input::EventType::ET_MousePress);
        conditions._2() << condition;
        c.add(conditions, h);
        c.finishReceivingEvents();

        sad::input::MousePressEvent ev;
        ev.Button = sad::MouseButton::MouseLeft;
        sad::input::MousePressEvent ev2;
        ev2.Button = sad::MouseButton::MouseRight;

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_MousePress, ev);
        c.postEvent(sad::input::EventType::ET_MousePress, ev2);
        c.finishReceivingEvents();

        ASSERT_TRUE( _triggered_count == 1);

        condition->setButton(sad::MouseButton::MouseRight);

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_MousePress, ev);
        c.postEvent(sad::input::EventType::ET_MousePress, ev2);
        c.finishReceivingEvents();
   
        ASSERT_TRUE( _triggered_count == 2);

        ev.Button = sad::MouseButton::MouseRight;

        c.startReceivingEvents();
        c.postEvent(sad::input::EventType::ET_MousePress, ev);
        c.postEvent(sad::input::EventType::ET_MousePress, ev2);
        c.finishReceivingEvents();

        ASSERT_TRUE( _triggered_count == 4);  
   }

} _sad_controls_test;
