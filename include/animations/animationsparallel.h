/*!  \file animations/animationsparallel.h
     

     Defines a parallel animations as a list of animations, which
     are executed at the same time
 */
#pragma once
#include "animationscomposite.h"


namespace sad
{

namespace animations
{

/*! Defines a type of animation, which are applied at the same
    time
 */
class Parallel: public sad::animations::Composite
{
SAD_OBJECT
public:
    /*! Constructs new sequential animation
     */
    Parallel();
    /*! Could be inherited
     */
    virtual ~Parallel() override;
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time) override;
    /*! Non-applicable. Only forces to update time of animation from
        source animation
        \param time a time of animation
     */
    virtual void setTime(double time) override;
};

}

}
