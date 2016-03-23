#include "childrenprovider.h"

#include "mainpanelproxy.h"

#include "../core/editor.h"

#include "gui/actions/actions.h"

#include "gui/actions/scenenodeactions.h"

#include <renderer.h>
#include <scene.h>
#include <layouts/grid.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>


gui::ChildrenProvider::ChildrenProvider(MainPanelProxy* proxy) : m_proxy(proxy)
{

}

void gui::ChildrenProvider::setProxy(MainPanelProxy* p)
{
    m_proxy = p;
}

gui::ChildrenProvider::~ChildrenProvider()
{

}

QString gui::ChildrenProvider::name(sad::SceneNode* node)
{
    QString result;
    if (node && m_proxy)
    {
        gui::actions::SceneNodeActions* a = m_proxy->editor()->actions()->sceneNodeActions();
        result = a->fullNameForNode(node);
    }
    return result;
}

QVector<QPair<QString, unsigned long long> >  gui::ChildrenProvider::possibleChildren()
{
    QVector<QPair<QString, unsigned long long> > result;
    if (m_proxy)
    {
        const sad::Vector<sad::Scene*>& scenes = sad::Renderer::ref()->scenes();

        sad::Vector<sad::SceneNode*> nodes;
        size_t i;
        for(i = 0; i < scenes.size(); i++)
        {
            if (scenes[i]->Active)
            {
                nodes << scenes[i]->objects();
            }
        }

        for(i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->Active)
            {
                result << QPair<QString, unsigned long long>(
                    this->name(nodes[i]),
                    nodes[i]->MajorId
                );
            }
        }

        sad::db::Database* db = sad::Renderer::ref()->database("");
        sad::db::Table* tbl = db->table("layouts");

        gui::actions::SceneNodeActions* a = m_proxy->editor()->actions()->sceneNodeActions();
        sad::Vector<unsigned long long> already_children;
        QHash<unsigned long long, bool> already_children_hash;
        sad::layouts::Grid* g = m_proxy->editor()->shared()->selectedGrid();
        if (g)
        {
            already_children << g->MajorId;
        }
        if (tbl)
        {
            sad::Vector<sad::db::Object*> objs;
            tbl->objects(objs);
            for(i = 0; i < objs.size(); i++)
            {
                if (objs[i]->Active && objs[i]->isInstanceOf("sad::layouts::Grid"))
                {
                    result << QPair<QString, unsigned long long>(
                        a->viewableObjectName(objs[i]),
                        objs[i]->MajorId
                    );
                    already_children << static_cast<sad::layouts::Grid*>(objs[i])->childrenMajorIds();
                }
            }
        }
        // Erase non-addable items
        for(i = 0; i < already_children.size(); i++)
        {
            already_children_hash.insert(already_children[i], true);
        }
        for(i = 0; i < result.size(); i++)
        {
            if (already_children_hash.contains(result[i].second))
            {
                result.remove(i);
                --i;
            }
        }
    }
    return result;
}
