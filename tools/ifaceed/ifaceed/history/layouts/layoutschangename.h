/*! \file layoutschangename.h


    Describes a command, when layout name is changed
 */
#pragma once
#include "../command.h"
#include "layouts/grid.h"

namespace history
{

namespace layouts
{
/*! A command, which must be added, when user is changed
    name of layout
 */
class ChangeName: public history::Command
{
public:
     /*! Constructs new command for grid
         \param[in] d a grid
         \param[in] old_name old name of layout
         \param[in] new_name new name of layout
      */
     ChangeName(  // NOLINT(readability-inconsistent-declaration-parameter-name)
         sad::layouts::Grid* d,
         const sad::String& old_name,
         const sad::String& new_name
     );
     /*! Erases link to a node
      */
     virtual ~ChangeName() override;
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
    /*! Commits a change without updating UI. Useful, when command should not touch
        a user interface, see https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/55 for 
        reason, why this function exists.

        By default it's a stub for calling a commit function.

        \param[in] e editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor * e) override;
protected:
    /*! Updates UI, according to property value
        \param[in] e editor
     */
    void tryUpdateUI(core::Editor * e);
    /*! Updates UI, according to property value
        \param[in] e editor
     */
    void tryUpdateUIWithEditableFields(core::Editor * e);
    /*! A node, whose name is changed
     */
    sad::layouts::Grid * m_grid;
    /*! An old value of property
     */
    sad::String m_old_value;
    /*! A new value of property
     */
    sad::String m_new_value;
};

}

}
