/*! \file changefontsize.h
    

    Describe a command, which change scene label's font size
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's font size
 */
class ChangeFontSize: public history::scenenodes::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old value of font size
        \param[in] newvalue new value of font size
      */
    ChangeFontSize(sad::SceneNode* d, unsigned int oldvalue, unsigned int newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeFontSize();
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value);
};

}

}
