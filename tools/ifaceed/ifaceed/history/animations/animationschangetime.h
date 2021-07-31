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
        \param[in] old_value old value
        \param[in] new_value new value
      */
    ChangeTime(sad::animations::Animation* d, double old_value, double new_value);
    /*! Erases link to an animation
      */
    virtual ~ChangeTime() override;
protected:
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const double& value) override ;
};

}

}
