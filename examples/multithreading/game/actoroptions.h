/*! \file actoroptions.h
  
    An actor options, used to define how actor should be animated
 */
#pragma once
#include <stdexcept>
#include <refcountable.h>
#include <fuzzyequal.h>

#include <sadstring.h>


class Game;

namespace game
{


/*! An options, which is define how actor should behave or move.
    Once options are assigned to actor, we cannot change, so we should 

    call corresponding actor options.
 */
struct ActorOptions: sad::RefCountable
{
    /*! Whether actor, should float and ignore a gravity
     */
    bool IsFloater;
    /*! Whether actor can emit sound  when jumping or moving
     */
    bool CanEmitSound;
    /*! A maximal horizontal velocity for actor
     */
    double WalkerHorizontalVelocity;
    /*! A maximal vertical velocity for actor
     */
    double WalkerVerticalVelocity;
    /*! A floater's horizontal velocity
     */
    double FloaterHorizontalVelocity;
    /*! A floater's vertical velocity
     */
    double FloaterVerticalVelocity;
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
    /*! A sprite for floater mode
     */
    sad::String FloaterSprite;
    /*! A floater flying options
     */
    sad::Vector<sad::String> FloaterFlyAnimationOptions;

    /*! Constructs default actor options - non-valid, non-floater
     */
    inline ActorOptions() : IsFloater(false), CanEmitSound(false), WalkerHorizontalVelocity(0), WalkerVerticalVelocity(0), FloaterHorizontalVelocity(0), FloaterVerticalVelocity(0), WalkingAnimationTime(0),  JumpingAnimationTime(0)
    {

    }

    /*! Validates options, throws exception if buggy
        \throws logic_error if there were an error
     */
    inline void validate() const
    {
        if (sad::is_fuzzy_zero(WalkerHorizontalVelocity))
        {
            throw std::logic_error("Please, specify maximal horizontal velocity for walker");
        }

        if (sad::is_fuzzy_zero(WalkerVerticalVelocity))
        {
            throw std::logic_error("Please, specify maximal vertical velocity for walker");
        }

        if (sad::is_fuzzy_zero(FloaterHorizontalVelocity))
        {
            throw std::logic_error("Please, specify maximal horizontal velocity for floater");
        }

        if (sad::is_fuzzy_zero(FloaterVerticalVelocity))
        {
            throw std::logic_error("Please, specify maximal vertical velocity for floater");
        }

        
        if  (WalkingAnimationOptions.empty())
        {
            throw std::logic_error("Please, specify walking animation options");
        }
        
        if (sad::is_fuzzy_zero(WalkingAnimationTime))
        {
            throw std::logic_error("Please, specify time for walking animation");
        }

        if  (JumpingAnimationOptions.empty())
        {
            throw std::logic_error("Please, specify jumping animation options");
        }
        
        if (sad::is_fuzzy_zero(JumpingAnimationTime))
        {
            throw std::logic_error("Please, specify time for walking animation");
        }
        
        if (FloaterSprite.length() == 0)
        {
            throw std::logic_error("Please, specify floater sprite");
        }

        if (StandingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify standing sprite");
        }
        
        if (WalkingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify walking sprite");
        }

        if (JumpingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify jumping sprite");
        }

        if (FallingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify falling sprite");
        }
        
        if (DuckingSprite.length() == 0)
        {
            throw std::logic_error("Please, specify ducking sprite");
        }
    }
};

/*! Exposes actor options
 *  \param[in] c context
 *  \param[in] game a game
 */
void exposeActorOptions(void* c, Game* game);

}
