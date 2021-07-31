/*! \file scenenodesremove.h
    

    Describe a command, when scene node is removed from scene
 */
#pragma once
#include "../command.h"
#include "scenenode.h"

#include <animations/animationsinstance.h>
#include "../gui/actions/gridactions.h"

namespace history
{
    
namespace scenenodes
{
/*! A command, which must be added, when user removes object from editor scene
 */
class Remove: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] position a position 
      */
     Remove(sad::SceneNode* d, int position);
     /*! Erases link to a node
      */
     virtual ~Remove() override;
     /*! Sets a parameters for command
         \param[in] position_in_instance_combo a position in combo for instances
         \param[in] instances a dependent instances
      */
     void set(
        int  position_in_instance_combo,
        const sad::Vector< sad::animations::Instance* >& instances
     );
     /*! Sets a list of dependent grid for a scene node
          \param grids
      */
     void set(const sad::Vector< gui::GridPosition >& grids);
     /*! Applies changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void commit(core::Editor * ob = nullptr) override;
     /*! Reverts changes, described in command
         \param[in] ob an observer for looking for command
      */
     virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A node, which must be removed from editor
     */
    sad::SceneNode * m_node;
    /*! A position, where node must be inserted
     */
    int m_position;
    /*! A position of item in instance combo
     */
    int m_position_in_instance_combo;
    /*! A dependent instances from this animation
     */
    sad::Vector< sad::animations::Instance* > m_dependent_instances;
    /*!  A list of dependent grids
     */
    sad::Vector< gui::GridPosition > m_dependent_grids;
};

}

}
