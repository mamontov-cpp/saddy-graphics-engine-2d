/*! \file changebreaktext.h
    

    Describe a command, which change scene label's break text property
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change how label handles overflows, whether it should break line or not
 */
class ChangeBreakText: public history::scenenodes::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeBreakText(sad::SceneNode* d, unsigned int old_value, unsigned int new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeBreakText() override;
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value) override;
};

}

}
