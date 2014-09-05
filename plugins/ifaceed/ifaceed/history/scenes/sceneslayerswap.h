/*! \file scenelayerswap.h
	\author HiddenSeeker

	Used to swap layers on scenes.
 */
#pragma once
#include "../command.h"

namespace sad
{
class Scene;
}

namespace history
{

namespace scenes
{

class LayerSwap
{
public: 
	/*! Adds a scene, which was added. Command links to scene
		\param[in] s a scene
		\param[in] position a position of scene in scene lists
	 */
	LayerSwap(sad::Scene* s1, sad::Scene* s2, int l1, int l2);
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
	sad::Scene* m_s1;
	/*! A second scene
	 */
	sad::Scene* m_s2;
	/*! A list position for first scene
	 */
	int m_list_position1;
	/*! A list position for second scene
	 */
	int m_list_position2;
};

}

}
