/*! \file layoutsremove.h


    Describe a command, when layouts is being removed
 */
#pragma once
#include "../command.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "layouts/grid.h"

#include "../gui/actions/gridactions.h"

namespace history
{

namespace layouts
{
/*! A command, which must be added, when user removes layouts from editor
 */
class Remove: public history::Command
{
public:
     /*! Constructs new command for grid
        \param[in] grid a grid
        \param[in] position a position in list of grids
        \param[in] parents a list of positions of grid in parent
      */
     Remove(
        sad::layouts::Grid* grid,
        int position,
        const sad::Vector<gui::GridPosition>& parents
     );
     /*! Erases link to a node
      */
     virtual ~Remove() override;
     /*! Applies changes, described in command
         \param[in] ob editor
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob editor
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Actually commits a command
        \param[in] ob editor
     */
    void _commit(core::Editor* ob);
    /*! Rollbacks a command
        \param[in] ob editor
     */
    void _rollback(core::Editor* ob);
    /*! A grid, which must be removed from editor
     */
    sad::layouts::Grid * m_grid;
    /*! A position, where grid was in list of grids
     */
    int m_position;
    /*! A list of cells
     */
    sad::Vector<sad::layouts::SerializableCell>  m_cells;
    /*! A list of parent nodes
     */
    sad::Vector<gui::GridPosition> m_parent_nodes;
};

}

}
