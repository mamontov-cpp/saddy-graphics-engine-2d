/*!  \file animationschangeeasingfunctiontype.h
 *   
 *
 *   Defines a command type, which describes change of function type
 *   of easing function in animation
 */
#pragma once
#include "animationschangeeasingproperty.h"

namespace history
{

namespace animations
{

class ChangeEasingFunctionType: public history::animations::ChangeEasingProperty<unsigned int>
{
public:
    /*! Constructs new command for animation
        \param[in] d an animation
        \param[in] old_value old value of property
        \param[in] new_value new value of property
     */
    ChangeEasingFunctionType(
        sad::animations::Animation* d,
        unsigned int old_value,
        unsigned int new_value
    );
    /*! Could be inherited
     */
    virtual ~ChangeEasingFunctionType();
protected:
    /* Updates UI
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value) override;
};

}

}
