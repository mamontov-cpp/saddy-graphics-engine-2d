#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/world.h"
#pragma warning(pop)


namespace p2dworld
{		  
    class UserObject1: public sad::Object
    {
        SAD_OBJECT

        sad::p2d::Point m_pos;
        double m_angle;
      public:
          UserObject1() { m_angle = 0;}

          void notifyMove(const sad::p2d::Vector & v) { m_pos += v; }
          void notifyRotate(const double & a) { m_angle += a; }
          
          const sad::p2d::Point & position() const { return m_pos;}
          double angle() const { return m_angle; }
    };

    class UserObject2: public sad::Object
    {
        SAD_OBJECT
        sad::p2d::Point m_pos;
        double m_angle;
      public:
          UserObject2() { m_angle = 0;}

          void notifyMove(const sad::p2d::Vector & v) { m_pos += v; }
          void notifyRotate(const double & a) { m_angle += a; }

          const sad::p2d::Point & position() const { return m_pos;}
          double angle() const { return m_angle; }
    };
    
    typedef sad::p2d::MovementDeltaListener<p2dworld::UserObject1, sad::p2d::Vector> move1_t;
    typedef sad::p2d::MovementDeltaListener<p2dworld::UserObject2, sad::p2d::Vector> move2_t;
    typedef sad::p2d::MovementDeltaListener<p2dworld::UserObject1, double> rotate1_t;
    typedef sad::p2d::MovementDeltaListener<p2dworld::UserObject2, double> rotate2_t;
}
DECLARE_SOBJ(p2dworld::UserObject1);
DECLARE_SOBJ(p2dworld::UserObject2);

int eventperformed = 0; 

namespace p2dworld
{
    class Body: public sad::p2d::Body
    {
      public:
          Body() : sad::p2d::Body() {}

          ~Body() { ::eventperformed++; }
    };
}

void performEvent(const sad::p2d::BasicCollisionEvent & ev)
{
    eventperformed++;
}

void performEvent11(const sad::p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject1> & ev)
{
    eventperformed++;
}

void performEvent12(const sad::p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject2> & ev)
{
    eventperformed++;
}

void performEventChecked(const sad::p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject1> & ev)
{ 
   if (ev.m_object_2 == NULL || ev.m_object_1 == NULL)
       eventperformed++;
}

/*! Makes body as circle with specified velocity at specified position
    \param[in] radius a radius of object
    \param[in] x an x position
    \param[in] y an y position
    \param[in] vx a horizontal velocity
    \param[in] vy a vertical velocity
 */
static sad::p2d::Body* makeBodyWithVelocity(double radius, double x, double y, double vx, double vy) {
    sad::p2d::Body* b = new sad::p2d::Body();
    sad::p2d::Circle* c = new sad::p2d::Circle();
    c->setRadius(1);
    b->setShape(c);
    b->setCurrentPosition(sad::p2d::Point(x, y));
    b->setCurrentTangentialVelocity(sad::p2d::Vector(vx, vy));
    b->addRef();
    return b;
}


/*!
 * Tests working with world
 */
struct WorldTest : tpunit::TestFixture
{
 public:
    WorldTest() : tpunit::TestFixture(
        TEST(WorldTest::testCollisionCallbacksForBodies),
        TEST(WorldTest::testCollisionCallbacksForU1U1),
        TEST(WorldTest::testCollisionCallbacksForU1U2),
        TEST(WorldTest::testRemove),
        TEST(WorldTest::testClear),
        TEST(WorldTest::testRemoveFromCallback),
        TEST(WorldTest::testClearFromCallback),
        TEST(WorldTest::testListeners),
        TEST(WorldTest::testPickedGroups),
        TEST(WorldTest::testPickedGroupsTypeCheck),
        TEST(WorldTest::testNestedStep),
        TEST(WorldTest::testNestedStep2),
        TEST(WorldTest::testAddBodyToWorld),
        TEST(WorldTest::testRemoveBodyFromWorld),
        TEST(WorldTest::testClearBodies),
        TEST(WorldTest::testAddBodyToGroup),
        TEST(WorldTest::testRemoveBodyFromGroup),
        TEST(WorldTest::testGlearGroup),
        TEST(WorldTest::testAddGroup),
        TEST(WorldTest::testRemoveGroup),
        TEST(WorldTest::testClearGroups),
        TEST(WorldTest::testAddHandler),
        TEST(WorldTest::testRemoveHandler),
        TEST(WorldTest::testRemoveHandlerFromGroups),
        TEST(WorldTest::testClearHandlers),
        TEST(WorldTest::testClearHandlersForGroups),
        TEST(WorldTest::testClearWorld)
    ), eventperformed(0) {}

    int eventperformed;

    void performEvent(const sad::p2d::BasicCollisionEvent & ev)
    {
        this->eventperformed++;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void removeFirstBody(const sad::p2d::BasicCollisionEvent & ev)
    {
        ev.m_object_1->world()->removeBody(ev.m_object_1);
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void clearWorld(const sad::p2d::BasicCollisionEvent & ev)
    {
        ev.m_object_1->world()->clear();
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void performEvent11(const sad::p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject1> & ev)
    {
        this->eventperformed++;
    }

    void performEvent12(const sad::p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject2> & ev)
    {
        this->eventperformed++;
    }

    void testCollisionCallbacksForBodies()
    {
        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
       
        sad::p2d::Body * b2 = new sad::p2d::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));

        sad::p2d::World * w = new sad::p2d::World();
        w->addHandler(::performEvent);
        w->addHandler(this, &WorldTest::performEvent);
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( this->eventperformed == 1 );
        ASSERT_TRUE( ::eventperformed == 1 );
        delete w;
    }

    void testCollisionCallbacksForU1U1()
    {
        p2dworld::UserObject1  * u11 = new p2dworld::UserObject1();
        p2dworld::UserObject1  * u21 = new p2dworld::UserObject1();
        u11->addRef();
        u21->addRef();

        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
        b1->setUserObject(u11);

        sad::p2d::Body * b2 = new sad::p2d::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));
        b2->setUserObject(u21);

        sad::p2d::World * w = new sad::p2d::World();
        w->addHandler(::performEvent);
        w->addHandler(this, &WorldTest::performEvent);
        w->addHandler(::performEvent11);
        w->addHandler(this, &WorldTest::performEvent11);
       
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( this->eventperformed == 2 );
        ASSERT_TRUE( ::eventperformed == 2 );
        delete w;
        u11->delRef();
        u21->delRef();
    }

    void testCollisionCallbacksForU1U2()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();
        u1->addRef();
        u2->addRef();
   
        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
        b1->setUserObject(u1);

        sad::p2d::Body * b2 = new sad::p2d::Body();
       sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));
        b2->setUserObject(u2);

        sad::p2d::World * w = new sad::p2d::World();
        w->addHandler(::performEvent);
        w->addHandler(this, &WorldTest::performEvent);
        w->addHandler(::performEvent12);
        w->addHandler(this, &WorldTest::performEvent12);
       
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( this->eventperformed == 2 );
        ASSERT_TRUE( ::eventperformed == 2 );
        delete w;
        u1->delRef();
        u2->delRef();
    }

   // ReSharper disable once CppMemberFunctionMayBeStatic
   // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemove()
    {
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new p2dworld::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
       
        sad::p2d::World * w = new sad::p2d::World();
        w->addBody(b1);
       
        w->step(1.0);
        w->step(1.0);
        w->removeBody(b1);
        ASSERT_TRUE( ::eventperformed == 1 );
        delete w;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClear()
    {
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new p2dworld::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
       
        sad::p2d::World * w = new sad::p2d::World();
        w->addBody(b1);
       
        w->step(1.0);
        w->step(1.0);
        w->clear();
        ASSERT_TRUE( ::eventperformed == 1 );
        delete w;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveFromCallback()
    {
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new p2dworld::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
       
        sad::p2d::Body * b2 = new p2dworld::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));

        sad::p2d::World * w = new sad::p2d::World();
        w->addHandler(this, &WorldTest::removeFirstBody);
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( ::eventperformed == 1 );
        delete w;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearFromCallback()
    {
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new p2dworld::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
       
        sad::p2d::Body * b2 = new p2dworld::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));

        sad::p2d::World * w = new sad::p2d::World();
        w->addHandler(this, &WorldTest::clearWorld);
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( ::eventperformed == 2 );
        delete w;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testListeners()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();
        u1->addRef();
        u2->addRef();
   
        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(1.0, 0.0));
        b1->setCurrentAngularVelocity(1.0);
        b1->setUserObject(u1);
        b1->addMoveListener( new p2dworld::move1_t(u1, &p2dworld::UserObject1::notifyMove) );
        b1->addRotateListener( new p2dworld::rotate1_t(u1, &p2dworld::UserObject1::notifyRotate) );

        sad::p2d::Body * b2 = new sad::p2d::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setUserObject(u2);
        b2->setCurrentTangentialVelocity(sad::p2d::Vector(1.0, 0.0));
        b2->setCurrentAngularVelocity(1.0);
        b2->addMoveListener( new p2dworld::move2_t(u2, &p2dworld::UserObject2::notifyMove) );
        b2->addRotateListener( new p2dworld::rotate2_t(u2, &p2dworld::UserObject2::notifyRotate) );
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));
       
        sad::p2d::World * w = new sad::p2d::World();
       
        w->addBody(b1);
        w->addBody(b2);
       
        w->step(1.0);
        w->step(1.0);
       
        ASSERT_TRUE( sad::is_fuzzy_equal(u1->position().x(), 2.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(u2->position().x(), 8.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(u1->position().y(), 0.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(u2->position().y(), 0.0) );

        ASSERT_TRUE( sad::is_fuzzy_equal(u1->angle(), 2.0) );
        ASSERT_TRUE( sad::is_fuzzy_equal(u2->angle(), 2.0) );

        delete w;

        u1->delRef();
        u2->delRef();
    }

    void testPickedGroups()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();
        u1->addRef();
        u2->addRef();

        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
        b1->setUserObject(u1);

        sad::p2d::Body * b2 = new sad::p2d::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));
        b2->setUserObject(u2);

        sad::p2d::Body * b3 = new sad::p2d::Body();
        sad::p2d::Circle * c3 = new sad::p2d::Circle();
        c3->setRadius(1.0);
        b3->setShape(c3);
        b3->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));

        sad::p2d::World * w = new sad::p2d::World();
        // Ok, we ignored handlers
        w->addHandler("first", "first", ::performEvent);
        w->addHandler(::performEvent);
       
        w->addBodyToGroup("first", b1);
        w->addBodyToGroup("p2d::Body", b2);
        w->addBodyToGroup("first", b2);
        w->addBodyToGroup("p2d::Body", b3);
        w->addBodyToGroup("second", b3);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( ::eventperformed == 2 );
        delete w;
        u1->delRef();
        u2->delRef();  
    }

    void testPickedGroupsTypeCheck()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        //p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();
        u1->addRef();

        this->eventperformed = 0;
        ::eventperformed = 0;
        sad::p2d::Body * b1 = new sad::p2d::Body();
        sad::p2d::Circle * c1 = new sad::p2d::Circle();
        c1->setRadius(1.0);
        b1->setShape(c1);
        b1->setCurrentTangentialVelocity(sad::p2d::Vector(3.0, 0.0));
        b1->setUserObject(u1);

        sad::p2d::Body * b2 = new sad::p2d::Body();
        sad::p2d::Circle * c2 = new sad::p2d::Circle();
        c2->setRadius(1.0);
        b2->setShape(c2);
        b2->setCurrentPosition(sad::p2d::Point(6.0, 0.0));
        //b2->setUserObject(u2);


        sad::p2d::World * w = new sad::p2d::World();
        // Ok, we ignored handlers
        w->addHandler("first", "second", ::performEvent);
       
        w->addBodyToGroup("first", b1);
        w->addBodyToGroup("second", b2);
       
        w->step(1.0);
        w->step(1.0);
        ASSERT_TRUE( ::eventperformed > 0 );
        delete w;
        u1->delRef();
        //delete u2;  
     }

    /*! Tests a nested step for making data
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testNestedStep()
    {
       sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
       sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
       sad::p2d::World* w = new sad::p2d::World();
       w->addBodyToGroup("step_world_1", b1);
       w->addBodyToGroup("step_world_2", b2);
       w->addHandler("step_world_1", "step_world_2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
           w->removeBody(ev.m_object_1);
           w->removeBody(ev.m_object_2);
           sad::p2d::Body* b3 = makeBodyWithVelocity(1, 0, 0, 1, 0);
           sad::p2d::Body* b4 = makeBodyWithVelocity(1, 0, 0, -1, 0);

           w->addBodyToGroup("step_world_3", b3);
           w->addBodyToGroup("step_world_4", b4);

           w->addHandler("step_world_3", "step_world_4", [w](const sad::p2d::BasicCollisionEvent& ev2) {
               w->removeBody(ev2.m_object_1);
               w->removeBody(ev2.m_object_2);
           });

           w->step(1.0);
       });
       w->step(1.0);

       size_t total_amount_bodies = w->totalBodyCount();
       ASSERT_TRUE(total_amount_bodies == 0);

       ASSERT_TRUE(w->isBodyInWorld(b1) == false);
       ASSERT_TRUE(w->isBodyInWorld(b2) == false);

       ASSERT_TRUE(w->isInGroup("step_world_1", b1) == false);
       ASSERT_TRUE(w->isInGroup("step_world_2", b1) == false);

       ASSERT_TRUE(w->isInGroup("step_world_3", b2) == false);
       ASSERT_TRUE(w->isInGroup("step_world_4", b2) == false);

       ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_1") == 0);
       ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_2") == 0);
       ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_3") == 0);
       ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_4") == 0);


       delete w;
    }


    /*! Tests a nested step two times for splitting time in world
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testNestedStep2()
    {
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("step_world_1", b1);
        w->addBodyToGroup("step_world_2", b2);
        bool do_not_iterate_anymore = false;
        w->addHandler("step_world_1", "step_world_2", [w, b1, b2, &do_not_iterate_anymore](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeBody(ev.m_object_1);
            w->removeBody(ev.m_object_2);
            sad::p2d::Body* b3 = makeBodyWithVelocity(1, 0, 0, 1, 0);
            sad::p2d::Body* b4 = makeBodyWithVelocity(1, 0, 0, -1, 0);

            w->addBodyToGroup("step_world_3", b3);
            w->addBodyToGroup("step_world_4", b4);

            w->addHandler("step_world_3", "step_world_4", [w, &do_not_iterate_anymore](const sad::p2d::BasicCollisionEvent& ev2) {
                w->removeBody(ev2.m_object_1);
                w->removeBody(ev2.m_object_2);

                if (!do_not_iterate_anymore)
                {
                    sad::p2d::Body* b5 = makeBodyWithVelocity(1, 0, 0, 1, 0);
                    sad::p2d::Body* b6 = makeBodyWithVelocity(1, 0, 0, -1, 0);

                    w->addBodyToGroup("step_world_3", b5);
                    w->addBodyToGroup("step_world_4", b6);

                    do_not_iterate_anymore = true;
                    w->step(1.0);
                }
            });

            w->step(1.0);
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 0);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == false);

        ASSERT_TRUE(w->isInGroup("step_world_1", b1) == false);
        ASSERT_TRUE(w->isInGroup("step_world_2", b1) == false);

        ASSERT_TRUE(w->isInGroup("step_world_3", b2) == false);
        ASSERT_TRUE(w->isInGroup("step_world_4", b2) == false);

        ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_2") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_3") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("step_world_4") == 0);


        delete w;
    }

    /*! Tests adding body to a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAddBodyToWorld()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBody(b1);
        w->addBody(b2);
        w->addHandler([w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            sad::p2d::Body* b3 = makeBodyWithVelocity(1, 0, 0, 1, 0);
            sad::p2d::Body* b4 = makeBodyWithVelocity(1, 0, 0, -1, 0);

            w->addBody(b3);
            w->addBody(b4);
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 4);
        ASSERT_TRUE(w->amountOfBodiesInGroup("p2dworld::UserObject1") == 2);

        ASSERT_TRUE(w->isBodyInWorld(b1) == true);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == true);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == true);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == true);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == true);

        delete w;
    }

    /*! Tests removing body from a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveBodyFromWorld()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBody(b1);
        w->addBody(b2);
        w->addHandler([w](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeBody(ev.m_object_1);
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 1);
        ASSERT_TRUE(w->amountOfBodiesInGroup("p2dworld::UserObject1") == 1);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == true);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == true);

        delete w;
    }

    /*! Tests clearing bodies
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearBodies()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBody(b1);
        w->addBody(b2);
        w->addHandler([w](const sad::p2d::BasicCollisionEvent& ev) {
            w->clearBodies();
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("p2dworld::UserObject1") == 0);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == false);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("p2d::Body"));
        ASSERT_TRUE(w->doesGroupExists("p2dworld::UserObject1"));

        delete w;
    }

    /*! Tests adding bodies to groups of the world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAddBodyToGroup()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            sad::p2d::Body* b3 = makeBodyWithVelocity(1, 0, 0, 1, 0);
            w->addBodyToGroup("b3", b3);
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 3);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 1);

        ASSERT_TRUE(w->isBodyInWorld(b1) == true);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b3"));
        ASSERT_TRUE(w->doesGroupExists("b1"));
        ASSERT_TRUE(w->doesGroupExists("b2"));
        ASSERT_TRUE(w->amountOfBodiesInGroup("b3") == 1);

        ASSERT_TRUE(w->allBodies().size() == 3);
        ASSERT_TRUE(w->allBodiesInGroup("b3").size() == 1);
        ASSERT_TRUE(w->existingGroups().size() == 3);


        delete w;
    }

    /*! Tests adding bodies to groups of the world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveBodyFromGroup()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeBodyFromGroup("b1", b1);
        });
        w->step(1.0);
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 1);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("b1", b1) == false);
        ASSERT_TRUE(w->isInGroup("b2", b2) == true);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b1"));
        ASSERT_TRUE(w->doesGroupExists("b2"));

        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b2") == 1);

        delete w;
    }

    /*! Tests clearing group
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testGlearGroup()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->clearGroup("b1");
        });
        w->step(1.0);
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 1);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("b1", b1) == false);
        ASSERT_TRUE(w->isInGroup("b2", b2) == true);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b1"));
        ASSERT_TRUE(w->doesGroupExists("b2"));

        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b2") == 1);

        delete w;
    }

    /*! Tests adding group to a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAddGroup()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->addGroup("b3");
        });
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 2);

        ASSERT_TRUE(w->isBodyInWorld(b1) == true);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b3"));
        ASSERT_TRUE(w->doesGroupExists("b1"));
        ASSERT_TRUE(w->doesGroupExists("b2"));
        ASSERT_TRUE(w->amountOfBodiesInGroup("b3") == 0);

        delete w;
    }

    /*! Tests removing group to a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveGroup()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeGroup("b1");
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 1);

        ASSERT_TRUE(w->amountOfHandlers() == 0);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == true);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b1") == false);
        ASSERT_TRUE(w->doesGroupExists("b2"));
        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b2") == 1);
        ASSERT_TRUE(w->totalGroupCount() == 1);


        delete w;
    }

    /*! Tests clearing groups in a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearGroups()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->clearGroups();
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == false);
        ASSERT_TRUE(w->totalGroupCount() == 0);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b1") == false);
        ASSERT_TRUE(w->doesGroupExists("b2") == false);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b2") == 0);

        delete w;
    }

    /*! Tests adding handler to a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testAddHandler()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->addHandler("b1", "b2", [](const sad::p2d::BasicCollisionEvent& e) {});
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);

        ASSERT_TRUE(w->amountOfHandlers() == 2);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 2);

        ASSERT_TRUE(w->allHandlers().size() == 2);
        ASSERT_TRUE(w->allHandlersForGroups("b1", "b2").size() == 2);


        delete w;
    }

    /*! Tests removing handler from a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveHandler()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        sad::p2d::BasicCollisionHandler* h = w->addHandler("b1", "b2", [w, b1, b2, &h](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeHandler(h);
        });
        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 0);
        ASSERT_TRUE(w->isHandlerInWorld(h) == false);
        ASSERT_TRUE(w->isHandlerInGroups("b1", "b2", h) == false);

        delete w;
    }

    /*! Tests removing handler from groups of a world
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testRemoveHandlerFromGroups()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        sad::p2d::BasicCollisionHandler* h = w->addHandler("b1", "b2", [w, b1, b2, &h](const sad::p2d::BasicCollisionEvent& ev) {
            w->removeHandlerFromGroups("b1", "b2", h);
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 0);
        ASSERT_TRUE(w->totalHandlerOccurences(h) == 0);
        ASSERT_TRUE(w->totalHandlerOccurencesInGroups("b1", "b2", h) == 0);
        ASSERT_TRUE(w->isHandlerInWorld(h) == false);
        ASSERT_TRUE(w->isHandlerInGroups("b1", "b2", h) == false);

        delete w;
    }

    /*! Tests clearing handlers
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearHandlers()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        sad::p2d::BasicCollisionHandler* h = w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->clearHandlers();
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 0);
        ASSERT_TRUE(w->totalHandlerOccurences(h) == 0);
        ASSERT_TRUE(w->totalHandlerOccurencesInGroups("b1", "b2", h) == 0);
        ASSERT_TRUE(w->isHandlerInWorld(h) == false);
        ASSERT_TRUE(w->isHandlerInGroups("b1", "b2", h) == false);

        delete w;
    }


    /*! Tests clearing handlers for groups
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearHandlersForGroups()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        sad::p2d::BasicCollisionHandler* h = w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->clearHandlersForGroups("b1", "b2");
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 0);
        ASSERT_TRUE(w->totalHandlerOccurences(h) == 0);
        ASSERT_TRUE(w->totalHandlerOccurencesInGroups("b1", "b2", h) == 0);
        ASSERT_TRUE(w->isHandlerInWorld(h) == false);
        ASSERT_TRUE(w->isHandlerInGroups("b1", "b2", h) == false);

        delete w;
    }

    /*! Tests clearing the world
    */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testClearWorld()
    {
        p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
        sad::p2d::Body* b1 = makeBodyWithVelocity(1, 0, 0, 1, 0);
        b1->setUserObject(u1);
        sad::p2d::Body* b2 = makeBodyWithVelocity(1, 4, 0, -1, 0);
        b2->setUserObject(u1);

        sad::p2d::World* w = new sad::p2d::World();
        w->addBodyToGroup("b1", b1);
        w->addBodyToGroup("b2", b2);
        w->addHandler("b1", "b2", [w, b1, b2](const sad::p2d::BasicCollisionEvent& ev) {
            w->clear();
        });

        ASSERT_TRUE(w->amountOfHandlers() == 1);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 1);

        w->step(1.0);
        w->step(1.0);
        w->step(1.0);

        size_t total_amount_bodies = w->totalBodyCount();
        ASSERT_TRUE(total_amount_bodies == 0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);

        ASSERT_TRUE(w->isBodyInWorld(b1) == false);
        ASSERT_TRUE(w->isBodyInWorld(b2) == false);
        ASSERT_TRUE(w->totalGroupCount() == 0);

        ASSERT_TRUE(w->isInGroup("p2d::Body", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2d::Body", b2) == false);

        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b1) == false);
        ASSERT_TRUE(w->isInGroup("p2dworld::UserObject1", b2) == false);

        ASSERT_TRUE(w->doesGroupExists("b1") == false);
        ASSERT_TRUE(w->doesGroupExists("b2") == false);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b1") == 0);
        ASSERT_TRUE(w->amountOfBodiesInGroup("b2") == 0);

        ASSERT_TRUE(w->amountOfHandlers() == 0);
        ASSERT_TRUE(w->amountOfHandlersForGroups("b1", "b2") == 0);


        delete w;
    }

} _world_test;
