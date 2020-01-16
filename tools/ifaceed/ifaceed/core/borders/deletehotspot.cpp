#include "deletehotspot.h"

#include <renderer.h>

#include <geometry2d.h>

#include <p2d/vector.h>

core::borders::DeleteHotspot::DeleteHotspot() : m_sprite_options_set(false)
{
    m_sprite = new sad::Sprite2D();
}

core::borders::DeleteHotspot::~DeleteHotspot()
{
    delete m_sprite;
}

// A tested points, and points, adjacent to them
static const int HotSpotPointsAndAdjacentPoints[4][3] = {
    {0, 1, 3},
    {1, 0, 2},
    {2, 1, 3},
    {3, 0, 2}
};

void core::borders::DeleteHotspot::setScene(sad::Scene* scene) const
{
    this->m_sprite->setScene(scene);
}

void core::borders::DeleteHotspot::render(const sad::Rect2D & r)
{
    if (m_sprite_options_set == false)
    {
        m_sprite_options_set = true;
        m_sprite->setTreeName(sad::Renderer::ref(), "icons");
        m_sprite->set("delete");
    }
    // Select most right and most top point index
    int pointindex = 0;
    for(size_t i = 1; i < 4; i++)
    {
        if ((r[i].y() > r[pointindex].y()) && !sad::is_fuzzy_equal(r[i].y(), r[pointindex].y()))
        {
            pointindex = i;
        }
    }
    
    // Compute sum of vectors from point to adjacent point
    sad::p2d::Vector v1 = r[HotSpotPointsAndAdjacentPoints[pointindex][0]] - r[HotSpotPointsAndAdjacentPoints[pointindex][1]];
    sad::p2d::Vector v2 = r[HotSpotPointsAndAdjacentPoints[pointindex][0]] - r[HotSpotPointsAndAdjacentPoints[pointindex][2]];
    
    sad::p2d::Vector sum = v1 + v2;
    sad::p2d::mutableUnit(sum);
    sum *= core::borders::DeleteHotspot::Padding;

    m_sprite->setMiddle(r[pointindex] + sum);
    m_sprite->render();
}

bool core::borders::DeleteHotspot::isWithin(const sad::Point2D& p) const
{
    return sad::isWithin(p, m_sprite->renderableArea());
}


const int core::borders::DeleteHotspot::Padding = 25;