/*! \file changelinespacing.h
    

    Describe a command, which change scene label's line spacing
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's line spacing
 */
class ChangeLineSpacing: public history::scenenodes::ChangeProperty<float>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old value of line spacing
        \param[in] new_value new value of line spacing
      */
    ChangeLineSpacing(sad::SceneNode* d, float old_value, float new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeLineSpacing() override;
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void updateUI(core::Editor* e, const float& value) override;
};

}

}
