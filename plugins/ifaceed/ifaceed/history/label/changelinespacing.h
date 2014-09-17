/*! \file changelinespacing.h
    \author HiddenSeeker

    Describe a command, which change scene label's line spacing
 */
#pragma once
#include "../scenenodes/scenenodespropertychanged.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's line spacing
 */
class ChangeLineSpacing: public history::scenenodes::PropertyChanged<float>
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
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    void updateUI(core::Editor* e, const float& value);
};

}

}
