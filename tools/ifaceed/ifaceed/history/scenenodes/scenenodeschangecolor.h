/*! \file scenenodeschangecolor.h
    

    Describe a command, which describes change of node's color
 */
#pragma once
#include <sadcolor.h>

#include "scenenodeschangeproperty.h"

namespace history
{

namespace scenenodes
{
/*! A command, which change scene label's text
 */
class ChangeColor: public history::scenenodes::ChangeProperty<sad::AColor>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeColor(sad::SceneNode* d, const sad::AColor& old_value, const sad::AColor& new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeColor() override;
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::AColor& value) override;
};

}

}
