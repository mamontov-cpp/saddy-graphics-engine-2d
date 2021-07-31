/*! \file   player.h
    
    \brief class Player is placed here

    Here is placed class of player - class, that provides any player associated
    objects
*/
#pragma once
#include "gameobject.h"
#include <input/events.h>

class Game;

/*! A player class used to be alter-ego of gamer. 
    It's a sad smiley face
 */
class Player: public GameObject
{
 SAD_OBJECT
 private:
         AbstractAutomaticGun * m_gun;  //!< A min object gun
         int         m_score;           //!< A current player score
         /*! A pivot key, which is pressed last, when player is moving
             A moment, when player stops is the moment when this key is released
          */
         sad::KeyboardKey   m_stop_keys[2];   
 public:
         Player();
         /*! Makes player look to position
             \param[in] p point
          */
         void tryLookAt(const sad::input::MouseMoveEvent & p);
         /*! Returns a score for data
             \return score of players
          */
         int score() const;
         /*! Increases a score for player
             \param[in] delta delta for score
             \return new increased score
          */
         int increaseScore(int delta);
         /*! Tries to start moving left
             \param[in] e event
          */
         void tryStartMovingLeft(const sad::input::KeyPressEvent & e);
         /*! Tries to start moving right
             \param[in] e event
          */
         void tryStartMovingRight(const sad::input::KeyPressEvent & e);
         /*! Tries to start moving left
             \param[in] e event
          */
         void tryStartMovingUp(const sad::input::KeyPressEvent & e);
         /*! Tries to start moving left
             \param[in] e event
          */
         void tryStartMovingDown(const sad::input::KeyPressEvent & e);
         /*! Tries to stop moving horizontally if key matches
          */
         void tryStopMovingHorizontally(const sad::input::KeyReleaseEvent & e);
         /*! Tries to stop moving horizontally if key matches
          */
         void tryStopMovingVertically(const sad::input::KeyReleaseEvent & e);
         /*! Player starts shooting
          */
         void startShooting();
         /*! Player stops shooting
          */
         void stopShooting();
};

