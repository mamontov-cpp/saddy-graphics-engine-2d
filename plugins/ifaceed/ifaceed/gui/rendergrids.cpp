#include "rendergrids.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include <renderer.h>

#include <db/dbdatabase.h>
#include <algorithm>

// ====================== PUBLIC METHODS ======================

gui::RenderGrids::RenderGrids(core::Editor* editor)
: m_editor(editor)
{
    m_enabled = false;
    m_scene = new sad::Scene();
}

void gui::RenderGrids::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void gui::RenderGrids::cleanup() const
{
    m_scene->clear();
}

struct grid_comparator_t
{
    bool operator()(sad::layouts::Grid* a, sad::layouts::Grid* b) const
    {				
        return a->cachedLayer() < b->cachedLayer();
    }	
};

void gui::RenderGrids::gridsFromTable(sad::db::Table* t, sad::Vector<sad::layouts::Grid*>& grids)
{
    grids.clear();
    if (t)
    {
        sad::Vector<sad::db::Object*> objects;
        t->objects(objects);
        sad::String gridname = "sad::layouts::Grid";
        for(size_t i = 0; i < objects.size(); i++)
        {
            sad::db::Object* object = objects[i];
            if (object->isInstanceOf(gridname))
            {
                if (object->Active)
                {
                    grids << static_cast<sad::layouts::Grid*>(object);
                }
            }
        }
        if (grids.size())
        {
            grid_comparator_t comparator;
            std::sort(grids.begin(), grids.end(), comparator);
        }
    }
}

void gui::RenderGrids::synchronizeWithDatabase() const
{
    this->cleanup();
    m_scene->setRenderer(sad::Renderer::ref());

    sad::db::Database* db = sad::Renderer::ref()->database("");
    sad::db::Table* t  = db->table("layouts");
    sad::Vector<sad::layouts::Grid*> grids;
    gui::RenderGrids::gridsFromTable(t, grids);
    for(size_t i = 0; i < grids.size(); i++)
    {
        grids[i]->setRenderColor(gui::RenderGrids::defaultColor());
        m_scene->add(grids[i]);
    }
}

void gui::RenderGrids::add(sad::layouts::Grid* g) const
{
    if (g)
    {
        m_scene->add(g);
    }
}

void gui::RenderGrids::insert(int pos, sad::layouts::Grid* g) const
{
    if (g)
    {
        sad::Renderer::ref()->lockRendering();
        m_scene->add(g);
        m_scene->setLayer(g, pos);
        sad::Renderer::ref()->unlockRendering();
    }
}

int gui::RenderGrids::find(sad::layouts::Grid* g) const
{
    return m_scene->findLayer(g);
}

int gui::RenderGrids::remove(sad::layouts::Grid* g) const
{
    int result = m_scene->findLayer(g);
    m_scene->remove(g);
    return result;
}

void gui::RenderGrids::swap(sad::layouts::Grid* g1, sad::layouts::Grid* g2) const
{
    m_scene->swapLayers(g1, g2);
}

const sad::AColor& gui::RenderGrids::selectedColor()
{
    return m_selected_color;    
}

const sad::AColor& gui::RenderGrids::defaultColor()
{
    return m_default_color;    
}

gui::RenderGrids::~RenderGrids()
{
    delete m_scene;
}

//============================== PROTECTED METHOD ==============================

sad::AColor gui::RenderGrids::m_default_color(255, 0, 0, 255);
sad::AColor gui::RenderGrids::m_selected_color(0, 255, 255, 255);

void gui::RenderGrids::_process()
{
    if (m_editor)
    {
         if (m_editor->isInWaysEditingState() == false)
         {
             m_scene->render();
         }
    }
}
