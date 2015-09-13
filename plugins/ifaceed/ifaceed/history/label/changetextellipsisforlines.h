/*! \file changetextellipsisforlines.h
    

    Describe a command, which change scene label's text ellipsis position for  lines
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change scene label's text ellipsis for lines
 */
class ChangeTextEllipsisForLines: public history::scenenodes::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangeTextEllipsisForLines(sad::SceneNode* d, unsigned int oldvalue, unsigned int newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeTextEllipsisForLines();
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value);
};

}

}
