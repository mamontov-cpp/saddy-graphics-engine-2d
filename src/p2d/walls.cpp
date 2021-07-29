#define NOMINMAX

#include <p2d/walls.h>
#include <p2d/bounds.h>
#include <p2d/weight.h>
#include <renderer.h>

DECLARE_SOBJ(sad::p2d::Walls);
DECLARE_SOBJ(sad::p2d::Wall);

// ============================== sad::p2d::Walls METHODS ==============================


sad::p2d::Walls::Walls(double padding) : m_padding(padding)
{
    this->makeWalls(
        sad::Renderer::ref()->settings().width(), 
        sad::Renderer::ref()->settings().height()
    );
}

sad::p2d::Walls::Walls(double width, double height, double padding) : m_padding(padding)
{
    this->makeWalls(width, height);
}

void sad::p2d::Walls::makeWalls(double width, double height)
{
    double w = width;
    double h = height;
    
    sad::Vector< sad::Pair<p2d::BoundType, double> > pairs;
    pairs << sad::Pair<p2d::BoundType, double>( sad::p2d::BoundType::BT_LEFT, -m_padding );
    pairs << sad::Pair<p2d::BoundType, double>( sad::p2d::BoundType::BT_RIGHT, w + m_padding);
    pairs << sad::Pair<p2d::BoundType, double>( sad::p2d::BoundType::BT_UP, h + m_padding );
    pairs << sad::Pair<p2d::BoundType, double>( sad::p2d::BoundType::BT_DOWN, -m_padding );

    for(size_t i = 0; i < pairs.size(); i++)
    {
        sad::p2d::Bound * bound = new p2d::Bound();
        bound->setPosition(pairs[i].p2());
        bound->setType(pairs[i].p1());
        
        sad::p2d::Body * b = new p2d::Body();
        b->setShape(bound);
        b->setWeight( p2d::Weight::infinite() );
        b->addRef();
        m_bodies << b;

        sad::p2d::Wall * wall = new sad::p2d::Wall(m_padding);
        b->setUserObject(wall);
        b->setFixed(true);
        wall->setBody(b);
        m_walls << wall;
    }

    m_walls[0]->setOppositeBody(m_walls[1]->body());
    m_walls[1]->setOppositeBody(m_walls[0]->body());
    m_walls[2]->setOppositeBody(m_walls[3]->body());
    m_walls[3]->setOppositeBody(m_walls[2]->body());

    for(size_t i = 0; i < m_walls.size(); i++)
    {
        m_walls[i]->addRef();
    }
}

sad::Vector<sad::p2d::Body *> sad::p2d::Walls::bodies() const
{
    return m_bodies;
}


sad::p2d::Walls::~Walls()
{
    for(size_t i = 0; i < m_walls.count(); i++)
    {
        m_walls[i]->delRef();
    }
    for (size_t i = 0; i < m_bodies.count(); i++)
    {
        m_bodies[i]->delRef();
    }
}

// ============================== sad::p2d::Wall METHODS ==============================

sad::p2d::Wall::Wall(double padding) : m_padding(padding)
{
    m_body = nullptr;
    m_opposite_body = nullptr;
}

void sad::p2d::Wall::tryTeleport(sad::p2d::Body * b)
{
    if (this->m_opposite_body == nullptr)
    {
        return;
    }
    sad::p2d::Bound * bound = static_cast<sad::p2d::Bound *>(this->m_opposite_body->currentShape());
    sad::p2d::Vector  n = bound->normal();
    sad::p2d::CollisionShape & s = b->at(b->timeStep());
    sad::p2d::Cutter1D projection = s.project(n);

    sad::p2d::Point P = bound->boundingLine().intersection(
                sad::p2d::InfiniteLine::appliedVector(s.center(), n)
             ).value();
    double On = std::min(projection.p1(), projection.p2());
    double O =  p2d::scalar(s.center(), n);

    P += n * (O - On + COLLISION_PRECISION);
    b->shedulePosition(P);
}

void sad::p2d::Wall::setBody(p2d::Body * b)
{
    if (m_body)
    {
        m_body->delRef();
    }
    m_body = b;
    if (b)
    {
        b->addRef();
    }
}

void sad::p2d::Wall::setOppositeBody(p2d::Body * b)
{
    if (m_opposite_body)
    {
        m_opposite_body->delRef();
    }
    m_opposite_body = b;
    if (b)
    {
        b->addRef();
    }
}

sad::p2d::BoundType  sad::p2d::Wall::type() const
{
    if (this->m_body == nullptr)
    {
        return sad::p2d::BoundType::BT_LEFT;
    }
    return  static_cast<p2d::Bound *>(this->body()->currentShape())
            ->type();
}

int sad::p2d::Wall::typeAsIntegralValue() const
{
    return static_cast<int>(this->type());
}

sad::p2d::Wall::~Wall()
{
    if (m_body)
    {
        m_body->delRef();
    }
    if (m_opposite_body)
    {
        m_opposite_body->delRef();
    }
}

