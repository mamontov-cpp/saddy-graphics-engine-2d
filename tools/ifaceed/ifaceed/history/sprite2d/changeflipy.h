/*! \file changeflipy.h
    

    Contents a function for changing "flipY" property
 */
#pragma  once
#include "../scenenodes/scenenodeschangepropertywhichlinkedtocheckbox.h"

namespace core
{
class Editor;    
}

namespace history
{
    
namespace sprite2d
{

/*! Creates command for changing "flipY" property
    \param[in] e editor
    \param[in] d a node
    \param[in] old_value an old_value for property
    \param[in] new_value a new_value for property
 */
history::Command* changeFlipY(
    core::Editor* e,
    sad::SceneNode* d, 
    bool old_value, 
    bool new_value
);

}

}