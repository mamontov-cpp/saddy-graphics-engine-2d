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

	struct StateMachine
	{
		   int state;
		   p2d::Body * b1;
		   p2d::Body * b2;

		   void step(const p2d::BasicCollisionEvent & ev)
		   {		
				this->state +=1;
				if (this->state == 1)
				{
					b1->world()->splitTimeStepAt(ev.m_time);
					b1->sheduleTangentialVelocityAt(p2d::Vector(0.0, 6.0), ev.m_time);
					b2->setCurrentPosition(p2d::Vector(3.0, 3.0));
				}
				if (this->state == 2)
				{
					b1->world()->splitTimeStepAt(ev.m_time);
					b1->sheduleTangentialVelocityAt(p2d::Vector(-4.0, 0.0), ev.m_time);
					b2->setCurrentPosition(p2d::Vector(7.0, 1.5));
				}
		   }

	};
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
	   TEST(WorldTest::testClearFromCallback),
	   TEST(WorldTest::testListeners),
	   TEST(WorldTest::testMultipleSteps)
   ) {}

   int eventperformed;

   void performEvent(const p2d::BasicCollisionEvent & ev)
   {
	   this->eventperformed++;
   }

   void removeFirstBody(const p2d::BasicCollisionEvent & ev)
   {
	   ev.m_object_1->world()->remove(ev.m_object_1);
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
	   w->add(b1);
	   w->add(b2);
	   
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
	   
	   w->add(b1);
	   w->add(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   ASSERT_TRUE( this->eventperformed == 2 );
	   ASSERT_TRUE( ::eventperformed == 2 );
	   delete w;
	   delete u11;
	   delete u21;
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
	   
	   w->add(b1);
	   w->add(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   ASSERT_TRUE( this->eventperformed == 2 );
	   ASSERT_TRUE( ::eventperformed == 2 );
	   delete w;
	   delete u1;
	   delete u2;
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
	   w->add(b1);
	   
	   w->step(1.0);
	   w->step(1.0);
	   w->remove(b1);
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
	   w->add(b1);
	   
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
	   w->add(b1);
	   w->add(b2);
	   
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
	   w->add(b1);
	   w->add(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   ASSERT_TRUE( ::eventperformed == 2 );
	   delete w;
   }

   void testListeners()
   {
	   p2dworld::UserObject1  * u1 = new p2dworld::UserObject1();
	   p2dworld::UserObject2  * u2 = new p2dworld::UserObject2();

	   this->eventperformed = 0;
	   ::eventperformed = 0;
	   p2d::Body * b1 = new p2d::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(1.0, 0.0));
	   b1->setCurrentAngularVelocity(1.0);
	   b1->setUserObject(u1);
	   b1->addMoveListener( new p2dworld::move1_t(u1, &p2dworld::UserObject1::notifyMove) );
	   b1->addRotateListener( new p2dworld::rotate1_t(u1, &p2dworld::UserObject1::notifyRotate) );

	   p2d::Body * b2 = new p2d::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setUserObject(u2);
	   b2->setCurrentTangentialVelocity(p2d::Vector(1.0, 0.0));
	   b2->setCurrentAngularVelocity(1.0);
	   b2->addMoveListener( new p2dworld::move2_t(u2, &p2dworld::UserObject2::notifyMove) );
	   b2->addRotateListener( new p2dworld::rotate2_t(u2, &p2dworld::UserObject2::notifyRotate) );
	   b2->setCurrentPosition(p2d::Point(6.0, 0.0));
	   
	   p2d::World * w = new p2d::World();
	   
	   w->add(b1);
	   w->add(b2);
	   
	   w->step(1.0);
	   w->step(1.0);
	   
	   ASSERT_TRUE( is_fuzzy_equal(u1->position().x(), 2.0) );
	   ASSERT_TRUE( is_fuzzy_equal(u2->position().x(), 8.0) );
	   ASSERT_TRUE( is_fuzzy_equal(u1->position().y(), 0.0) );
	   ASSERT_TRUE( is_fuzzy_equal(u2->position().y(), 0.0) );

	   ASSERT_TRUE( is_fuzzy_equal(u1->angle(), 2.0) );
	   ASSERT_TRUE( is_fuzzy_equal(u2->angle(), 2.0) );

	   delete w;
	   delete u1;
	   delete u2;
   }

   void testMultipleSteps()
   {
	   p2dworld::StateMachine sm;

	   p2d::Body * b1 = new p2d::Body();
	   p2d::Circle * c1 = new p2d::Circle();
	   c1->setRadius(1.0);
	   b1->setShape(c1);
	   b1->setCurrentTangentialVelocity(p2d::Vector(6.0, 0.0));
	   
	   p2d::Body * b2 = new p2d::Body();
	   p2d::Circle * c2 = new p2d::Circle();
	   c2->setRadius(1.0);
	   b2->setShape(c2);
	   b2->setCurrentPosition(p2d::Point(3.0, 0.0));
	
	   sm.state = 0;
	   sm.b1 = b1;
	   sm.b2 = b2;

	   p2d::BroadCollisionDetector * det = new p2d::BroadCollisionDetector();
	   p2d::World * w = new p2d::World();
	   w->setDetector(det);
	   w->addHandler(&sm, &p2dworld::StateMachine::step);
	   w->add(b1);
	   w->add(b2);
	   
	   w->step(1.0);
	   ASSERT_TRUE( sm.state == 2 );
	   ASSERT_FLOAT_EQUAL( b1->position().x(), 2);
	   ASSERT_FLOAT_EQUAL( b1->position().y(), 1.5);
	   ASSERT_FLOAT_EQUAL( b2->position().x(), 7);
	   ASSERT_FLOAT_EQUAL( b2->position().y(), 1.5);
	   delete w;
   }


} _world_test;
