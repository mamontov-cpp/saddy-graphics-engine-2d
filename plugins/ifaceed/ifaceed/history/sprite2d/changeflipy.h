/*! \file changeflipy.h
	

	Contents a function for changing "flipY" property
 */
#pragma  once
#include "../scenenodes/scenenodeschangepropertywhichlinkedtocheckbox.h"

namespace history
{
	
namespace sprite2d
{

/*! Creates command for changing "flipY" property
	\param[in] d a node
	\param[in] oldvalue an oldvalue for property
	\param[in] newvalue a newvalue for property
 */
history::Command* changeFlipY(sad::SceneNode* d, bool oldvalue, bool newvalue);

}

}