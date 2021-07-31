/*! \file changetext.h
    

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
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeText(sad::SceneNode* d, const sad::String& old_value, const sad::String& new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeText() override;
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void updateUI(core::Editor* e, const sad::String& value) override;
};

}

}
