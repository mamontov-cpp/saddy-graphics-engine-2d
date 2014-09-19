/*! \file scenenodesremove.h
	\author HiddenSeeker

	Describe a command, when scene node is removed from scene
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
	 virtual ~Remove();
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
	/*! A position, where node must be inserted
	 */
	int m_position;
};

}

}
