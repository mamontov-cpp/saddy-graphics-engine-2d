#include "activeborder.h"

#include <renderer.h>

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
        sad::Vector<sad::Rect2D> regions;
        o->regions(regions);
        sad::Renderer::ref()->render()
                            ->rectangle(regions[0], sad::AColor(255, 0, 0, 255));
    }
}

