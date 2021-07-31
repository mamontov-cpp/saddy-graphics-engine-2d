/*! \file animationsremovefromcomposite.h
 *  
 *
 * Describes a simple command for remove other animations from a list of composite animation
 */
#pragma once
#include "../command.h"
#include <animations/animationscomposite.h>

namespace history
{

namespace animations
{

/*! \class history::animations::RemoveFromComposite
 *   A simple command for removing from list of composite animation
 */
class RemoveFromComposite: public history::Command
{
public:
    /*! Creates new command
        \param[in] a animations
        \param[in] id id of animation
        \param[in] position a position for removal
     */
    RemoveFromComposite(sad::animations::Composite* a, unsigned long long id, int position);
    /*! Could be inherited
     */
    virtual ~RemoveFromComposite();
    /*! Makes animation active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Makes animation inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected animation
     */
    sad::animations::Composite* m_animation;
    /*! An id of added animations
     */
    unsigned long long m_id;
    /*! A position for animation
     */
    int m_position;
};

}

}
