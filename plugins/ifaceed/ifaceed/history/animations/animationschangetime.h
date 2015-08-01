/*! \file animationschangetime.h
    

    Describe a command, which changes time of animations
 */
#pragma once
#include "animationschangeproperty.h"

namespace history
{

namespace animations
{
/*! A command, which change time of animation
 */
class ChangeTime: public history::animations::ChangeProperty<double>
{
public:
     /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] oldvalue old value
        \param[in] newvalue new value
      */
    ChangeTime(sad::animations::Animation* d, double oldvalue, double newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeTime();
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value);
};

}

}
