/*! \file animationsremove.h
 *  
 *
 * Describes a simple command for removing an animation
 */
#pragma once
#include "../command.h"
#include <animations/animationscomposite.h>
#include <animations/animationsinstance.h>
#include <sadpair.h>

namespace history
{

namespace animations
{

/*! \class history::animations::Remove
 *   A simple command for removing an animation
 */
class Remove: public history::Command
{
public:
    /*! Creates new command
        \param[in] a animations
     */
    Remove(sad::animations::Animation* a);
    /*! Could be inherited
     */
    virtual ~Remove();
    /*! Sets data for command
        \param[in] position_in_animation_list a position
        \param[in] position_in_animation_instance_list a position in instance list
        \param[in] list a list of dependent animations
     */
    void set(
        int position_in_animation_list,
        int position_in_animation_instance_list,
        const sad::Vector< sad::Pair<sad::animations::Composite*, sad::Vector<int> > >& list
    );
    /*! Sets data for command
        \param[in] list a list of instances
     */
    void set(
        const sad::Vector< sad::animations::Instance* >& list
    );
    /*! Makes animation active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes animation inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Inserts item
        \param[in] editor an editor
     */
    void insertAnimationIntoUI(core::Editor* editor);
    /*! A current selected animation
     */
    sad::animations::Animation* m_animation;
    /*! A position of animation in animation list
     */
    int m_position_in_animation_list;
    /*! A position of animation in animation instance list
     */
    int m_position_in_animation_instance_list;
    /*! A list of composite animations, dependent from current
     */
    sad::Vector< sad::Pair<sad::animations::Composite*, sad::Vector<int> > > m_composites;
    /*! A dependent instances from this animation
     */
    sad::Vector< sad::animations::Instance* > m_dependent_instances;
};

}

}
