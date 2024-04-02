#include "world.h"
#include "platform.h"
#include "uncoloredbullet.h"
#include "coloredbullet.h"
#include "ball.h"
#include "gridnode.h"
#include "gridnodedge.h"
#include "shooter.h"

#include <orthographiccamera.h>
#include <keymouseconditions.h>
#include <p2d/elasticforce.h>
#include <p2d/app/object.h>
#include <p2d/app/objectemitter.h>
#include <pipeline/pipeline.h>
#include <input/controls.h>

World::World()
{
    this->initApp();
    m_walls = new sad::p2d::Walls();
    m_find = new sad::p2d::FindContactPoints();
    m_solver = new sad::p2d::BounceSolver();
    m_hit_count = 0;
    m_way = new sad::p2d::app::Way();
}


World::~World()
{
    delete m_walls;
    delete m_find;
    delete m_solver;
    delete m_way;
}


void World::toggleFullscreen()
{
    sad::Renderer::ref()->toggleFullscreen();
}


void World::run()
{
    // Init new empty scene with orthographic projection
    sad::Scene * sc = new sad::Scene();

    sad::Sprite2D * background = new sad::Sprite2D(
        "background", 
        sad::Rect2D(0, 0, 512, 512),
        sad::Rect2D(0, 0, 800, 600)
    );
    sc->add(background);


    sad::Renderer::ref()->setScene(sc);
    sad::Renderer::ref()->setWindowTitle("Physics stress test");

    sad::Renderer::ref()->controls()
                        ->add(*sad::input::EventType::ET_KeyPress & sad::KeyboardKey::F, this, &World::toggleFullscreen);
    
    // Set window size to be fixed
    sad::Renderer::ref()->makeFixedSize();

    m_step_task = new sad::p2d::WorldStepTask(m_world);
    sad::Renderer::ref()->pipeline()->append(m_step_task);


    // SETUP WORLD CALLBACKS HERE!!!
    m_world->addHandler(this, &World::performBounce<sad::p2d::Wall,GridNode>);
    m_world->addHandler(this, &World::performBounce<GridNode,GridNode>);
    m_world->addHandler(this, &World::onWallBall);
    m_world->addHandler(this, &World::onBallNode);
    m_world->addHandler(this, &World::removeFirst<UncoloredBullet, sad::p2d::Wall>);
    m_world->addHandler(this, &World::removeFirst<UncoloredBullet, Ball>);
    m_world->addHandler(this, &World::performBounce<GridNode,UncoloredBullet>);
    m_world->addHandler(this, &World::onBallPlatform);
    m_world->addHandler(this, &World::performBounce<sad::p2d::Wall,Platform>);
    m_world->addHandler(this, &World::performBounce<sad::p2d::Wall,Shooter>);
    m_world->addHandler(this, &World::incrementHitCount);
    m_world->addHandler(this, &World::removeFirst<ColoredBullet, sad::p2d::Wall>);
    m_world->addHandler(this, &World::eraseBullets);
    m_world->addHandler(this, &World::boostBall);
    m_world->addHandler(this, &World::removeFirst<MovingSmile, Ball>);


    // Add walls
    sad::Vector<sad::p2d::Body *> bodies = m_walls->bodies();
    for(size_t i = 0; i < bodies.count(); i++)
    {
        m_world->addBody(bodies[i]);
    }

    // Build elastic grid with 9 nodes

    // Produce a nodes of grids
    GridNode * g[9];
    for(int i = 0; i < 9; i++)
    {
        g[i] = new GridNode();
        g[i]->setPosition(sad::p2d::Point(300 + (i % 3) * 100, 400 - (i / 3) * 100));
        // Add a gravity force
        if (i != 0 && i != 2) 
        {
            g[i]->body()->addForce( new sad::p2d::TangentialForce(sad::p2d::Vector(0, -60) ) );
        }
        else
        {
            // Make bodies unmovable
            g[i]->body()->setFixed(true);
        }
    }

    // Sets forces, between nodes	
    int forces[20][2] =  {
        {0, 1}, {2, 1}, {4, 1},
        {0, 3}, {4, 3}, {6, 3},
        {3, 4}, {1, 4}, {5, 4}, {7, 4},
        {2, 5}, {4, 5}, {8, 5},
        {3, 6}, {7, 6},
        {6, 7}, {4, 7}, {8, 7},
        {5, 8}, {7, 8}
    };
    for(int i = 0; i < 20; i++)
    {
        int  f = forces[i][0];
        int  s = forces[i][1];
        g[s]->addTangentialForce( new sad::p2d::ElasticForce(g[s]->body(), g[f]->body()) );
    }
    int graphic[12][2] = { 
        {0, 1}, {1, 2}, {0, 3}, {1, 4}, {3, 4}, {2, 5},
        {4, 5}, {3, 6}, {4, 7}, {6, 7}, {5, 8}, {7, 8}
    };
    // Build grafical representation for elastic grid
    for(int i = 0; i < 12; i++)
    {
        sc->add( new GridNodeEdge(g[graphic[i][0]]->body(), g[graphic[i][1]]->body()) );
    }
    
    // Add elastic grid nodes to scene
    for(int i = 0 ; i < 9; i++)
    {
        this->addObject(g[i]);
    }


    // Add two platforms to scene
    Platform * platform1 = new Platform();
    platform1->setPosition(sad::p2d::Point(150, 300));
    platform1->body()->setWeight(sad::p2d::Weight::infinite());
    platform1->setTangentialVelocity(sad::p2d::Vector(0, -5));
    this->addObject(platform1);

    Platform * platform2 = new Platform();
    platform2->setPosition(sad::p2d::Point(650, 200));
    platform2->body()->setWeight(sad::p2d::Weight::infinite());
    platform2->setTangentialVelocity(sad::p2d::Vector(0, -5));
    this->addObject(platform2);

    // Add ball to scene
    Ball * ball = new Ball();
    ball->setPosition(sad::p2d::Point(40, 200));
    ball->body()->tangentialForces().add( new sad::p2d::TangentialForce(sad::p2d::Vector(0, -30) ) );
    ball->body()->setCurrentTangentialVelocity(sad::p2d::Vector(80, 180));
    ball->body()->setCurrentAngularVelocity(1.0);
    this->addObject(ball);


    // Add shooter to scene
    Shooter * shooter = new Shooter();
    shooter->setPosition(sad::p2d::Point(400, 21));
    shooter->body()->setCurrentTangentialVelocity(sad::p2d::Vector(60, 0));
    shooter->body()->setCurrentAngle(M_PI / 2);
    this->addObject(shooter);
    

    // Add FPS and hit counters label
    sad::FormattedLabel * label = new sad::FormattedLabel();
    label->setFont("font");
    label->setSize(30);
    label->setPoint(0, sad::Renderer::ref()->settings().height());
    label->setFormatString("Shooter hit: {0} FPS: {1}");
    label->setUpdateInterval(1000.0);
    label->arg(this, &World::hitCount);
    label->argFPS();
    sc->add( label );

    typedef sad::p2d::app::RandomDirectionObjectEmitter<UncoloredBullet> Emitter;
    Emitter * b = new Emitter(this);
    // Set emitting task at upper middle of screen
    b->setMinPosition(sad::p2d::Vector(400, 570));
    b->setMaxPosition(sad::p2d::Vector(400, 570));

    b->setMaxAngularVelocity(1.0);
    b->setMinAngularVelocity(0.0);
    
    b->setMinSpeed(sad::p2d::Vector(-40, -100));
    b->setMaxSpeed(sad::p2d::Vector(40, -100));
    
    b->setInterval(200);
    b->setMaxCount(3);


    double total = 10.0;
    m_way->startConstruction();
    m_way->setTotalTime(total);
    m_way->makeClosed();
    double angle = M_PI / 6;
    for(int i = 0; i < 12; i++)
    {
        m_way->addPoint( sad::p2d::Point(400.0 + 200 * cos(angle * i), 300 + 200 * sin(angle * i)) );
    }
    m_way->construct();


    double timestep = total / 12 ;
    for(int i = 0; i < 12; i++)
    {
        MovingSmile * smile = new MovingSmile(m_way);
        smile->setPosition(sad::p2d::Point(600.0, 300));
        smile->step(timestep * i);
        this->addObject(smile);
    }


    // Added periodical task
    shooter->startShooting();
    
    sad::Renderer::ref()->pipeline()->append( new sad::PeriodicalEventPollProcess(b) );
    // Run an engine, starting a main loop
    SL_MESSAGE("Will start now");

    m_solver->enableDebug();
    sad::Renderer::ref()->run();
}


void World::quit()
{
    sad::Renderer::ref()->quit();
}

void World::onBallPlatform(const sad::p2d::CollisionEvent<Ball, Platform> & ev)
{
    m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
#ifdef PHYSICS_ENGINE_DEBUG
    ev.m_object_1->log()->debug(std::string("Ball to Platform\n") + m_solver->dump());
#endif
}

void World::onWallBall(const sad::p2d::CollisionEvent<Ball, sad::p2d::Wall> & ev)
{
    m_solver->pushRotationFriction(0.0001, 1);
    m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
#ifdef PHYSICS_ENGINE_DEBUG
    ev.m_object_1->log()->debug(std::string("Ball to Wall\n") + m_solver->dump());
#endif
}

void World::onBallNode(const sad::p2d::CollisionEvent<Ball, GridNode> & ev)
{
    m_solver->pushRotationFriction(0.0001, 1);
    m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
#ifdef PHYSICS_ENGINE_DEBUG
    ev.m_object_1->log()->debug(std::string("Ball to GridNode\n") + m_solver->dump());
#endif
}


int World::hitCount()
{
    return m_hit_count;
}

void World::incrementHitCount(const sad::p2d::CollisionEvent<UncoloredBullet, Shooter> & ev)
{
    this->removeObject(ev.m_object_1);
    ++m_hit_count;
}

void World::eraseBullets(const sad::p2d::CollisionEvent<ColoredBullet, UncoloredBullet> & ev)
{
    this->removeObject(ev.m_object_1);
    this->removeObject(ev.m_object_2);
}

void World::boostBall(const sad::p2d::CollisionEvent<ColoredBullet, Ball> & ev) 
{
    // If tangential velocity will change, ignore bullet, because it
    // could lead to worse work with collisions
    if (ev.m_object_2->body()->willTangentialVelocityChange() == false)
    {
        sad::p2d::Vector v = ev.m_object_2->body()->tangentialVelocity() 
                           * ev.m_object_2->body()->weight().value();
        v += ev.m_object_1->body()->tangentialVelocity() 
           * ev.m_object_1->body()->weight().value();
        v /= ev.m_object_2->body()->weight().value();
        ev.m_object_2->body()->sheduleTangentialVelocity(v);
    }

    this->removeObject(ev.m_object_1);
}
