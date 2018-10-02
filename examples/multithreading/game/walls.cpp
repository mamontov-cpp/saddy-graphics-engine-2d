#include "walls.h"
#include <p2d/rectangle.h>

const int  game::Walls::LENGTH = 1000;

game::Walls::Walls() 
: m_left_bound(0), 
m_right_bound(0), 
m_top_bound(0),
m_bottom_bound(0),
m_left_wall(NULL),
m_right_wall(NULL),
m_bottom_wall(NULL),
m_top_wall(NULL)
{
    
}

game::Walls::Walls(const game::Walls& walls)
: m_left_bound(walls.m_left_bound),
m_right_bound(walls.m_right_bound),
m_top_bound(walls.m_top_bound),
m_bottom_bound(walls.m_bottom_bound),
m_left_wall(NULL),
m_right_wall(NULL),
m_bottom_wall(NULL),
m_top_wall(NULL)
{
    
}

game::Walls& game::Walls::operator=(const game::Walls& walls)
{
    m_left_bound = walls.m_left_bound;
    m_right_bound = walls.m_right_bound;
    m_top_bound = walls.m_top_bound;
    m_bottom_bound = walls.m_bottom_bound;

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
        m_left_wall->setShape(r);
        m_left_wall->initPosition((rect[0] + rect[2]) / 2.0);
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
        m_right_wall->setShape(r);
        m_left_wall->initPosition((rect[0] + rect[2]) / 2.0);
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
        m_bottom_wall->setShape(r);
        m_bottom_wall->initPosition((rect[0] + rect[2]) / 2.0);
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
        m_top_wall->setShape(r);
        m_top_wall->initPosition((rect[0] + rect[2]) / 2.0);
    }
    return *this;
}