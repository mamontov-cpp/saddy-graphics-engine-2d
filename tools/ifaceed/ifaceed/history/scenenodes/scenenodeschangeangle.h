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
        \param[in] old_value old text
        \param[in] new_value new text
      */
    ChangeAngle(sad::SceneNode* d, float old_value, float new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeAngle() override;
protected:
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const float& value) override;
};

}

}
