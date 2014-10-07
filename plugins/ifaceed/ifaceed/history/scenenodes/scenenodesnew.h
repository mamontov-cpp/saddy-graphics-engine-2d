/*! \file scenenodesnew.h
	\author HiddenSeeker

	Describes a command, when scene node is added to an editor scene
 */
#pragma once
#include "../command.h"
#include "scenenode.h"

namespace history
{
	
namespace scenenodes
{
/*! A command, which must be added, when user added object to editor scene
 */
class New: public history::Command
{
public:
	 /*! Constructs new command for node
		\param[in] d a node
	  */
	 New(sad::SceneNode* d);
	 /*! Erases link to a node
	  */
	 virtual ~New();
	 /*! Applies changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void commit(core::Editor * ob = NULL);
	 /*! Reverts changes, described in command
		 \param[in] ob an observer for looking for command
	  */
	 virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! A node, which must be added to an item
	 */
	sad::SceneNode * m_node;

};

}

}
