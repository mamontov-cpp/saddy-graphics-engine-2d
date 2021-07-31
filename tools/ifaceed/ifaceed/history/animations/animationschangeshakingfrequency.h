/*! \file animationschangeshakingfrequency.h
    

    Describes a command, which changes frequency of shaking for CameraShaking animation
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command, which changes frequency of shaking for CameraShaking animation
 */
class ChangeShakingFrequency: public history::animations::ChangeProperty<int>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeShakingFrequency(sad::animations::Animation* d, int old_value, int new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeShakingFrequency() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const int& value) override;
};

}

}
