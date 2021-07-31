/*! \file layoutschange.h


    Defines a generic change command for layouts
 */
#pragma once
#include "../command.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"
#include "../../gui/actions/scenenodeactions.h"


#include <3rdparty/picojson/picojson.h>

#include <sadvector.h>

#include <layouts/grid.h>

namespace history
{

namespace layouts
{
/*! Describes a generic change in layouts
 */
template<
    gui::actions::GridActions::GridUpdateOptions O
>
class Change: public history::Command
{
public:
    /*! Creates new command for specified grid
        \param[in] g grid
     */
    Change(sad::layouts::Grid* g) 
    : m_grid(g), 
      m_old_state(picojson::object_type, false),
      m_new_state(picojson::object_type, false),
      m_should_update_children(false),
      m_should_update_cells(false)
    {
        m_grid->addRef();        
    }
    /*! Destroys all linked data
     */
    virtual ~Change() override
    {
        m_grid->delRef();
        for(size_t i = 0; i < m_affected_nodes.size(); i++)
        {
            m_affected_nodes[i]->delRef();
        }
    }
    /*! Adds new affected node to list
        \param[in] node a node list
     */
    void addAffectedNode(sad::SceneNode* node)
    {
        node->addRef();
        m_affected_nodes << node;
    }
    /*! Adds list of affected nodes to list
        \param[in] nodes a node list
     */
    void addAffectedNodes(const sad::Vector<sad::SceneNode*>& nodes)
    {
        for(size_t i = 0; i < nodes.size(); i++)
        {
            nodes[i]->addRef();
        }
        m_affected_nodes << nodes;
    }
    /*! Saves old state for a grid
        \param[in] v value
     */
    void saveOldState(const picojson::value& v)
    {
        m_old_state = v;
    }
    /*! Saves old state for a grid
     */
    void saveOldState()
    {
        m_grid->save(m_old_state);
    }

    /*! Saves state to a new state for a grid
     */
    void saveNewState()
    {
        m_grid->save(m_new_state);
    }
    /*! Saves new state for a grid
        \param[in] v value
     */
    void saveNewState(const picojson::value& v)
    {
        m_new_state = v;
    }
     /*! Applies new saved state, described in command
         \param[in] ob an editor
      */
    virtual void commit(core::Editor * ob = nullptr) override
    {
       if (!ob)
       {
           return;
       }
       m_grid->load(m_new_state);
       m_grid->update();
       tryUpdateUI(ob);
    }
     /*! Reverts to old saved state, described in command
         \param[in] ob an editor
      */
    virtual void rollback(core::Editor * ob = nullptr) override
    {
        if (!ob)
        {
            return;
        }
       m_grid->load(m_old_state);
       m_grid->update();
       tryUpdateUI(ob);
    }
    /*! Mark, as amount of children could change during update 
        of cell. Also, forces update cells, rows and columns, when toggled
     */
    void markAsChangingChildrenList()
    {
        m_should_update_children = true;
    }
    /*! Marks, whether command application should update cells
        \param[in] v value
     */
    void toggleWhetherShouldUpdateCells(bool v)
    {
        m_should_update_cells = v;
    }
protected:
    /*! Tries to update UI
        \param[in] e editor
     */
    void tryUpdateUI(core::Editor* e)
    {
        if (!e)
            return;
        gui::actions::GridActions* actions = e->actions()->gridActions();
        if (m_should_update_children)
        {
            for(size_t i = 0; i < m_affected_nodes.size(); i++)
            {
                if (m_grid->find(m_affected_nodes[i]).exists())
                {
                    actions->insertNodeToGridEntry(m_affected_nodes[i], m_grid);
                }
                else
                {
                    actions->eraseNodeToGridEntry(m_affected_nodes[i]);
                }
            }
        }
        if (e->shared()->selectedGrid() == m_grid)
        {
            if (m_should_update_children)
            {
                actions->updateOnlyGridPropertiesInUI(gui::actions::GridActions::GridUpdateOptions::GGAUO_Cells);
            }

            actions->updateOnlyGridPropertiesInUI(O);
            actions->updateRegion();
            if (m_should_update_cells)
            {
                actions->updateCellBrowser();
            }
        }

        actions->updateParentGridsRecursively(m_grid, false);
        for(size_t i = 0; i < m_affected_nodes.size(); i++)
        {
            if (e->shared()->selectedObject() == m_affected_nodes[i])
            {
                e->actions()->sceneNodeActions()->updateUIForSelectedSceneNode();
            }
        }
    }
    /*! An affected grid
     */
    sad::layouts::Grid* m_grid;
    /*! A list of affected nodes by grid change
     */
    sad::Vector<sad::SceneNode*> m_affected_nodes;
    /*! An old state for layout change
     */
    picojson::value m_old_state;
    /*! A new state for layout change
     */
    picojson::value m_new_state;
    /*! Whether we should update children list, belonging to grid
     */
    bool m_should_update_children;
    /*! Whether we should update cell list
     */
    bool m_should_update_cells;
};

}

}

