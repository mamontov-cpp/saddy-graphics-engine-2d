#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "renderer.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct SadRendererTest : tpunit::TestFixture
{
 public:
   SadRendererTest() : tpunit::TestFixture(
       TEST(SadRendererTest::testDefaultTree),
       TEST(SadRendererTest::testAddNULL),
       TEST(SadRendererTest::testAddTake),
       TEST(SadRendererTest::testAddExisting),
       TEST(SadRendererTest::testRemove)
   ) {}
   
   void testDefaultTree()
   {
       sad::Renderer r;
       ASSERT_TRUE( r.tree() != NULL)
   }

   void testAddNULL()
   {
       sad::Renderer r;
       r.addTree("1", NULL);
       ASSERT_TRUE( r.tree("1") == NULL)
   }

   void testAddTake()
   {
       sad::Renderer r;
       r.addTree("1", new sad::resource::Tree());
       ASSERT_TRUE( r.tree("1") != NULL);
       ASSERT_TRUE( r.tree("1")->renderer() == &r);

       sad::resource::Tree * t = r.takeTree("1");
       ASSERT_TRUE( r.tree("1") == NULL);
       delete t;
   }

   void testAddExisting()
   {
       sad::Renderer r;
       r.addTree("1", new sad::resource::Tree());
       ASSERT_TRUE( r.tree("1") != NULL);
       sad::resource::Tree * old = r.tree("1");
       r.addTree("1", new sad::resource::Tree());	
       ASSERT_TRUE( r.tree("1") != old);
   }

   void testRemove()
   {
       sad::Renderer r;
       r.addTree("1", new sad::resource::Tree());
       r.removeTree("1");
       ASSERT_TRUE( r.tree("1") == NULL);
   }
   

} _sad_renderer_test;