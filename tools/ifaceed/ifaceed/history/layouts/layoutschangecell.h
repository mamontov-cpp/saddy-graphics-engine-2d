/*! \file layoutschangecell.h


    Defines a generic change command for cell of layout grid
 */
#pragma once
#include "../command.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/gridactions.h"

#include <layouts/grid.h>

#include <db/dbvariant.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <db/load.h>

namespace history
{

namespace layouts
{
/*! Defines a generic change command for cell of layout grid
 */
template<
    gui::actions::GridActions::CellUpdateOptions O,
    typename PropType
>
class ChangeCell: public history::Command
{
public:
    /*! Creates new command for specified cell
        \param[in] g parent grid for cell
        \param[in] row a row value
        \param[in] column a column value
        \param[in] property_name a property string name
     */
    ChangeCell(sad::layouts::Grid* g, size_t row, size_t column, const sad::String& property_name) 
    : m_grid(g), 
      m_row(row),
      m_column(column),
      m_property_name(property_name),
      m_could_update_area(false)
    {
        m_grid->addRef();        
    }
    /*! Destroys all linked data
     */
    virtual ~ChangeCell()
    {
        m_grid->delRef();
    }
    /*! Sets old value for property
        \param[in] v value
     */
    void setOldValue(const PropType& v)
    {
        m_old_value = v;
    }
    /*! Sets new value for property
        \param[in] v value
     */
    void setNewValue(const PropType & v)
    {
        m_new_value = v;
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
        const sad::db::Variant v(m_new_value);
        m_grid->cell(m_row, m_column)->setProperty(m_property_name, v);
       tryUpdateUI(ob, v);
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
        const sad::db::Variant v(m_old_value);
        m_grid->cell(m_row, m_column)->setProperty(m_property_name, v);
        tryUpdateUI(ob, v);
    }
    /*! Mark, as layout could change region of grid and all children in it
     */
    void markAsCouldChangeRegion()
    {
         m_could_update_area = true;
    }
    /*! Commits a change without updating UI. In that case,
        it doesn't change current cell, if this won't change region

        \param[in] e editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor * e) override
    {
        const sad::db::Variant v(m_new_value);
        m_grid->cell(m_row, m_column)->setProperty(m_property_name, v);
        tryUpdateUI(e, v, true);
    }
protected:
    /*! Tries to update UI
        \param[in] e editor
        \param[in] v value
        \param[in] without_cell whether we should update it without this cell
     */
    void tryUpdateUI(core::Editor* e, const sad::db::Variant& v, bool without_cell = false) const
    {
        if (!e)
            return;        
        gui::actions::GridActions* actions = e->actions()->gridActions();
        const bool is_selected = e->shared()->selectedGrid() == m_grid;
        if (!without_cell && is_selected)
        {
           actions->updateCellPartInUI(m_row, m_column, O, v);
        }
        if (m_could_update_area)
        {
            actions->updateParentGridsRecursively(m_grid, false);
            actions->tryUpdateRegionsInChildren(m_grid);            
        }
        else
        {
            actions->tryUpdateRegionsInChildren(m_grid->cell(m_row, m_column));
        }         
    }
    /*! An affected grid
     */
    sad::layouts::Grid* m_grid;
    /*! A row of cell
     */
    size_t m_row;
    /*! A column of cell
     */
    size_t m_column;
    /*! An old value for cell
     */
    PropType m_old_value;
    /*! A new value for cell
     */
    PropType m_new_value;
    /*! A property name for cell
     */
    sad::String m_property_name;
    /*! Whether it could update area of current grid, changing
        all of it's children
     */
    bool m_could_update_area;
};

}

}

