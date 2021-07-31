/*! \file animationsnew.h
 *  
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
    virtual ~New() override;
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
    sad::animations::Animation* m_animation;
};

}

}
