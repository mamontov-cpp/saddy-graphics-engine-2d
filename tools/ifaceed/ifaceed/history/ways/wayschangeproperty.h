/*!  \file wayschangeproperty.h
 *   
 *
 *   Defines a command type, which describes change of way
 *   in scene node
 */
#pragma once

#include <renderer.h>

#include "../command.h"

#include "../../core/editor.h"


namespace history
{

namespace ways
{

/*! A command, which describes changing property of scen node
 */
template<
    typename T
>
class ChangeProperty: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] property a property name
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangeProperty(
        sad::p2d::app::Way* d,
        const sad::String& property,
        const T& oldvalue,
        const T& newvalue
    )
    : m_way(d), m_property(property), m_oldvalue(oldvalue), m_newvalue(newvalue)
    {
        m_way->addRef();
    }
    /*! Erases link to a node
      */
    virtual ~ChangeProperty()
    {
        m_way->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();
        m_way->setProperty<T>(m_property, m_newvalue);
        sad::Renderer::ref()->unlockRendering();

        tryUpdateUI(ob, m_newvalue);
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();
        m_way->setProperty<T>(m_property, m_oldvalue);
        sad::Renderer::ref()->unlockRendering();

        tryUpdateUI(ob, m_oldvalue);
    }
protected:
    /*! A changed way
     */
    sad::p2d::app::Way* m_way;
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
        if (e->shared()->selectedWay() == m_way && e->isInWaysEditingState())
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
