/*! \file scenesclear.h
    

    Describes a command, used to clear scene
 */
#pragma once
#include "../command.h"
#include <scenenode.h>
#include <sadvector.h>
#include <sadpair.h>

#include <animations/animationsinstance.h>

#include "../../gui/actions/gridactions.h"

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
    ~Clear() override;
    /*! Sets data for clear command
        \param[in] positions a list of positions for clear command
        \param[in] dependent a list of dependent animations
     */
    void set(
        const sad::Vector< sad::Pair<sad::SceneNode*, int> >& positions,
        const sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> >& dependent
    );
    /*! Sets list of affected grids
        \param[in] affected_grids a list of affected grids
     */
    void setAffectedGrids(
        const sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> >& affected_grids
    );
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
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
    /*! A list of grids, affected by clearing scene
     */
    sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> > m_affected_grids;
};

}

}
