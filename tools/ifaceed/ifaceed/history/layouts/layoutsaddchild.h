/*! \file layoutsaddchild.h

    Describes a command of adding child to grid's cell
 */
#pragma once
#include "../command.h"
#include "layouts/grid.h"


namespace history
{

namespace layouts
{
/*! A command, which must be added, when user adds child
    to grid's cell
 */
class AddChild: public history::Command
{
public:
    /*! Constructs new command for node
        \param[in] d a node
        \param[in] row a row for node
        \param[in] column a column for node
        \param[in] node a node to be added
        \param[in] old_area an old area for layout
    */
    AddChild(
        sad::layouts::Grid* d,
        size_t row,
        size_t column,
        sad::SceneNode* node,
        const sad::Rect2D& old_area
    );
    /*! Erases link to a node
     */
    virtual ~AddChild();
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
    /*! Commits a change without updating UI. In that case,
        it changes rows of cell, without actually removing elements

        \param[in] e editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor * e) override;
protected:
    /*! Really performs commit
        \param[in] ob object of editor
     */
    void _commit(core::Editor* ob);
    /*! Really performs rolling back
        \param[in] ob object of editor
     */
    void _rollback(core::Editor* ob);
    /*! A real implementation for commiting without updating UI
        \param[in] e editor
     */
    void _commitWithoutUpdatingUI(core::Editor * e);
    /*! A node, whose name is changed
     */
    sad::layouts::Grid * m_grid;
    /*! A row of node
     */
    size_t m_row;
    /*! A column of node
     */
    size_t m_column;
    /*! An old value of property
     */
    sad::SceneNode* m_node;
    /*! A new value of property
     */
    sad::Rect2D m_old_area;
};

}

}
