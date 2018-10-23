/*! \file bulletsettings.h


    Defines a basic settings for bullet
 */
#pragma once
#include <sadstring.h>
#include <sadpoint.h>

namespace weapons
{

/*! A settings for bullet
 */
class BulletSettings
{
public:
    /*! A name for icon
     */
    sad::String IconName;
    /*! A tangential speed fpr bullet
     */
    double Speed;
    /*! Whether gravity should be applied to bullet
     */
    bool ApplyGravity;
    /*! A gravity value to be applied
     */
    sad::Point2D GravityValue;
    /*! An angular speed for bullet
     */
    double AngularSpeed;
    /*! Whether bullet should bounce or be removed, when collided with platform.
     */
    bool IsGhost;
    /*! Maximal amount of bouncing, before bullet will be removed
     */
    int MaxBounceCount;
    /*! A resilience coefficient for bouncing
     */
    double BounceResilienceCoefficient;
    /*! If set to true bullet won't decay, when enemy is hit
     */
    bool IsPiercing;

    /*! Default bullet settings
     */
    inline BulletSettings()
    : IconName("bullets/green/x_huge"),
    Speed(400),
    ApplyGravity(false),
    AngularSpeed(0),
    IsGhost(false),
    MaxBounceCount(0),
    BounceResilienceCoefficient(1.0),
    IsPiercing(false)
    {

    }
};

}
