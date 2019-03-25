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
     TEST(BounceSolverTest::testInelasticLeftCollision12),
     TEST(BounceSolverTest::testInelasticLeftCollisionComplex1),
     TEST(BounceSolverTest::testInelasticLeftCollisionComplex2),
     TEST(BounceSolverTest::testInelasticLeftCollisionEverythingMoving),
     TEST(BounceSolverTest::testInelasticLeftCollisionEverythingSpecial1),
     TEST(BounceSolverTest::testInelasticLeftCollisionEverythingSpecial2),
     TEST(BounceSolverTest::testInelasticLeftCollisionEverythingSpecial3)
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
        ASSERT_TRUE( pos.x() > 10.9 );
        ASSERT_TRUE( pos.y() > 10.9 );
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
        ASSERT_TRUE(player->currentShape()->center().y() >=  4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 6.0);
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
        ASSERT_TRUE(player->currentShape()->center().x() >= 6.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 8.0);
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
        ASSERT_TRUE(player->currentShape()->center().x() >= 6.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 8.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 6.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 8.0);
    }

    /*! Tests inelastic collision from a data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionComplex1()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(4, 5, 6, 7));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(5, 6));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-4, -3));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(0, 0, 2, 7));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(1, 3.5));

        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(2, 0, 9, 2));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(5.5, 1));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform1->currentShape()));
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform2->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 2.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 2.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 4.0);
    }


    /*! Tests inelastic collision from a data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionComplex2()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(4, 5, 6, 7));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(5, 6));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-3, -5));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(0, 0, 2, 7));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(1, 3.5));

        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(2, 0, 9, 2));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(5.5, 1));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        world.addHandler("1", "2", [&solver](const sad::p2d::BasicCollisionEvent& ev) {
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        world.step(0.5);
        world.step(0.5);
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform1->currentShape()));
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform2->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 2.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 2.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 4.0);
    }

    /*! Tests inelastic collision from a data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionEverythingMoving()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(5, 5, 7, 7));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(6, 6));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-3, -3));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(-1, 0, 1, 7));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(0, 3.5));
        platform1->setCurrentTangentialVelocity(sad::p2d::Vector(1, 0));


        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(0, -1, 9, 1));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(5.5, 0));
        platform2->setCurrentTangentialVelocity(sad::p2d::Vector(0, 1));

        double t = 0;
        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        world.addHandler("1", "2", [&t, &solver](const sad::p2d::BasicCollisionEvent& ev) {
            std::cout << t << "\n";
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(0.5);
        t += 0.5;
        world.step(0.5);
        t += 0.5;
        world.step(0.5);
        t += 0.5;
        world.step(0.5);
        t += 0.5;
        world.step(0.5);
        t += 0.5;
        world.step(0.5);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform1->currentShape()));
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform2->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 6.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 6.0);
    }

    /*! Tests inelastic collision from a data
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionEverythingSpecial1()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(3, 3, 6, 6));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(4.5, 4.5));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-3, -2));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(-1, 0, 1, 7));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(0, 3.5));
        platform1->setCurrentTangentialVelocity(sad::p2d::Vector(1, 0));


        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(0, -1, 9, 1));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(5.5, 0));
        platform2->setCurrentTangentialVelocity(sad::p2d::Vector(0, 1));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        double t = 0;
        world.addHandler("1", "2", [&t, &solver](const sad::p2d::BasicCollisionEvent& ev) {
            std::cout << t << "\n";
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1.0);
        t += 1.0;
        world.step(1.0);
        t += 1.0;
        world.step(1.0);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform1->currentShape()));
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform2->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 7.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 7.0);
    }

    /*! Tests inelastic collision from a data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionEverythingSpecial2()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(3, 3, 6, 6));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(4.5, 4.5));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-2, -3));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(-1, 0, 1, 7));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(0, 3.5));
        platform1->setCurrentTangentialVelocity(sad::p2d::Vector(1, 0));


        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(0, -1, 9, 1));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(5.5, 0));
        platform2->setCurrentTangentialVelocity(sad::p2d::Vector(0, 1));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        double t = 0;
        world.addHandler("1", "2", [&t, &solver](const sad::p2d::BasicCollisionEvent& ev) {
            std::cout << t << "\n";
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });

        world.step(1.0);
        t += 1.0;
        world.step(1.0);
        t += 1.0;
        world.step(1.0);

        sad::p2d::CollisionTest test;
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform1->currentShape()));
        ASSERT_TRUE(!test.invoke(player->currentShape(), platform2->currentShape()));
        ASSERT_TRUE(player->currentShape()->center().x() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 7.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 4.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 7.0);
    }

    /*! Tests inelastic collision from a data
     */
     // ReSharper disable once CppMemberFunctionMayBeStatic
     // ReSharper disable once CppMemberFunctionMayBeConst
    void testInelasticLeftCollisionEverythingSpecial3()
    {
        sad::p2d::BounceSolver solver;
        solver.setInelasticCollisionType(sad::p2d::BounceSolver::ICT_FIRST);
        solver.toggleInelasticCollisions(true);
        solver.toggleIgnoreContactPoints(true);

        sad::p2d::Body* player = new sad::p2d::Body();
        sad::p2d::Rectangle* player_rect = new sad::p2d::Rectangle();
        player_rect->setRect(sad::Rect2D(4, 2, 6, 4));
        player->setShape(player_rect);
        player->initPosition(sad::Point2D(5, 3));
        player->setCurrentTangentialVelocity(sad::p2d::Vector(-2, 0));


        sad::p2d::Body* platform1 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect1 = new sad::p2d::Rectangle();
        platform_rect1->setRect(sad::Rect2D(0, 0, 2, 6));
        platform1->setShape(platform_rect1);
        platform1->initPosition(sad::Point2D(1, 3));
        platform1->setCurrentTangentialVelocity(sad::p2d::Vector(2, 0));


        sad::p2d::Body* platform2 = new sad::p2d::Body();
        sad::p2d::Rectangle* platform_rect2 = new sad::p2d::Rectangle();
        platform_rect2->setRect(sad::Rect2D(-2, 0, 0, 6));
        platform2->setShape(platform_rect2);
        platform2->initPosition(sad::Point2D(-1, 3));
        platform2->setCurrentTangentialVelocity(sad::p2d::Vector(4, 0));


        sad::p2d::World world;
        world.addGroup("1");
        world.addGroup("2");
        world.addBodyToGroup("1", player);
        world.addBodyToGroup("2", platform1);
        world.addBodyToGroup("2", platform2);
        double t = 0;
        world.addHandler("1", "2", [&t, &solver](const sad::p2d::BasicCollisionEvent& ev) {
            std::cout << t << "\n";
            solver.bounce(ev.m_object_1, ev.m_object_2);
        });


        sad::p2d::CollisionTest test;

        world.step(1.0);
        t += 1.0;
        for (size_t i = 0; i < 32; i++) {
            world.step(1.0 / 32.0);
            bool not_colliding = !test.invoke(player->currentShape(), platform1->currentShape());
            if (!not_colliding)
            {
                printf("Collision between player and first platform: %lf\n", t);
            }
            ASSERT_TRUE(not_colliding);
            not_colliding = !test.invoke(player->currentShape(), platform2->currentShape());
            if (!not_colliding)
            {
                printf("Collision between player and second platform: %lf\n", t);
            }
            ASSERT_TRUE(not_colliding);
            t += 1.0 / 32.0;
        }

        for (size_t i = 0; i < 32; i++) {
            world.step(1.0 / 32.0);
            bool not_colliding = !test.invoke(player->currentShape(), platform1->currentShape());
            if (!not_colliding)
            {
                printf("Collision between player and first platform: %lf\n", t);
            }
            ASSERT_TRUE(not_colliding);
            not_colliding = !test.invoke(player->currentShape(), platform2->currentShape());
            if (!not_colliding)
            {
                printf("Collision between player and second platform: %lf\n", t);
            }
            ASSERT_TRUE(not_colliding);
            t += 1.0 / 32.0;
        }
        
        ASSERT_TRUE(player->currentShape()->center().x() >= 12.0);
        ASSERT_TRUE(player->currentShape()->center().x() <= 14.0);
        ASSERT_TRUE(player->currentShape()->center().y() >= 2.0);
        ASSERT_TRUE(player->currentShape()->center().y() <= 4.0);
    }

} _test_bounce_solver;
