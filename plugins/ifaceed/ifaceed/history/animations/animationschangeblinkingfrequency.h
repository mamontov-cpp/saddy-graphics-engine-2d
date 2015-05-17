/*! \file animationschangeblinkingfrequency.h
    \author HiddenSeeker

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
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeBlinkingFrequency(sad::animations::Animation* d, unsigned int oldvalue, unsigned int newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeBlinkingFrequency();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value);
};

}

}
