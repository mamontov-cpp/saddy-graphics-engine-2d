/*! \file actor.h
    
    Defines an actor, which can respond to user actions
 */
#pragma once
#include <p2d/body.h>
#include <p2d/world.h>
#include <sprite2d.h>
#include <animations/animationsinstance.h>
#include <animations/animationsoptionlist.h>
#include <bitset>

#include "actoroptions.h" 

class Game;

namespace game
{
// A last key bitset offset
#define LAST_KEY_BITSET_OFFSET (4)
// A precision for resting detection
#define RESTING_DETECTION_PRECISION (0.5)
/*! An actor, that can respond to user actions
 */
class Actor  // NOLINT(cppcoreguidelines-special-member-functions)
{
public: 
    /*! A button for getting status bit
     */
    enum Button
    {
        ABTN_LEFT = 0,   //!< Left button status
        ABTN_RIGHT = 1,  //!< Right button status
        ABTN_UP = 2,     //!< Up button status
        ABTN_DOWN = 3    //!< Down button status
    };
    /*! Makes new actor options
     */
    Actor();
    /*! Destroys an actor
     */
    ~Actor();
    /*! Sets options for actor, returns false if options are already set
        \param[in] opts options
        \throws logic_error if options are invalid
        \return true if setting is succesfull, otherwise false
     */
    bool setOptions(game::ActorOptions* opts);
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
    /*! Tries to start actor going right
     */
    void tryStartGoingRight();
    /*! Tries to stop actor from going right
     */
    void tryStopGoingRight();
    /*! Called on platform collision
        \patam[in] ev collision event
     */
    void onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev);

    /*! Sets game for player
        \param[in] game a game
     */
    void setGame(Game* game);
    /*! Resets player's state in game
     */
    void reset();
    /*! Inits player's sprite according to state
        \param[in] no_sound whether we should disable sound on actor 
     */
    void init(bool no_sound = false);
    /*! Returns true if actor is in a floater state
        \return whether actor is floater
     */
    bool isFloater() const;
    /*! Sets a floater state for an actor
        \param[in] is_floater a floater state
     */
    void setFloaterState(bool is_floater);

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
    /*! Checks boundary collision for horizontal collision
        \param[in] left_bound a left bound
        \param[in] right_bound a right bound
        \param[in] up_bound an upper bound for actor
        \param[in] bottom_bound a bottom bound for actor
     */
    void checkBoundaryCollision(double left_bound, double right_bound, double up_bound, double bottom_bound);
private:
    /*! Compute whether floater should go up or down
        \param[out] is_going_up whether we should go up
        \param[out] is_going_down whether we should go down
     */
    void computeIsGoingUpDownFlags(bool& is_going_up, bool& is_going_down);
    /*! Compute whether floater should go left or right
        \param[out] is_going_left whether we should go left
        \param[out] is_going_right whether we should go right
     */
    void computeIsGoingLeftRightFlags(bool& is_going_left, bool& is_going_right);
    /*! Sets correct angle for floater
     */
    void setAngleForFloater();
    /*! Computes velocity for floater
        \throws std::logic_error on invalid options or not floater
        \return velocity for floater
     */
    sad::p2d::Vector computeVelocityForFloater();
    /*! Sets vertical velocity value
        \param[in] v velocity
     */
    void setVerticalVelocity(double v) const;
    /*! Returns animations list
        \return animations
     */
    sad::animations::Animations* animations() const;
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
    void correctShape() const;
    /*! A player's own horizontal velocity
     */
    double m_own_horizontal_velocity;
    /*! A main key states
        A (4 + key number) holds whether this type of button was pressed last
     */
    std::bitset<8> m_key_states;
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
    /*! Whether actor is floating object
     */
    bool m_is_floater;

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
    /*! A game for player
     */
    Game* m_game;
    /*! An in-game options for actor
     */
    game::ActorOptions* m_options;
};

}