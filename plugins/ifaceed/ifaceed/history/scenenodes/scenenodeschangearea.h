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
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeArea(sad::SceneNode* d, const sad::Rect2D& oldvalue, const sad::Rect2D& newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeArea();
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::Rect2D& value);
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const sad::Rect2D& value);
};

}

}
