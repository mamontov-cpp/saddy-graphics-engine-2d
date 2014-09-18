/*! \file changetext.h
    \author HiddenSeeker

    Describe a command, which change scene label's text
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's text
 */
class ChangeText: public history::scenenodes::ChangeProperty<sad::String>
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
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void updateUI(core::Editor* e, const sad::String& value);
};

}

}
