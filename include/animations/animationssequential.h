/*!  \file animations/animationssequential.h
     

     Defines a sequential animation as a list of animations from various sources
 */
#pragma once
#include "animationscomposite.h"


namespace sad
{

namespace animations
{

/*! A sequential animation is an animation, which applied sequentially
    to object. Note, that on second run, if animation is looped,
    object state is not necessarily restored, so make sure, that
    that object returns to it's initial state with last animation.
 */
class Sequential: public sad::animations::Composite
{
SAD_OBJECT
public:
    /*! Constructs new sequential animation
     */
    Sequential();
    /*! Could be inherited
     */
    virtual ~Sequential() override;
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time) override;
    /*! Non-applicable. Only forces to update time of animation from
        source animation
        \param time
     */
    virtual void setTime(double time) override;
    /*! Fetches time of animation, like sum of times of all animations
        \return time
     */
    virtual double time() const override;
    /*! Update animation before starting to play
     */
    virtual void updateBeforePlaying() override;
};

}

}
