/*! \file scenenodeschangearea.h
    

    Describe a command, which change scenes name text
 */
#pragma once
#include <sadrect.h>

#include "scenenodeschangeproperty.h"

namespace history
{

namespace scenenodes
{
/*! A command, which change scene label's text
 */
class ChangeArea: public history::scenenodes::ChangeProperty<sad::Rect2D>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeArea(sad::SceneNode* d, const sad::Rect2D& old_value, const sad::Rect2D& new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeArea() override;
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Rect2D& value) override;
};

}

}
