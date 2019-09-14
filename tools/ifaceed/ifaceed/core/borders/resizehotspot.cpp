#include "resizehotspot.h"

#include <renderer.h>

#include <geometry2d.h>

#include <p2d/vector.h>

core::borders::ResizeHotspot::ResizeHotspot(
    int pivot1, 
    int pivot2, 
    int adjacent, 
    const sad::p2d::Vector& p	
) : m_sprite_options_set(false),
m_pivot_pair(pivot1, pivot2),
m_adjacent_pair_index(adjacent),
m_default_vector(p)
{
    m_sprite = new sad::Sprite2D();
}

core::borders::ResizeHotspot::~ResizeHotspot()
{
    delete m_sprite;
}

void core::borders::ResizeHotspot::setScene(sad::Scene* scene) const
{
    m_sprite->setScene(scene);
}

void core::borders::ResizeHotspot::render(const sad::Rect2D & r)
{
    if (m_sprite_options_set == false)
    {
        m_sprite_options_set = true;
        m_sprite->setTreeName(sad::Renderer::ref(), "icons");
        m_sprite->set("directed");
    }
    sad::p2d::Vector direction = this->directionVector(r);
    double angle = atan2(direction.y(), direction.x());
    direction  *= core::borders::ResizeHotspot::Padding;
    

    sad::Point2D middle = r[m_pivot_pair.p1()] + r[m_pivot_pair.p2()];
    middle /= 2.0;
    middle += direction;
    m_sprite->setAngle(angle);
    m_sprite->setMiddle(middle);
    m_sprite->render();
}

bool core::borders::ResizeHotspot::isWithin(const sad::Point2D& p) const
{
    return sad::isWithin(p, m_sprite->renderableArea());
}

sad::p2d::Vector core::borders::ResizeHotspot::directionVector(
    const sad::Rect2D& r
) const
{
    sad::p2d::Vector vector = r[m_pivot_pair.p1()] - r[m_adjacent_pair_index];
    if (sad::is_fuzzy_zero(sad::p2d::modulo(vector)))
    {
        return m_default_vector;
    }
    sad::p2d::mutableUnit(vector);
    return vector;
}

sad::p2d::Vector core::borders::ResizeHotspot::defaultDirectionVector() const
{
    return m_default_vector;
}

const sad::Pair<int, int>& core::borders::ResizeHotspot::resizingIndexes() const
{
    return m_pivot_pair;	
}

const int core::borders::ResizeHotspot::Padding = 25;
