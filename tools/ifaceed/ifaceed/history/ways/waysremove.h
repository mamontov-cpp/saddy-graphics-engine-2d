/*! \file waysremove.h
 *  
 *
 * Describes a simple command for removing a way
 */
#pragma once
#include "../command.h"

#include <p2d/app/way.h>

#include <animations/animationswaymoving.h>
#include <animations/animationswayinstance.h>

namespace history
{

namespace ways
{

/*! \class history::ways::Remove
 *   A simple command for adding a new way to list
 */
class Remove: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
        \param[in] pos a position of way in source list
     */
    Remove(sad::p2d::app::Way* w, int pos);
    /*! Could be inherited
     */
    virtual ~Remove() override;
    /*! Sets dependencies for command
        \param[in] dependent_animations list of dependent animations
        \param[in] position_in_animation_combo a position in combo for way moving animation
        \param[in] position_in_animation_instances_combo a position in combo for animation instances
        \param[in] dependent_instances a list of dependent instances for a command
     */
    void setDependencies(
        const sad::Vector<sad::animations::WayMoving*>& dependent_animations,
        int position_in_animation_combo,
        int position_in_animation_instances_combo,
        const sad::Vector<sad::animations::WayInstance*>& dependent_instances
    );
    /*! Makes way inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes way active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected way
     */
    sad::p2d::app::Way* m_way;
    /*! A position of item in list
     */
    int m_position;
    /*! List of dependent animations for restoring links
     */
    sad::Vector<sad::animations::WayMoving*> m_dependent_animations;
    /*! A position of way in animation combo
     */
    int m_position_in_animation_combo;
    /*! A position of way in animation instance combo
     */
    int m_position_in_animation_instances_combo;
    /*! A list of dependent instances
     */
    sad::Vector<sad::animations::WayInstance*> m_dependent_instances;
};

}

}
