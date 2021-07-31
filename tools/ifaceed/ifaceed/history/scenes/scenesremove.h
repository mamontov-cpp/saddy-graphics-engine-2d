/*! \file scenesremove.h
    

    Describes a command, used to add remove scene from scenes list
 */
#pragma once
#include "../command.h"

#include <scene.h>
#include <sadpair.h>

#include <animations/animationsinstance.h>

#include "../../gui/actions/gridactions.h"

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
    ~Remove() override;
    /*! Sets an elements
        \param[in] position_in_instance_combo a position of instance in removal
        \param[in] positions a positions list
        \param[in] dependent_animations a list of dependent animation
        \param[in] dependent_from_nodes a list of animation instances, dependent from nodes of scene
     */
    void set(
        int position_in_instance_combo,
        const sad::Vector< sad::Pair<sad::SceneNode*, int> >& positions,
        const sad::Vector<sad::animations::Instance*>& dependent_animations,	
        const sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> >& dependent_from_nodes
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
    /*! A position of scene in scene list
     */
    int m_position;
    /*! A position of instances in animation instance combo
     */
    int m_position_in_instance_combo;
    /*! A position of scene nodes in comb boxes
     */
    sad::Vector< sad::Pair<sad::SceneNode*, int> > m_scenenodes_positions;
    /*! A dependent animation list
     */
    sad::Vector<sad::animations::Instance*> m_dependent_animations;
    /*! A dependent instances list
     */
    sad::Vector< sad::Pair<sad::animations::Instance*, unsigned long long> > m_dependent_from_nodes;
    /*! A list of grids, affected by clearing scene
     */
    sad::Vector< sad::Pair<sad::SceneNode*, gui::GridPosition> > m_affected_grids;
};

}

}
