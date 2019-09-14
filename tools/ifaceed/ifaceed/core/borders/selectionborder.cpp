#include "selectionborder.h"

#include <renderer.h>
#include <camera.h>



core::borders::SelectionBorder::SelectionBorder(core::Shared* data) : core::borders::Border(data), m_show_hotspots(true)
{
    m_delete_hotspot = new core::borders::DeleteHotspot();
    m_resize_hotspots[0] = new core::borders::ResizeHotspot(0, 1, 3, sad::p2d::Vector(0, -1));
    m_resize_hotspots[1] = new core::borders::ResizeHotspot(1, 2, 0, sad::p2d::Vector(1, 0));
    m_resize_hotspots[2] = new core::borders::ResizeHotspot(2, 3, 1, sad::p2d::Vector(0, 1));
    m_resize_hotspots[3] = new core::borders::ResizeHotspot(3, 0, 2, sad::p2d::Vector(-1, 0));
}


core::borders::SelectionBorder::~SelectionBorder()
{
    delete m_delete_hotspot;
    for(size_t i = 0; i < 4; i++)
    {
        delete m_resize_hotspots[i];
    }
}

void core::borders::SelectionBorder::toggleShowHotspot(bool flag)
{
    m_show_hotspots = flag;
}

core::borders::DeleteHotspot* core::borders::SelectionBorder::deleteHotspot()
{
    return m_delete_hotspot;
}

core::borders::ResizeHotspot* core::borders::SelectionBorder::selectedResizeHotspot(
    const sad::Point2D& p
)
{
    if (this->m_data->isSelectionResizeable() == false)
    {
        return NULL;
    }
    for(size_t i = 0; i < 4; i++)
    {
        if (m_resize_hotspots[i]->isWithin(p))
        {
            return m_resize_hotspots[i];
        }
    }
    return NULL;
}

void core::borders::SelectionBorder::_process()
{
    sad::SceneNode * o = this->m_data->selectedObject();
    if (o) 
    {
        o->scene()->camera().apply();

        sad::Vector<sad::Rect2D> regions;
        o->regions(regions);
        sad::Renderer::ref()->render()
                            ->rectangle(o->scene(), regions[0], sad::AColor(0, 255, 255));
        if (m_show_hotspots)
        {
            m_delete_hotspot->setScene(o->scene());
            m_delete_hotspot->render(regions[0]);
            if (this->m_data->isSelectionResizeable())
            {
                for(size_t i = 0; i < 4; i++)
                {
                    m_resize_hotspots[i]->setScene(o->scene());
                    m_resize_hotspots[i]->render(regions[0]);
                }
            }
        }

        o->scene()->camera().restore();
    }
}

