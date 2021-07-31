/*! \file animationschangeblinkingfrequency.h
    

    Describe a command, which changes frequency of blinking
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command, which change time of animation
 */
class ChangeBlinkingFrequency: public history::animations::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeBlinkingFrequency(sad::animations::Animation* d, unsigned int old_value, unsigned int new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeBlinkingFrequency() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value) override;
};

}

}
