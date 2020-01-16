#include "rendereditorgrid.h"

#include <renderer.h>
#include <camera.h>

// ========================= PUBLIC METHODS =========================

gui::RenderEditorGrid::RenderEditorGrid() : Color(0, 255, 255), GridSpaceX(0), GridSpaceY(0)
{
    this->Step::disable();
}

gui::RenderEditorGrid::~RenderEditorGrid()
{
    
}

bool gui::RenderEditorGrid::isEnabled() const
{
    return m_enabled;
}

// ========================= PROTECTED METHODS =========================

void gui::RenderEditorGrid::_process()
{
    sad::Renderer* r = sad::Renderer::ref();
    m_scene.setRenderer(r);
    m_scene.camera().apply();
    sad::Vector3D v = r->globalTranslationOffset();
    int startx = 0 - v.x();
    int endx = startx + r->settings().width();
    int starty = 0 - v.y();
    int endy = starty + r->settings().height();
    if (this->GridSpaceX > 1)
    {
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        int startxx = 0;
        if (startx < 0 )
        {
            startxx = startx + (startx % this->GridSpaceX) - (this->GridSpaceX);
        }
        else
        {
            startxx = startx - (startx % this->GridSpaceX);
        }
        int x = startxx;
        while(x < endx)
        {
            r->render()->line(&m_scene, sad::Point2D(x, starty), sad::Point2D(x, endy), this->Color);
            x += this->GridSpaceX;
        }
    }
    if (this->GridSpaceY > 1)
    {
        // ReSharper disable once CppInitializedValueIsAlwaysRewritten
        int startyy = 0;
        if (starty < 0 )
        {
            startyy = starty + (starty % this->GridSpaceY) - (this->GridSpaceY);
        }
        else
        {
            startyy = starty - (starty % this->GridSpaceY);
        }
        int y = startyy;
        while(y < endy)
        {
            r->render()->line(&m_scene, sad::Point2D(startx, y), sad::Point2D(endx, y), this->Color);
            y += this->GridSpaceY;
        }
    }
    m_scene.camera().restore();
}
