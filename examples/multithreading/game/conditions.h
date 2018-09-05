/*!  \file conditions.h
    
     A conditions for easy editing a conditions
 */
#pragma once
#include "options.h"
#include <keymouseconditions.h>
#include <sadvector.h>

// A condition for enum
#define CS_PLAYGAME_PLAYING CS_PLAYGAME_PLAYING_PRESSED
namespace game
{ 

/*! A list of key conditions for multithreading
 */
struct Conditions
{
    /*! A state stuff for conditions
     */
    enum State
    {
        CS_START_SCREEN     = 0,           //!< A starting screen
        CS_OPTIONS_SCREEN   = 1,           //!< An options screen
        CS_PLAYGAME_PLAYING_PRESSED  = 2,  //!< A press on playing screen(common version)
        CS_PLAYGAME_PLAYING_RELEASED = 3,  //!< A press on playing screen, when released
        CS_PLAYGAME_PAUSED  = 4            //!< A playing screen, when paused
    };

/*! A conditions for specified renderer
 */
struct ConditionsForRenderer
{
    /*! A list of conditions, that depends on left key
     */
    sad::KeyHoldCondition* LeftKeyConditions[5];
    /*! A list of conditions, that depends on right key
     */
    sad::KeyHoldCondition* RightKeyConditions[5];
    /*! A list of conditions, that depends on up key
    */
    sad::KeyHoldCondition* UpKeyConditions[5];
    /*! A list of conditions, that depends on down key
     */
    sad::KeyHoldCondition* DownKeyConditions[5];
    /*! A list of conditions, that depends on jump/action key
    */
    sad::KeyHoldCondition* JumpActionConditions[5];
    /*! A pause key condition, depends only if key is pause and in case if use is playing
     */
    sad::KeyHoldCondition* PauseCondition;
    /*! Inits a structure for specified thread
        \param[in] is_inventory_thread whether thread, that uses conditions is inventory thread
     */
    void init(bool is_inventory_thread);
    /*! Sets left key conditions to following key
        \param[in] key a key value
     */
    void setLeftKey(sad::KeyboardKey key);
    /*! Sets right key conditions to following key
        \param[in] key a key value
     */
    void setRightKey(sad::KeyboardKey key);
    /*! Sets up key conditions to following key
        \param[in] key a key value
     */
    void setUpKey(sad::KeyboardKey key);
    /*! Sets down key conditions to following key
        \param[in] key a key value
    */
    void setDownKey(sad::KeyboardKey key);
    /*! Sets jump/action key conditions to following key
        \param[in] key a key value
    */
    void setJumpActionKey(sad::KeyboardKey key);
    /*! Sets pause condition to following key
        \param[in] key a key value
     */
    void setPauseKey(sad::KeyboardKey key) const;

    /*! Sets key for specified conditions
        \param[in] c conditions
        \param[in] key a keyboard key
        \param[in] length a length of array
     */
    static void setKeyForConditions(sad::KeyHoldCondition* c[], sad::KeyboardKey key, size_t length = 5);
};
    /*! Constructs a conditions
     */
    Conditions();
    /*! Sets left key conditions to following key
        \param[in] key a key value
    */
    void setLeftKey(sad::KeyboardKey key);
    /*! Sets right key conditions to following key
        \param[in] key a key value
    */
    void setRightKey(sad::KeyboardKey key);
    /*! Sets up key conditions to following key
        \param[in] key a key value
    */
    void setUpKey(sad::KeyboardKey key);
    /*! Sets down key conditions to following key
        \param[in] key a key value
    */
    void setDownKey(sad::KeyboardKey key);
    /*! Sets jump/action key conditions to following key
        \param[in] key a key value
    */
    void setJumpActionKey(sad::KeyboardKey key);
    /*! Sets pause condition to following key
        \param[in] key a key value
    */
    void setPauseKey(sad::KeyboardKey key) const;
    /*! Sets a conditions for a game, according to their settings
        \param[in] opts options
     */
    void apply(const game::Options& opts);

    /*! A conditions for main renderer
     */
    Conditions::ConditionsForRenderer ConditionsForMainRenderer{};
    /*! A conditions for m renderer
    */
    Conditions::ConditionsForRenderer ConditionsForInventoryRenderer{};
};

}
