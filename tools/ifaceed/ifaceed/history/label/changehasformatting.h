/*! \file changehasformatting.h


     Describes a command, which changes scene label's font size
*/
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's has formatting parameter
*/
class ChangeHasFormatting : public history::scenenodes::ChangeProperty<bool>
{
public:
    /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old value of font size
        \param[in] newvalue new value of font size
     */
    ChangeHasFormatting(sad::SceneNode* d, bool oldvalue, bool newvalue);
    /*! Erases link to a node
     */
    virtual ~ChangeHasFormatting();
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
    */
    virtual void updateUI(core::Editor* e, const bool& value);
};

}

}
