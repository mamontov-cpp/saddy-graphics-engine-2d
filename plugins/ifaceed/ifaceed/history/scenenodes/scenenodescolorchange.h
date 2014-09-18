/*! \file scenenodescolorchange.h
    \author HiddenSeeker

    Describe a command, which describes change of node's color
 */
#pragma once
#include <sadcolor.h>

#include "scenenodespropertychanged.h"

namespace history
{

namespace scenenodes
{
/*! A command, which change scene label's text
 */
class ChangeColor: public history::scenenodes::PropertyChanged<sad::AColor>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeColor(sad::SceneNode* d, const sad::AColor& oldvalue, const sad::AColor& newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeColor();
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::AColor& value);
};

}

}
