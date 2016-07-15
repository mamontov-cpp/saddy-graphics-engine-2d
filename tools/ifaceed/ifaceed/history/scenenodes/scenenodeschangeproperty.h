/*!  \file scenenodeschangeproperty.h
 *   
 *
 *   Defines a command type, which describes change of property
 *   in scene node
 */
#pragma once

#include <renderer.h>

#include "../command.h"

#include "../../core/editor.h"

namespace history
{

namespace scenenodes
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
        sad::SceneNode* d,
        const sad::String& property,
        const T& oldvalue,
        const T& newvalue
    )
    : m_node(d), m_property(property), m_oldvalue(oldvalue), m_newvalue(newvalue), m_affects_parent_grid(false)
    {
        m_node->addRef();
    }
    /*! Erases link to a node
      */
    virtual ~ChangeProperty()
    {
        m_node->delRef();
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();
        m_node->setProperty<T>(m_property, m_newvalue);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_newvalue);
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL)
    {
        sad::Renderer::ref()->lockRendering();        
        m_node->setProperty<T>(m_property, m_oldvalue);
        sad::Renderer::ref()->unlockRendering();
        tryUpdateUI(ob, m_oldvalue);
    }
protected:
    /*! A node, which must be added to an item
     */
    sad::SceneNode* m_node;
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
    /*! Set this to true, if command affects parent grid for node
     */
    bool m_affects_parent_grid;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const T& value)
    {
        if (e->isNodeSelected(m_node))
        {
            this->updateUI(e, value);
        }

        if (m_affects_parent_grid)
        {
            e->tryUpdateParentGridForNode(m_node);
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
