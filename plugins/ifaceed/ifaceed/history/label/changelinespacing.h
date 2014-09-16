/*! \file changelinespacing.h
    \author HiddenSeeker

    Describe a command, which change scene label's line spacing
 */
#pragma once
#include "../command.h"
#include "scenenode.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's line spacing
 */
class ChangeLineSpacing: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old value of line spacing
        \param[in] newvalue new value of line spacing
      */
    ChangeLineSpacing(sad::SceneNode* d, float oldvalue, float newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeLineSpacing();
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
    float m_oldvalue;
    /*! A new value for font property of node
     */
    float m_newvalue;
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void tryUpdateUI(core::Editor* e, float value);
};

}

}
