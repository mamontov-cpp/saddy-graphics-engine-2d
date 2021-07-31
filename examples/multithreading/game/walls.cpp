#include "walls.h"

#include <p2d/rectangle.h>
#include <p2d/world.h>

// ============================================== PUBLIC METHODS ==============================================

const int  game::Walls::LENGTH = 1000;

game::Walls::Walls() 
: m_left_bound(0), 
m_right_bound(0), 
m_top_bound(0),
m_bottom_bound(0),
m_left_wall(nullptr),
m_right_wall(nullptr),
m_bottom_wall(nullptr),
m_top_wall(nullptr)
{
    
}

game::Walls::Walls(const game::Walls& walls)
: m_left_bound(walls.m_left_bound),
m_right_bound(walls.m_right_bound),
m_top_bound(walls.m_top_bound),
m_bottom_bound(walls.m_bottom_bound),
m_left_wall(nullptr),
m_right_wall(nullptr),
m_bottom_wall(nullptr),
m_top_wall(nullptr)
{
    
}

game::Walls& game::Walls::operator=(const game::Walls& walls)
{
    m_left_bound = walls.m_left_bound;
    m_right_bound = walls.m_right_bound;
    m_top_bound = walls.m_top_bound;
    m_bottom_bound = walls.m_bottom_bound;

    this->initShapes();
    return *this;
}

game::Walls::~Walls()
{
    this->destroyOldShapes();
}


void game::Walls::init(double left, double right, double top, double bottom)
{
    m_left_bound = left;
    m_right_bound = right;
    m_top_bound = top;
    m_bottom_bound = bottom;

    this->destroyOldShapes();

    m_left_wall = new sad::p2d::Body();
    m_left_wall->addRef();
    m_right_wall = new sad::p2d::Body();
    m_right_wall->addRef();
    m_top_wall = new sad::p2d::Body();
    m_top_wall->addRef();
    m_bottom_wall = new sad::p2d::Body();
    m_bottom_wall->addRef();

    this->initShapes();
}

void game::Walls::free()
{
    this->destroyOldShapes();
}

sad::p2d::Body* game::Walls::leftWall() const
{
    return m_left_wall;
}

sad::p2d::Body* game::Walls::rightWall() const
{
    return m_right_wall;
}

sad::p2d::Body* game::Walls::topWall() const
{
    return m_top_wall;
}

sad::p2d::Body* game::Walls::bottomWall() const
{
    return m_bottom_wall;
}

void game::Walls::setLeftBound(double bound)
{
    m_left_bound = bound;
    initShapes();
}

void game::Walls::setRightBound(double bound)
{
    m_right_bound = bound;
    initShapes();
}

void game::Walls::setTopBound(double bound)
{
    m_top_bound = bound;
    initShapes();
}

void game::Walls::setBottomBound(double bound)
{
    m_top_bound = bound;
    initShapes();
}


void game::Walls::addToWorld(sad::p2d::World* world) const
{
    if (!world->doesGroupExists("walls"))
    {
        world->addGroup("walls");
    }
    world->addBodyToGroup("walls", m_left_wall);
    world->addBodyToGroup("walls", m_right_wall);
    world->addBodyToGroup("walls", m_top_wall);
    world->addBodyToGroup("walls", m_bottom_wall);
}

// ============================================== PRIVATE METHODS ==============================================

void game::Walls::initShapes() const
{
    if (m_left_wall)
    {
        sad::p2d::Rectangle* r = new sad::p2d::Rectangle();
        sad::Rect2D rect(
            m_left_bound - game::Walls::LENGTH,
            m_bottom_bound - game::Walls::LENGTH,
            m_left_bound,
            m_top_bound + game::Walls::LENGTH
        );
        r->setRect(rect);
        sad::Point2D center = (rect[0] + rect[2]) / 2.0;
        m_left_wall->setShape(r);
        m_left_wall->move(center - m_left_wall->currentShape()->center());
    }

    if (m_right_wall)
    {
        sad::p2d::Rectangle* r = new sad::p2d::Rectangle();
        sad::Rect2D rect(
            m_right_bound,
            m_bottom_bound - game::Walls::LENGTH,
            m_right_bound + game::Walls::LENGTH,
            m_top_bound + game::Walls::LENGTH
        );
        r->setRect(rect);
        sad::Point2D center = (rect[0] + rect[2]) / 2.0;
        m_right_wall->setShape(r);
        m_right_wall->move(center - m_right_wall->currentShape()->center());
    }

    if (m_bottom_wall)
    {
        sad::p2d::Rectangle* r = new sad::p2d::Rectangle();
        sad::Rect2D rect(
            m_left_bound - game::Walls::LENGTH,
            m_bottom_bound - game::Walls::LENGTH,
            m_right_bound + game::Walls::LENGTH,
            m_bottom_bound
        );
        r->setRect(rect);
        sad::Point2D center = (rect[0] + rect[2]) / 2.0;
        m_bottom_wall->setShape(r);
        m_bottom_wall->move(center - m_bottom_wall->currentShape()->center());
    }

    if (m_top_wall)
    {
        sad::p2d::Rectangle* r = new sad::p2d::Rectangle();
        sad::Rect2D rect(
            m_left_bound - game::Walls::LENGTH,
            m_top_bound,
            m_right_bound + game::Walls::LENGTH,
            m_top_bound + game::Walls::LENGTH
        );
        r->setRect(rect);
        sad::Point2D center = (rect[0] + rect[2]) / 2.0;
        m_top_wall->setShape(r);
        m_top_wall->move(center - m_top_wall->currentShape()->center());
    }
}

void game::Walls::destroyOldShapes()
{
    if (m_left_wall)
    {
        m_left_wall->delRef();
        m_left_wall = nullptr;
    }
    if (m_right_wall)
    {
        m_right_wall->delRef();
        m_right_wall = nullptr;
    }
    if (m_top_wall)
    {
        m_top_wall->delRef();
        m_top_wall = nullptr;
    }
    if (m_bottom_wall)
    {
        m_bottom_wall->delRef();
        m_bottom_wall = nullptr;
    }
}