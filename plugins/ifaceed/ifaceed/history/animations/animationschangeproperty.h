/*!  \file animationschangeproperty.h
 *   
 *
 *   Defines a command type, which describes change of property
 *   in scene node
 */
#pragma once

#include <animations/animationsanimation.h>

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
class ChangeProperty: public history::Command
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] property a property name
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangeProperty(
        sad::animations::Animation* d,
        const sad::String& property,
        const T& oldvalue,
        const T& newvalue
    )
    : m_animation(d), m_property(property), m_oldvalue(oldvalue), m_newvalue(newvalue)
    {
        m_animation->addRef();
    }
    /*! Erases link to an animation
      */
    virtual ~ChangeProperty()
    {
        m_animation->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();
        m_animation->setProperty<T>(m_property, m_newvalue);
        sad::Renderer::ref()->unlockRendering();
        if (m_enable_update_ui)
        {
            tryUpdateUI(ob, m_newvalue);
        }
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();        
        m_animation->setProperty<T>(m_property, m_oldvalue);
        sad::Renderer::ref()->unlockRendering();
        if (m_enable_update_ui)
        {
            tryUpdateUI(ob, m_oldvalue);
        }
    }
protected:
    /*! A changed animation
     */
    sad::animations::Animation* m_animation;
    /*!
     * A name of property, which is changed
     */
    sad::String m_property;
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
