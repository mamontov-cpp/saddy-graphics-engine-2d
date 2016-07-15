/*! \file changeflipx.h
    

    Contents a function for changing "flipX" property
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

/*! Creates command for changing "flipX" property
    \param[in] e editor
    \param[in] d a node
    \param[in] oldvalue an oldvalue for property
    \param[in] newvalue a newvalue for property
 */
history::Command* changeFlipX(
    core::Editor* e, 
    sad::SceneNode* d,
    bool oldvalue, 
    bool newvalue
);

}

}