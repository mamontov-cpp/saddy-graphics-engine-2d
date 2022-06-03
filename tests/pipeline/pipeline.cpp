#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "sadsleep.h"
#include "pipeline/pipeline.h"
#include "pipeline/pipelinepausabledelayedtasklist.h"
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


sad::pipeline::Pipeline * global_pipeline = nullptr;

void trigger_and_add()
{
    ++_triggered_count;
    global_pipeline->appendTask(::trigger);
}

/*!
 * Tests sad::Controls callbacks working and main workflow
 */
struct SadPipelineTest : tpunit::TestFixture
{
 public:
   SadPipelineTest() : tpunit::TestFixture(
       TEST(SadPipelineTest::testSystemCallbacks),
       TEST(SadPipelineTest::testUserCallbacks),
       TEST(SadPipelineTest::testRemoval),
       TEST(SadPipelineTest::testAppendAtRuntime),
       TEST(SadPipelineTest::testPausableDelayed)
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


   void testSystemCallbacks()
   {
       sad::pipeline::Pipeline p;
       _triggered_count = 0;

       p.systemAppendProcess(this, &SadPipelineTest::trigger);
       p.systemAppendProcess(this, &SadPipelineTest::trigger_const);
       p.systemAppendProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.systemAppendProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.systemAppendProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.systemAppendProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::trigger);
       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::trigger_const);
       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.systemPrependSceneRenderingWithProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.run();

       ASSERT_TRUE(_triggered_count == 12);
   }

   void testUserCallbacks()
   {
       sad::pipeline::Pipeline p;


       p.prependProcess(::trigger);
       p.prependProcess(this, &SadPipelineTest::trigger);
       p.prependProcess(this, &SadPipelineTest::trigger_const);
       p.prependProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.prependProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.prependProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.prependProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.prependTask(::trigger);
       p.prependTask(this, &SadPipelineTest::trigger);
       p.prependTask(this, &SadPipelineTest::trigger_const);
       p.prependTask(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.prependTask(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.prependTask(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.prependTask(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.appendProcess(::trigger);
       p.appendProcess(this, &SadPipelineTest::trigger);
       p.appendProcess(this, &SadPipelineTest::trigger_const);
       p.appendProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.appendProcess(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.appendProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.appendProcess(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.appendTask(::trigger);
       p.appendTask(this, &SadPipelineTest::trigger);
       p.appendTask(this, &SadPipelineTest::trigger_const);
       p.appendTask(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger);
       p.appendTask(this, &SadPipelineTest::primitive,  &sadPPrimitive::trigger_const);
       p.appendTask(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger);
       p.appendTask(this, &SadPipelineTest::primitive_const,  &sadPPrimitive::trigger_const);

       p.appendProcess(::trigger)->mark("Trigger");
       p.insertBefore("Trigger", new sad::pipeline::Process(::trigger));
       p.insertAfter("Trigger", new sad::pipeline::Process(::trigger));

       _triggered_count = 0;
       p.run();

       ASSERT_TRUE(_triggered_count == 31);
       _triggered_count = 0;
       p.run();
       ASSERT_TRUE(_triggered_count == 17);
   }
   

   void testRemoval()
   {
        sad::pipeline::Pipeline p;
        sad::pipeline::Step * step = p.appendProcess(::trigger);
        _triggered_count = 0;
        p.run();
        ASSERT_TRUE(_triggered_count == 1);

        p.remove(step);
        _triggered_count = 0;
        p.run();
        ASSERT_TRUE(_triggered_count == 0);
   }

   void testAppendAtRuntime()
   {
       sad::pipeline::Pipeline p;
       global_pipeline = &p;
       _triggered_count = 0;
       p.appendProcess(::trigger_and_add);
       p.run();
       ASSERT_TRUE(_triggered_count == 2);
   }


   // ReSharper disable once CppMemberFunctionMayBeStatic
   void testPausableDelayed()
   {
       sad::pipeline::PausableDelayedTaskList list;
       _triggered_count = 0;
       sad::pipeline::AbstractTask* task = list.makeTask(::trigger, 600);
       task->process();
       ASSERT_TRUE(list.count() == 1)
       ASSERT_TRUE(_triggered_count == 0)
       sad::sleep(800);
       task->process();
       ASSERT_TRUE(list.count() == 0)
       ASSERT_TRUE(_triggered_count == 1)
   }

} _sad_controls_test;
