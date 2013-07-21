/*! \file   player.h
    \author HiddenSeeker
	\brief class Player is placed here

	Here is placed class of player - class, that provides any player associated
	objects
*/
#include "movitem.h"
#include "collisionmanager.h"
#include "../texturemanager.h"
#pragma once

typedef hPointF Vector;


class Game;

/*! A player class for moving
 */
class Player: public Collidable
{
 SAD_OBJECT
 private:
		 Game  *     m_game;         //!< Game data
		 int         m_score;        //!< A current player score
		 int         m_health;       //!< Amount of healthpoints in player
		 /*! A pivot key, which is pressed last, when player is moving
			 A moment, when player stops is the moment when this key is released
		  */
		 int         m_stopkeys[2];   
		 /*! A players velocity vector. A first coordinate is X and second is Y
		  */
		 Vector       m_velocity;
		
		 float       m_angle;       //!< Angle
		 clock_t     m_lastshot;    //!< Time,when last shot was performed
		 
		 /*! Moves a player by specified vector
			 \param[in] p vector of moving
		  */
		 void move(const Vector & p);
 public:
		 /*! Constructs player
		     \param[in] pos     position
		 */
	     Player(const hPointF & pos);
		 /*! Renderers a player at specified position
		  */
		 void render();

		 /*! Makes player look to position
			 \param[in] p point
		  */
		 void tryLookAt(const sad::Event & p);
		 /*! Makes player look to position
			 \param[in] p point
		  */
		 void lookAt(const hPointF & p);
		 /*! Sets an angle
			 \param[in] angle angle, in which player should rotate
		 */
		 void setAngle(float angle);


		 /*! Shoots a bullet in current direction
		 */
		 void shoot();
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
		 /*! Returns current player health
		  */
		 int health() const;
		 /*! Increases player health by certain points
			 \param[in] by amount of points
		  */
		 int increaseHealth(int by);
		 /*! Decreases player health by certain points
			 \param[in] by amount of points
		  */
		 int decreaseHealth(int by);



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

		 /*! Sets a player to a game, where it cames from
			 \param[in] game game information
		  */
		 void setGame(Game * game);
};

