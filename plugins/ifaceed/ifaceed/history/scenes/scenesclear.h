/*! \file scenesclear.h
	

	Describes a command, used to clear scene
 */
#pragma once
#include "../command.h"
#include <scenenode.h>
#include <sadvector.h>
#include <sadpair.h>

#include <animations/animationsinstance.h>

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
	/*! Sets data for clear command
		\param[in] positions a list of positions for clear command
		\param[in] dependent a list of dependent animations
	 */
	void set(
		const sad::Vector< sad::Pair<sad::SceneNode*, int> >& positions,
		const sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> >& dependent
	);
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
	/*! A positions of nodes in animation objects list
	 */
	sad::Vector< sad::Pair<sad::SceneNode*, int> > m_positions;
	/*! A dependent instances list
	 */
	sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> > m_dependent;
};

}

}
