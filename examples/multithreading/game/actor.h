/*! \file actor.h
    
    Defines an actor, which can respond to user actions
 */
#pragma once
#include "actoroptions.h" 
#include <p2d/body.h>

namespace game
{

/*! An actor, that can respond to user actions
 */
class Actor
{
public:
    /*! Makes new actor options
        \param[in] opts an actor options
     */
    Actor(game::ActorOptions* opts);
    /*! Destroys an actor
     */
    ~Actor();

    /*! Tries to start actor going up
     */
    void tryStartGoingUp();
    /*! Tries to start actor from going up
     */
    void tryStopGoingUp();
    /*! Tries to start actor going down
     */
    void tryStartGoingDown();
    /*! Tries to stop actor from going down
     */
    void tryStopGoingDown();
    /*! Tries to start actor going left
     */
    void tryStartGoingLeft();
    /*! Tries to stop actor from going left
     */
    void tryStopGoingLeft();
    /*! Tries to stop actor from going right
     */
    void tryStopGoingRight();
    /*! Called on platform collision
        \patam[in] platform a platform, which were collided
     */
    void onPlatformCollision(sad::p2d::Body* platform);

    /*! Sets game for player
        \param[in] gama a game
     */
    void setGame(Game* game);
    /*! Resets player's items in game
     */
    void reset();

    /*! Whether player is resting on platform
        \return whether he is resting
     */
    bool isResting() const;
    /*! Whether player can jump
        \return whether player can jump
     */
    bool canJump() const;
    /*! Sets sprite for player
     *  \param[in] sprite a sprite
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Sets body for player
     *  \param[in] body a body for player
     */
    void setBody(sad::p2d::Body* body);
    /*! Sets horizontal velocity for player
     *  \param[in] value velocity value
     */
    void setHorizontalVelocity(double value);
    /*! Increments vertical velocity of player by value
     */
    void incrementVerticalVelocity(double value) const;
    /*! Enables gravity for body
     */
    void enableGravity() const;
    /*! Disables gravity for body
     */
    void disableGravity() const;
    /*! Rests player on platform
        \param[in] b body
        \param[in] old_velocity old platform's velocity
     */
    void restOnPlatform(sad::p2d::Body* b, const  sad::p2d::Vector& old_velocity);
    /*! Disables resting player on platform
     */
    void disableResting();
    /*! Fetches area for user
     */
    sad::Rect2D area() const;
    /*! Moves player by point
     */
    void move(const sad::Point2D& p) const;
    /*! Clears flags, which determine, whether player's position is fixed
     */
    void clearFixedFlags();
    /*! Tests, whether x coordinate fixed
     *  \return whehter it's fixed
     */
    bool isXCoordinateFixed() const;
    /*! Tests, whether y coordinate fixed
     *  \return whehter it's fixed
     */
    bool isYCoordinateFixed() const;
    /*! Makes x coordinate fixed or not
     *  \param[in] value m new value
     */
    void setXCoordinateFixed(bool value);
    /*! Makes y coordinate fixed or not
     *  \param[in] value m new value
     */
    void setYCoordinateFixed(bool value);
    /*! Returns body for player
     *  \return body
     */
    sad::p2d::Body* body() const;
    /*! Returns sprite for player
        \return sprite for player
     */
    sad::Sprite2D* sprite() const;
    /*! Tests resting a player
     */
    void testResting();
    /*! Pushes old options, replaces with new in sprite
     *  \param[in] new_options a new options
     */
    void pushOptions(const sad::String& new_options);
    /*! Pops options
     */
    void popOptions();
    /*! Starts moving actor to left
     */
    void startMovingLeft();
    /*! Start moving actor to right
     */
    void startMovingRight();
    /*! Stops actor from moving horizontally
     */
    void stopMovingHorizontally();
    /*! Makes actor try to jump
     */
    void tryJump();
    /*! Makes actor start falling or duck
     */
    void startFallingOrDuck();
    /*! Stops falling or ducking for actor
     */
    void stopFallingOrStopDucking();
    /*! Makes actor stop ducking
     */
    void duck();
    /*!  Makes actor stand, not duck
     */
    void stopDucking();
    /*! Returns whether actor is ducking
        \return whether actor is ducking
     */
    bool isDucking() const;
    /*! Returns whether actor is freefalling
        \return whether actor is freefalling
     */
    bool isFreefalling() const;
    /*! Returns old velocity for player
        \return old veloctity
     */
    const sad::p2d::Vector& oldVelocity() const;
private:
   /*! Returns animations list
        \return animations
     */
    sad::animations::Animations* animations();
    /*! Plays walking animation
     */
    void playWalkingAnimation();
    /*! Cancels walking animation
     */
    void cancelWalkingAnimation();
    /*! Plays jumping animation
     */
    void playJumpingAnimation();
    /*! Cancels jumping animation
     */
    void cancelJumpingAnimation();
    /*! Starts moving player in specified direction (positive - right, negative - left)
        \param[in] flip_flag flip flag value
        \param[in] velocity a velocity value
     */
    void startMoving(bool flip_flag, double velocity);
    /*! Synchronizes actor's shape with it's area.
        Used for ducking
     */
    void correctShape();
    /*! A player's own horizontal velocity
     */
    double m_own_horizontal_velocity;
    /*! A player's sprite
     */
    sad::Sprite2D* m_sprite;
    /*! An old velocity, until player canceled moving or other stuff
     */ 
    sad::p2d::Vector m_old_velocity;
    /*! A player's body
     */
    sad::p2d::Body* m_body;
    /*! Whether player is resting on platform
     */
    bool m_is_resting;
    /*! Whether player is ducking
     */
    bool m_is_ducking;
    /*! Whether we are free falling, using down button
     */
    bool m_is_free_fall;
    /*! True, if walking animation is playing
     */
    bool m_is_walking_animation_playing;
    /*! True, if jumping animation is playing
     */
    bool m_is_jumping_animation_playing;
    /*! A platform, where player is resting
     */
    sad::p2d::Body* m_resting_platform;
    /*! Whether x position is fixed
     */
    bool m_fixed_x;
    /*! Whether y position is fixed
     */
    bool m_fixed_y;

    /*! A walking animation for actor
     */
    sad::animations::OptionList* m_walking_animation;
    /*! A walking animation instance
     */
    sad::animations::Instance* m_walking_instance;
    
    /*! A jumping animation for player
     */
    sad::animations::OptionList* m_jumping_animation;
    /*! A jumping animation instance
     */
    sad::animations::Instance* m_jumping_instance;
    /*! An old options for sprite
     */
    sad::Vector<sad::String> m_old_options;
    /*! A game for player
     */
    Game* m_game;
    /*! An in-game options for actor
     */
    game::ActorOptions* m_options;
};

}