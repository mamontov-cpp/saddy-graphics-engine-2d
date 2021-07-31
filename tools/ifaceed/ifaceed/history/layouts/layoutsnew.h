/*! \file layoutsnew.h


    Describes a command, when layout grid
    is added to database
 */
#pragma once
#include "../command.h"
#include "layouts/grid.h"

namespace history
{

namespace layouts
{
/*! A command, which must be added, when user added object to editor scene
 */
class New: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
      */
     New(sad::layouts::Grid* d);
     /*! Erases link to a node
      */
     virtual ~New() override;
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A node, which is being operated on
     */
    sad::layouts::Grid * m_grid;

};

}

}
