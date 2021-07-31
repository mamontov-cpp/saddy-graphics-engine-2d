/*! \file scenenodeschangevisibility.h
    

    Contents a function for creating a command for visibility change
 */
#pragma  once
#include "scenenodeschangepropertywhichlinkedtocheckbox.h"


namespace core
{
class Editor;
}

namespace history
{
    
namespace scenenodes
{

/*! Creates command for visibility change of node
    \param[in] blk editor
    \param[in] d a node
    \param[in] old_value an old_value for visibility of node
    \param[in] new_value a new_value for visibility of ode
 */
history::Command* changeVisibility(
    core::Editor* blk, 
    sad::SceneNode* d, 
    bool old_value, 
    bool new_value
);

}

}