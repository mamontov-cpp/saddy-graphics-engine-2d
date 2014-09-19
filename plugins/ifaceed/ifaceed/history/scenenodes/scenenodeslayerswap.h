/*! \file scenenodeslayerswap.h
	\author HiddenSeeker

	Used to swap layers on scenes.
 */
#pragma once
#include "../command.h"

namespace sad
{
class SceneNode;
}

namespace history
{

namespace scenenodes
{

class LayerSwap: public history::Command
{
public: 
	/*! Adds a scene, which was added. Command links to scene
		\param[in] n1 first node
		\param[in] n2 second node
		\param[in] l1 first position
		\param[in] l2 second position
	 */
	LayerSwap(sad::SceneNode* s1, sad::SceneNode* s2, int l1, int l2);
	/*! Deletes reference to a scene
	 */
	~LayerSwap();
	/*! Applies changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void commit(core::Editor * ob = NULL);
	/*! Reverts changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! A first scene
	 */
	sad::SceneNode* m_n1;
	/*! A second scene
	 */
	sad::SceneNode* m_n2;
	/*! A list position for first scene
	 */
	int m_list_position1;
	/*! A list position for second scene
	 */
	int m_list_position2;
};

}

}
