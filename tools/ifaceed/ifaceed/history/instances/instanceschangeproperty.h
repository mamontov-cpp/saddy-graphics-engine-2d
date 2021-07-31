/*!  \file instanceschangeproperty.h
 *   
 *
 *   Defines a command type, which describes change of property for animation instance
 *   in scene node
 */
#pragma once

#include <animations/animationsinstance.h>

#include "../command.h"

#include "../../core/editor.h"


namespace history
{

namespace instances
{

/*! A command, which describes changing property of animation instance
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
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeProperty(
        sad::animations::Instance* d,
        const sad::String& property,
        const T& old_value,
        const T& new_value
    )
    : m_animation(d), m_property(property), m_old_value(old_value), m_new_value(new_value)
    {
        m_animation->addRef();
    }
    /*! Erases link to an animation
      */
    virtual ~ChangeProperty() override
    {
        m_animation->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override
    {
        sad::Renderer::ref()->lockRendering();
        m_animation->setProperty<T>(m_property, m_new_value);
        sad::Renderer::ref()->unlockRendering();

        tryUpdateUI(ob, m_new_value);
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override
    {
        sad::Renderer::ref()->lockRendering();
        m_animation->setProperty<T>(m_property, m_old_value);
        sad::Renderer::ref()->unlockRendering();

        tryUpdateUI(ob, m_old_value);
    }
    /*! An old value for changed property
        \return value
     */
    const T& old_value() const
    {
        return m_old_value;
    }
    /*! A new value for changed property
        \return value
     */
    const T& new_value() const
    {
        return m_new_value;
    }
    /*! Returns linked animation
        \return animation
     */
    sad::animations::Instance* animation() const
    {
        return m_animation;
    }
protected:
    /*! A changed animation
     */
    sad::animations::Instance* m_animation;
    /*!
     * A name of property, which is changed
     */
    sad::String m_property;
    /*! An  old value for font property of node
     */
    T m_old_value;
    /*! A new value for font property of node
     */
    T m_new_value;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const T& value)
    {
        if (e->shared()->selectedInstance() == m_animation)
        {
            this->updateUI(e, value);
        }
    }
    /*!
     * Descendants must re-implement this method to make UI update
     * actually happen - marked as empty to simplify creating of complex commands
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const T& value)
    {
    
    }
};

}

}
