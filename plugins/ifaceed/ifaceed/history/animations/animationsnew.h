/*! \file animationsnew.h
 *  \author HiddenSeeker
 *
 * Describes a simple command for adding an animation
 */
#pragma once
#include "../command.h"
#include <animations/animationsanimation.h>

namespace history
{

namespace animations
{

/*! \class history::animations::New
 *   A simple command for adding a new animation
 */
class New: public history::Command
{
public:
    /*! Creates new command
        \param[in] a animations
     */
    New(sad::animations::Animation* a);
    /*! Could be inherited
     */
    virtual ~New();
    /*! Makes animation active, adds it to list
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Makes animation inactive, removes it from list
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current selected animation
     */
    sad::animations::Animation* m_animation;
};

}

}
