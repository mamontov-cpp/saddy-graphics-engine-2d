/*! \file scenesremove.h
	\author HiddenSeeker

	Describes a command, used to add remove scene from scenes list
 */
#pragma once
#include "../command.h"
#include <scene.h>


namespace history
{

namespace scenes
{

/*! A command to add scene to scene list
 */
class Remove: public history::Command
{
public:
	/*! Adds a scene, which was added. Command links to scene
		\param[in] s a scene
		\param[in] position a position of scene in scene lists
	 */
	Remove(sad::Scene * s, int position);
	/*! Deletes reference to a scene
	 */
	~Remove();
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
	/*! A position of scene in scene list
	 */
	int m_position;
};

}

}
