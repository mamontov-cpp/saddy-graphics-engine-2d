#include "renderways.h"

#include "../mainpanel.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include <renderer.h>

#include <p2d/app/way.h>


gui::RenderWays::RenderWays(core::Editor* editor)
: m_editor(editor),
m_init(false),
m_enabled(false),
m_default(0, 255, 255, 255),
m_red(255, 0, 0, 255)
{
    m_scene = new sad::Scene();
	m_default_sprite = new sad::Sprite2D();
	m_red_sprite = new sad::Sprite2D();
}

void gui::RenderWays::enable()
{
	m_enabled = true;
}

gui::RenderWays::~RenderWays()
{
	delete m_default_sprite;
	delete m_red_sprite;
    delete m_scene;
}

void gui::RenderWays::_process()
{
    m_scene->setRenderer(sad::Renderer::ref());
    if (m_editor->panel() == NULL || m_enabled == false)
    {
        return;
    }
	if (!m_init)
	{
		m_init = true;
		m_default_sprite->setTreeName(sad::Renderer::ref(), "icons");
        m_default_sprite->set("default");
		m_red_sprite->setTreeName(sad::Renderer::ref(), "icons");
        m_red_sprite->set("default-red");
	}
	sad::db::Database* db = sad::Renderer::ref()->database("");
    if (db && m_editor->isInWaysEditingState())
	{
        m_scene->camera().apply();
		sad::db::Table* t  = db->table("ways");
		if (t)
		{
			sad::Vector<sad::db::Object*> objects;
			t->objects(objects);
			for(size_t i = 0; i < objects.size(); i++)
			{
				sad::p2d::app::Way* way = static_cast<sad::p2d::app::Way*>(objects[i]);
				if (way->Active)
				{
					sad::Sprite2D * s = m_red_sprite;
					sad::AColor* c = &m_red;
					bool selected = false;
                    if (way == m_editor->shared()->selectedWay())
					{
						s = m_default_sprite;
						c = &m_default;
						selected = true;
					}
					const sad::Vector<sad::Point2D>& pts = way->wayPoints();
                    for(int i = 1; i < pts.size(); i++)
					{
						sad::Renderer::ref()->render()->line(pts[i-1], pts[i], *c);
					}
					if (way->closed() && pts.size() > 1)
					{
						sad::Renderer::ref()->render()->line(pts[0], pts[pts.size() - 1], *c);
					}                    
                    int currentrow = m_editor->panel()->UI()->lstWayPoints->currentRow();
					for(int i = 0; i < pts.size(); i++)
					{
						s->setMiddle(pts[i]);
						s->render();
                        if (selected && i == currentrow)
						{
							sad::Renderer::ref()->render()->rectangle(s->area(), *c);
						}                        
					}
				}
			}
		}
	}
}
