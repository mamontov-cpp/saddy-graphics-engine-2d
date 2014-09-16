/*! \file changetext.h
    \author HiddenSeeker

    Describe a command, which change scene label's text
 */
#pragma once
#include "../command.h"
#include "scenenode.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's text
 */
class ChangeText: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeText(sad::SceneNode* d, const sad::String& oldvalue, const sad::String& newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeText();
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A node, which must be added to an item
     */
    sad::SceneNode * m_node;
    /*! An  old value for font property of node
     */
    sad::String m_oldvalue;
    /*! A new value for font property of node
     */
    sad::String m_newvalue;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void tryUpdateUI(core::Editor* e, const sad::String& value);
};

}

}
