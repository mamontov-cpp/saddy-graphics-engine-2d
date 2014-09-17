/*! \file scenenodesvisibilitychange.h
	\author HiddenSeeker

	Contents a function for creating a command for visibility change
 */
#pragma  once
#include "scenenodescheckboxreferencedpropertychange.h"

namespace history
{
	
namespace scenenodes
{

/*! Creates command for visibility change of node
	\param[in] d a node
	\param[in] oldvalue an oldvalue for visibility of node
	\param[in] newvalue a newvalue for visibility of ode
 */
history::Command* visibilityChange(sad::SceneNode* d, bool oldvalue, bool newvalue);

}

}