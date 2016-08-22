/*!  \file animationschangeeasingproperty.h
 *   
 *
 *   Defines a command type, which describes change of property
 *   of easing function in animation
 */
#pragma once

#include <animations/animationsanimation.h>
#include <animations/easing/easingfunction.h>

#include "../command.h"

#include "../../core/editor.h"


namespace history
{

namespace animations
{

/*! A command, which describes changing property of animation
 */
template<
    typename T
>
class ChangeEasingProperty: public history::Command
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] setter a setter for property
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangeEasingProperty(
        sad::animations::Animation* d,
        void (sad::animations::easing::Function::*setter)(T),
        const T& oldvalue,
        const T& newvalue
    )
    : m_animation(d), m_setter(setter), m_oldvalue(oldvalue), m_newvalue(newvalue)
    {
        m_animation->addRef();
    }
    /*! Erases link to an animation
      */
    virtual ~ChangeEasingProperty()
    {
        m_animation->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();
        ((m_animation->easing())->*m_setter)(m_newvalue);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_newvalue);
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();        
        ((m_animation->easing())->*m_setter)(m_oldvalue);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_oldvalue);
    }
protected:
    /*! A changed animation
     */
    sad::animations::Animation* m_animation;
    /*!
     * A setter for property
     */
    void (sad::animations::easing::Function::*m_setter)(T);
    /*! An  old value for font property of node
     */
    T m_oldvalue;
    /*! A new value for font property of node
     */
    T m_newvalue;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const T& value)
    {
        if (e->shared()->selectedAnimation() == m_animation)
        {
            this->updateUI(e, value);
        }
    }
    /*!
     * Descendants must reimplement this method to make UI update
     * actually happen
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const T& value) = 0;
};

}

}
