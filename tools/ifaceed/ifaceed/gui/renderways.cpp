#include <QListWidget>

#include "renderways.h"

#include "../core/editor.h"
#include "../core/shared.h"

#include <renderer.h>
#include <geometry2d.h>

#include <db/dbdatabase.h>

#include <way.h>

#include "uiblocks/uiblocks.h"
#include "uiblocks/uiwayblock.h"

gui::RenderWays::RenderWays(core::Editor* editor)
: m_editor(editor),
m_init(false),
m_default(0, 255, 255),
m_red(255, 0, 0)
{
    m_enabled = false;
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
    if (m_editor->panel() == nullptr || m_enabled == false)
    {
        return;
    }
    if (!m_init)
    {
        m_init = true;
        m_default_sprite->setScene(m_scene);
        m_default_sprite->setTreeName(sad::Renderer::ref(), "icons");
        m_default_sprite->set("default");

        m_red_sprite->setScene(m_scene);
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
                sad::Way* way = static_cast<sad::Way*>(objects[i]);
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
                    for(int j = 1; j < pts.size(); j++)
                    {
                        this->renderArrow(m_scene, pts[j-1], pts[j], *c);
                    }
                    if (way->closed() && pts.size() > 1)
                    {
                        this->renderArrow(m_scene, pts[pts.size() - 1], pts[0], *c);
                    }  
                    gui::uiblocks::UIWayBlock* ui_way_block = m_editor->uiBlocks()->uiWayBlock();
                    int currentrow = ui_way_block->lstWayPoints->currentRow();
                    for(int j = 0; j < pts.size(); j++)
                    {
                        s->setMiddle(pts[j]);
                        s->render();
                        if (selected && j == currentrow)
                        {
                            sad::Renderer::ref()->render()->rectangle(m_scene, s->area(), *c);
                        }                        
                    }
                }
            }
        }
        m_scene->camera().restore();
    }
}

void gui::RenderWays::renderArrow(
    sad::Scene* scene,
    const sad::Point2D& begin, 
    const sad::Point2D& end, 
    const sad::AColor& c
)
{
    sad::PrimitiveRenderer* r = sad::Renderer::ref()->render();
    const double mindistance = 11.0; // !< A minimal distance - radius of point
    const double arrowpartlength = 5.0; //!< An arrow part
    if (begin.distance(end) > 2 * mindistance)
    {
        double angle = sad::angleOf(end.x() - begin.x(), end.y() - begin.y());
        sad::Point2D linepivot = end;
        linepivot.setX(linepivot.x() - mindistance * cos(angle));
        linepivot.setY(linepivot.y() - mindistance * sin(angle));
        double maxangle = angle + M_PI / 8.0, minangle = angle - M_PI / 8.0;

        sad::Point2D maxpoint(linepivot), minpoint(linepivot);
        maxpoint.setX(maxpoint.x() - arrowpartlength * cos(maxangle));
        maxpoint.setY(maxpoint.y() - arrowpartlength * sin(maxangle));

        minpoint.setX(minpoint.x() - arrowpartlength * cos(minangle));
        minpoint.setY(minpoint.y() - arrowpartlength * sin(minangle));

        r->line(scene, linepivot, maxpoint, c);
        r->line(scene, linepivot, minpoint, c);
        r->line(scene, begin, end, c);
    }
}
