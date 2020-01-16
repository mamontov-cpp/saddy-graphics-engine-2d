#include "activeborder.h"

#include <renderer.h>
#include <camera.h>

core::borders::ActiveBorder::ActiveBorder(core::Shared* data) : core::borders::Border(data)
{
    
}


core::borders::ActiveBorder::~ActiveBorder()
{
    
}

void core::borders::ActiveBorder::_process()
{
    sad::SceneNode * o = this->m_data->activeObject();
    if (o && this->m_data->canShowActiveBorder()) 
    {
        if (o->visible())
        {
            sad::Vector<sad::Rect2D> regions;
            o->regions(regions);
            o->scene()->camera().apply();
            sad::Renderer::ref()->render()
                                ->rectangle(o->scene(), regions[0], sad::AColor(255, 0, 0));
            o->scene()->camera().restore();
        }
    }
}

