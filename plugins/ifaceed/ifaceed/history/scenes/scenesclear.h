/*! \file scenesclear.h
	\author HiddenSeeker

	Describes a command, used to clear scene
 */
#pragma once
#include "../command.h"
#include <scene.h>
#include <scenenode.h>
#include <sadvector.h>



namespace history
{

namespace scenes
{

/*! A command to add scene to scene list
 */
class Clear: public history::Command
{
public:
	/*! Adds a scene, which was added. Command links to scene
		\param[in] s a scene
	 */
	Clear(sad::Scene * s);
	/*! Deletes reference to a scene
	 */
	~Clear();
	/*! Applies changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void commit(core::Editor * ob = NULL);
	/*! Reverts changes, described in command
		\param[in] ob an observer for looking for command
	 */
	virtual void rollback(core::Editor * ob = NULL);
protected:
	/*! Describes an added scene
	 */
	sad::Scene* m_scene;
	/*! A scene nodes to be in command
	 */
	sad::Vector<sad::SceneNode *> m_nodes;
};

}

}
