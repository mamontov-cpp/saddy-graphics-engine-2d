#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "3rdparty/tpunit++/tpunit++.hpp"
#include <p2d/bouncesolver.h>
#include <p2d/walls.h>
#include <p2d/body.h>
#include <p2d/circle.h>
#include <p2d/world.h>
#include <object.h>
#pragma warning(pop)


class Ball: public sad::Object
{
    SAD_OBJECT;
};

DECLARE_SOBJ(Ball);

/*!
 * Tests collides1d operation
 */
struct BounceSolverTest : tpunit::TestFixture
{
public:
    BounceSolverTest() : tpunit::TestFixture(
     TEST(BounceSolverTest::testBug1),
     TEST(BounceSolverTest::testInelasticLeftCollision1),
     TEST(BounceSolverTest::testInelasticLeftCollision2),
     TEST(BounceSolverTest::testInelasticLeftCollision3),
     TEST(BounceSolverTest::testInelasticLeftCollision4),
     TEST(BounceSolverTest::testInelasticLeftCollision5),
     TEST(BounceSolverTest::testInelasticLeftCollision6),
     TEST(BounceSolverTest::testInelasticLeftCollision7),
     TEST(BounceSolverTest::testInelasticLeftCollision8),
     TEST(BounceSolverTest::testInelasticLeftCollision9),
     TEST(BounceSolverTest::testInelasticLeftCollision10),
     TEST(BounceSolverTest::testInelasticLeftCollision11),
     TEST(BounceSolverTest::testInelasticLeftCollision12)
    ), m_solver(nullptr)
    {
    }

    sad::p2d::BounceSolver * m_solver;

   void onWallBall(const sad::p2d::CollisionEvent<Ball, sad::p2d::Wall> & ev)
   {
       m_solver->pushRotationFriction(0.0001, 1);
       m_solver->bounce(ev.sad::p2d::BasicCollisionEvent::m_object_1, ev.m_object_2->body());
   }

    /**
     * This test-case reproduces bug of 17.09.13, when ball left bounds and continued
     * to drown beyond bounds of screen
     */
    void testBug1()
    {
        // Create ball
        Ball * ball = new Ball(); 
        ball->addRef();
        sad::p2d::Body * ballbody = new sad::p2d::Body();

        sad::p2d::Circle * circle = new sad::p2d::Circle();
        circle->setRadius(11);
        circle->setCenter(sad::p2d::Vector(0, 0));
        ballbody->setShape(circle);

        ballbody->setUserObject(ball);
        ballbody->tangentialForces().add( 
            new sad::p2d::TangentialForce(sad::p2d::Vector(0, -30) ) 
        );
        ballbody->setCurrentTangentialVelocity(sad::p2d::Vector(-195.131, -268.973));
        ballbody->setCurrentAngularVelocity(1.0);
        ballbody->setCurrentPosition(sad::p2d::Vector(12.8099,11.5461));

        // Create walls
        sad::p2d::Walls * walls = new sad::p2d::Walls(800, 600);

        // Create world
        sad::p2d::World * world = new sad::p2d::World();
        world->addHandler(this, &BounceSolverTest::onWallBall);
        world->addBody(ballbody);
        for(unsigned int i = 0; i < walls->bodies().size(); i++) 
        {
            world->addBody(walls->bodies()[i]);
        }

        // Create solver
        m_solver = new sad::p2d::BounceSolver();

        // FPS was 106.58, so timestep must be like this
        double timestep = 1.0 / 106.58;
        world->step(timestep);

        sad::p2d::Vector pos = ballbody->position();
        sad::p2d::Vector v  = ballbody->tangentialVelocity();

        delete world;
        delete m_solver;
        delete walls;
        ball->delRef();

        ASSERT_TRUE( sad::is_fuzzy_equal(v.x(), 0, 1) == false );
        ASSERT_TRUE( sad::is_fuzzy_equal(v.y(), 0, 1) == false );
        ASSERT_TRUE( pos.x() > 11.0 );
        ASSERT_TRUE( pos.y() > 11.0 );
    }

    /*! Tests inelastic collision from left
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision1()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(6, 2, 8, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(7, 3));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-5, 0));
        
        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 3, 6));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(1.5, 3));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

       sad::p2d::CollisionTest test;
       ASSERT_TRUE( !test.invoke(player->currentShape(), platform->currentShape()) );
       ASSERT_TRUE( sad::is_fuzzy_equal(player->currentShape()->center().y(), 3.0));
       ASSERT_TRUE( player->currentShape()->center().x() >= 4.0 );
       ASSERT_TRUE( player->currentShape()->center().x() <= 5.0 );
    }


    /*! Tests inelastic collision from left
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision2()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(6, 2, 8, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(7, 3));
        
        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 3, 6));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(1.5, 3));
        platform->setCurrentTangentialVelocity(sad::p2d::Vector(5, 0));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 3.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= 9.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 10.0);
    }

    /*! Tests inelastic collision from left
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision3()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(6, 2, 8, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(7, 3));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-5, 1));


        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 3, 6));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(1.5, 3));
        platform->setCurrentTangentialVelocity(sad::p2d::Vector(5, 0));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 5.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= 14.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 15.0);
    }


    /*! Tests inelastic collision from left
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision4()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(2, 1, 4, 3));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(3, 2));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 2.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= 5.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 6.0);
    }

    /*! Tests inelastic collision from right
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision5()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(0, 1, 2, 3));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(3, 2));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 2.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= -2.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= -1.0);
    }

    /*! Tests inelastic collision from up
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision6()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(1, 2, 3, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(2, 3));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().x(), 2.0));
        ASSERT_TRUE(player->currentShape()->center().y() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 6.0);
    }


    /*! Tests inelastic collision from down
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision7()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(1, 0, 3, 2));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(2, 1));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().x(), 2.0));
        ASSERT_TRUE(player->currentShape()->center().y() >= -2.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 0.0);
    }

    /*! Tests inelastic collision from top-up
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision8()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(2, 2, 4, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(3, 3));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 6.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 6.0);
    }

    /*! Tests inelastic collision from center
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision9()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(1, 1, 3, 3));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(2, 2));

        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 4, 4));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(2, 2));

        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 2.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 6.0);
    }


    /*! Tests inelastic collision from right
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision10()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(0, 2, 2, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(1, 3));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(2, 1));


        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(4, -2, 6, 8));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(5, 3));
        platform->setCurrentTangentialVelocity(sad::p2d::Vector(-2, 0.25));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().y(), 5.0));
        ASSERT_TRUE(player->currentShape()->center().x() >= -2.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 0.0);
    }

    /*! Tests inelastic collision from up
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision11()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(4, 0, 6, 2));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(5, 1));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(1, 2));


        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 4, 10, 6));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(5, 5));
        platform->setCurrentTangentialVelocity(sad::p2d::Vector(-1, -2));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().x(), 7.0));
        ASSERT_TRUE(player->currentShape()->center().y() >= -2.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 0.0);
    }

    /*! Tests inelastic collision from down
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollision12()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(4, 4, 6, 6));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(5, 5));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(1, -2));


        sad::p2d::Body* platform = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect = new sad::p2d::Rectangle();
        platform_rect->setRect(sad::Rect2D(0, 0, 10, 2));
        platform->setShape(platform_rect);
        platform->initPosition(sad::Point2D(5, 1));
        platform->setCurrentTangentialVelocity(sad::p2d::Vector(0.25, 2));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform->currentShape()));
        ASSERT_TRUE(sad::is_fuzzy_equal(player->currentShape()->center().x(), 7.0));
        ASSERT_TRUE(player->currentShape()->center().y() >= 6.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 8.0);
    }


} _test_bounce_solver;
