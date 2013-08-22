#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "p2d/world.h"
#pragma warning(pop)


namespace p2dworld
{		  
	class UserObject1: public sad::Object
	{
		SAD_OBJECT

		p2d::Point m_pos;
		double m_angle;
      public:
		  UserObject1() { m_angle = 0;}

		  void notifyMove(const p2d::Vector & v) { m_pos += v; }
		  void notifyRotate(const double & a) { m_angle += a; }
		  
		  const p2d::Point & position() const { return m_pos;}
		  double angle() const { return m_angle; }
	};

	class UserObject2: public sad::Object
	{
		SAD_OBJECT
		p2d::Point m_pos;
		double m_angle;
      public:
		  UserObject2() { m_angle = 0;}

		  void notifyMove(const p2d::Vector & v) { m_pos += v; }
		  void notifyRotate(const double & a) { m_angle += a; }

		  const p2d::Point & position() const { return m_pos;}
		  double angle() const { return m_angle; }
	};
	
	typedef p2d::MovementDeltaListener<p2dworld::UserObject1, p2d::Vector> move1_t;
	typedef p2d::MovementDeltaListener<p2dworld::UserObject2, p2d::Vector> move2_t;
	typedef p2d::MovementDeltaListener<p2dworld::UserObject1, double> rotate1_t;
	typedef p2d::MovementDeltaListener<p2dworld::UserObject2, double> rotate2_t;
}
DECLARE_SOBJ(p2dworld::UserObject1);
DECLARE_SOBJ(p2dworld::UserObject2);

int eventperformed = 0; 

namespace p2dworld
{
	class Body: public p2d::Body
	{
      public:
		  Body() : p2d::Body() {}

		  ~Body() { ::eventperformed++; }
	};
}

void performEvent(const p2d::BasicCollisionEvent & ev)
{
	eventperformed++;
}

void performEvent11(const p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject1> & ev)
{
	eventperformed++;
}

void performEvent12(const p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject2> & ev)
{
	eventperformed++;
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
	   TEST(WorldTest::testClearFromCallback)
   ) {}

   int eventperformed;

   void performEvent(const p2d::BasicCollisionEvent & ev)
   {
	   this->eventperformed++;
   }

   void removeFirstBody(const p2d::BasicCollisionEvent & ev)
   {
	   ev.m_object_1->world()->removeBody(ev.m_object_1);
   }

   void clearWorld(const p2d::BasicCollisionEvent & ev)
   {
	   ev.m_object_1->world()->clear();
   }

   void performEvent11(const p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject1> & ev)
   {
	   this->eventperformed++;
   }

   void performEvent12(const p2d::CollisionEvent<p2dworld::UserObject1, p2dworld::UserObject2> & ev)
   {
	   this->eventperformed++;
   }

   void testCollisionCallbacksForBodies()
   {
	   this->eventperformed = 0;
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2d::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   
	   p2d::Body * b2 = new p2d::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));

	   p2d::World * w = new p2d::World();
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

	   this->eventperformed = 0;
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2d::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   b1->setUserObject(u11);

	   p2d::Body * b2 = new p2d::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));
	   b2->setUserObject(u21);

	   p2d::World * w = new p2d::World();
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
   }

   void testCollisionCallbacksForU1U2()
   {
	   p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
	   p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();

	   this->eventperformed = 0;
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2d::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   b1->setUserObject(u1);

	   p2d::Body * b2 = new p2d::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));
	   b2->setUserObject(u2);

	   p2d::World * w = new p2d::World();
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
   }

   void testRemove()
   {
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2dworld::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   
	   p2d::World * w = new p2d::World();
	   w->addBody(b1);
	   
	   w->step(1.0);
	   w->step(1.0);
	   w->removeBody(b1);
	   ASSERT_TRUE( ::eventperformed == 1 );
	   delete w;
   }

   void testClear()
   {
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2dworld::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   
	   p2d::World * w = new p2d::World();
	   w->addBody(b1);
	   
	   w->step(1.0);
	   w->step(1.0);
	   w->clear();
	   ASSERT_TRUE( ::eventperformed == 1 );
	   delete w;
   }

   void testRemoveFromCallback()
   {
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2dworld::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   
	   p2d::Body * b2 = new p2dworld::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));

	   p2d::World * w = new p2d::World();
	   w->addHandler(this, &WorldTest::removeFirstBody);
	   w->addBody(b1);
	   w->addBody(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   ASSERT_TRUE( ::eventperformed == 1 );
	   delete w;
   }

   void testClearFromCallback()
   {
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2dworld::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(3.0, 0.0));
	   
	   p2d::Body * b2 = new p2dworld::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));

	   p2d::World * w = new p2d::World();
	   w->addHandler(this, &WorldTest::clearWorld);
	   w->addBody(b1);
	   w->addBody(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   ASSERT_TRUE( ::eventperformed == 2 );
	   delete w;
   }


} _world_test;
