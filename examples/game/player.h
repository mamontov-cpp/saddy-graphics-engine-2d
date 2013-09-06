/*! \file   player.h
    \author HiddenSeeker
	\brief class Player is placed here

	Here is placed class of player - class, that provides any player associated
	objects
*/
#include "gameobject.h"
#pragma once

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
		 int         m_stopkeys[2];   
 public:
	     Player();
		 /*! Makes player look to position
			 \param[in] p point
		  */
		 void tryLookAt(const sad::Event & p);
		 /*! Shoots or not, depending on shooting frequency and current game state
		  */
		 void tryShoot();
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
		 void tryStartMovingLeft(const sad::Event & e);
		 /*! Tries to start moving right
		 	 \param[in] e event
		  */
		 void tryStartMovingRight(const sad::Event & e);
		 /*! Tries to start moving left
			 \param[in] e event
		  */
		 void tryStartMovingUp(const sad::Event & e);
		 /*! Tries to start moving left
			 \param[in] e event
		  */
		 void tryStartMovingDown(const sad::Event & e);
		 /*! Tries to stop moving horizontally if key matches
		  */
		 void tryStopMovingHorizontally(const sad::Event & e);
		 /*! Tries to stop moving horizontally if key matches
		  */
		 void tryStopMovingVertically(const sad::Event & e);
	     /*! Player starts shooting
		  */
		 void startShooting();
	     /*! Player stops shooting
		  */
		 void stopShooting();
};

