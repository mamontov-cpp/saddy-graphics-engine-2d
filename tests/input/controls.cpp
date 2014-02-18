#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "input/controls.h"
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
	   TEST(SadControlsTest::testWorkflow)
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
	 
	   c.add(*sad::input::ET_Quit, ::trigger_no_arg);
	   c.add(*sad::input::ET_Quit, ::trigger_arg);
	   c.add(::trigger_arg);
	  
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::trigger_no_arg);
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::trigger_arg);
	   c.add(this, &SadControlsTest::trigger_arg);

	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::trigger_no_arg_const);
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::trigger_arg_const);
	   c.add(this, &SadControlsTest::trigger_arg_const);

	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg),
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_no_arg),
	   c.add(this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg),

	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg),
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_no_arg),
	   c.add(this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg),

	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg_const),
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_no_arg_const),
	   c.add(this, &SadControlsTest::primitive, &sadControlsTestPrimitive::trigger_arg_const),

	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg_const),
	   c.add(*sad::input::ET_Quit, this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_no_arg_const),
	   c.add(this, &SadControlsTest::primitive_const, &sadControlsTestPrimitive::trigger_arg_const),


	   _triggered_count = 0;

	   c.startReceivingEvents();
	   c.postEvent(sad::input::ET_Quit, sad::input::QuitEvent());
	   c.finishRecevingEvents();

	   ASSERT_TRUE(_triggered_count == 21);
   }

   void testWorkflow()
   {
		sad::input::Controls c;
		_triggered_count = 0;
		c.startReceivingEvents();
		sad::input::AbstractHandler* h = new sad::input::FreeFunctionHandler<sad::input::QuitEvent>(::trigger_arg);
		c.add(*sad::input::ET_Quit, h);
		// Handler will not be added
		c.postEvent(sad::input::ET_Quit, sad::input::QuitEvent());
		ASSERT_TRUE(_triggered_count == 0);
		// Here handler will be added
		c.finishRecevingEvents();
		
		c.startReceivingEvents();
		// Here handler will not be removed
		c.remove(h);
		c.postEvent(sad::input::ET_Quit, sad::input::QuitEvent());
		ASSERT_TRUE(_triggered_count == 1);
		// Here handler will be removed
		c.finishRecevingEvents();

		c.startReceivingEvents();
		c.postEvent(sad::input::ET_Quit, sad::input::QuitEvent());
		c.finishRecevingEvents();
		ASSERT_TRUE(_triggered_count == 1);
   }

} _sad_controls_test;
