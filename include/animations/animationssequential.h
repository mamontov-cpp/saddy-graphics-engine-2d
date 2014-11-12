/*!  \file animations/animationssequential.h
     \author HiddenSeeker

     Defines a sequential animations as a list of animations from various sources
 */
#pragma once
#include "animationscomposite.h"


namespace sad
{

namespace animations
{


class Sequential: public sad::animations::Composite
{
SAD_OBJECT
public:
    /*! Constructs new sequential animation
     */
    Sequential();
    /*! Could be inherited
     */
    virtual ~Sequential();
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Non-applicable. Only forces to update time of animation from
        source animation
        \param time
     */
    virtual void setTime(double time);
    /*! Fetches time of animation, like sum of times of all animations
        \return time
     */
    virtual double time() const;
};

}

}
