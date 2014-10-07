/*! \file sceneschanename.h
	\author HiddenSeeker

	Describes a command, used to change name for a scene
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
class ChangeName: public history::Command
{
public:
	/*! Adds a scene, which was added. Command links to scene
		\param[in] s a scene
		\param[in] oldname old name of a scene
		\param[in] newname new name of a scene
	 */
	ChangeName(sad::Scene * s, const sad::String& oldname, const sad::String& newname);
	/*! Deletes reference to a scene
	 */
	~ChangeName();
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
	/*! Old name for a scene
	 */
	sad::String m_old;
	/*! New name for a scene
	 */
	sad::String m_new;
};

}

}
