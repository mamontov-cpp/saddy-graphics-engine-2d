/*! \file actoroptions.h
  
    An actor options, used to define how actor should be animated
 */
#pragma once
#include <stdexcept>

#include <sadstring.h>
#include <geometry2d.h>

namespace game
{


/*! An options, which is define how actor should behave or move.
    Once options are assigned to actor, we cannot change, so we should 

    call corresponding actor options.
 */
struct ActorOptions
{
public:
    /*! Whether actor, should float and ignore a gravity
     */
    bool IsFloater;
    /*! Whether actor can emit sound  when jumping or moving
     */
    bool CanEmitSound;
    /*! A maximal horizontal velocity for actor
     */
    double MaxHorizontalVelocity;
    /*! A maximal vertical velocity for actor
     */
    double MaxVerticalVelocity;
    /*! A sprite list for walking animations
     */
    sad::Vector<sad::String> WalkingAnimationOptions;
    /*! A time for walking animation
     */
    double WalkingAnimationTime;
    /*! A sprite list for walking animations
     */
    sad::Vector<sad::String> JumpingAnimationOptions;
    /*! A time for jumping animation
     */
    double JumpingAnimationTime;
    /*! A sprite for standing actor
     */
    sad::String StandingSprite;
    /*! A sprite for walking actor
     */
    sad::String WalkingSprite;
    /*! A sprite for jumping actor
     */
    sad::String JumpingSprite;
    /*! A sprite for falling actor
     */
    sad::String FallingSprite;
    /*! A sprite for ducking actor
     */
    sad::String DuckingSprite;
    /*! A sprite for floate mode
     */
    sad::String FloaterSprite;

    /*! Constructs default actor options - non-valid, non-floater
     */
    inline ActorOptions() : IsFloater(false), CanEmitSound(false), MaxHorizontalVelocity(0), MaxVerticalVelocity(0), WalkingAnimationTime(0),  JumpingAnimationTime(0)
    {

    }

    /*! Validates options, throws exception if buggy
        \throws logic_error if there were an error
     */
    inline void validate()
    {
        if (sad::is_fuzzy_zero(MaxHorizontalVelocity))
        {
            throw std::logic_error("Please, specify maximal horizontal velocity")
        }

        if (sad::is_fuzzy_zero(MaxVerticalVelocity))
        {
            throw std::logic_error("Please, specify maximal vertical velocity")
        }
        
        
        if  (WalkingAnimationOptions.size() == 0)
        {
            throw std::logic_error("Please, specify walking animation options")
        }
        
        if (sad::is_fuzzy_zero(WalkingAnimationTime))
        {
            throw std::logic_error("Please, specify time for walking animation")
        }

        if  (JumpingAnimationOptions.size() == 0)
        {
            throw std::logic_error("Please, specify jumping animation options")
        }
        
        if (sad::is_fuzzy_zero(JumpingAnimationTime))
        {
            throw std::logic_error("Please, specify time for walking animation")
        }
        
        if (FloaterSprite.length() == 0)
        {
            throw std::logic_error("Please, specify floater sprite")
        }

        if (StandingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify standing sprite")
        }
        
        if (WalkingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify walking sprite")
        }

        if (JumpingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify jumping sprite")
        }

        if (FallingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify falling sprite")
        }
        
        if (DuckingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify ducking sprite")
        }
    }
};

}