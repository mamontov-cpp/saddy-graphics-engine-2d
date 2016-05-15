/*! \file scenenodeschangeangle.h
    

    Describe a command, which change angle of scene node
 */
#pragma once
#include "scenenodeschangeproperty.h"

namespace history
{

namespace scenenodes
{
/*! A command, which change angle of scene node
 */
class ChangeAngle: public history::scenenodes::ChangeProperty<float>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeAngle(sad::SceneNode* d, float oldvalue, float newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeAngle();
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const float& value);
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const float& value);
};

}

}
