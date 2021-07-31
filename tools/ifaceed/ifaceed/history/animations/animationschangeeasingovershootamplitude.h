/*!  \file animationschangeeasingovershootamplitude.h
 *   
 *
 *   Defines a command type, which describes change of overshoot amplitude
 *   of easing function in animation
 */
#pragma once
#include "animationschangeeasingproperty.h"

namespace history
{

namespace animations
{

class ChangeEasingOvershootAmplitude: public history::animations::ChangeEasingProperty<double>
{
public:
    /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value of property
        \param[in] new_value new value of property
     */
    ChangeEasingOvershootAmplitude(
        sad::animations::Animation* d,
        double old_value,
        double new_value
    );
    /*! Could be inherited
     */
    virtual ~ChangeEasingOvershootAmplitude();
protected:
    /* Updates UI
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const double& value) override ;
};

}

}
