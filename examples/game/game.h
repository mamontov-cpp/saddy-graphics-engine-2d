/*! \file game.h
	\author HiddenSeeker

	Describes a game state with all references and other stuff
 */
#pragma once
#include <extra/statemachine.h>
#include <extra/periodicalevent.h>
#include <scene.h>
#include <p2d/worldsteptask.h>
#include <p2d/world.h>

/*! An enumerations for creating factory objects
 */
enum Objects
{
	O_BONUS = 0,
	O_ENEMY = 1,
	O_SHOOTINGENEMY = 2,
	O_SUPERSHOOTINGENEMY = 3
};

class Player;
class GameObject;
class Walls;
class Wall;

/*! Represents enumeration of game state data
 */
class GameState
{
 public:
	static const hst::string START;
	static const hst::string PLAYING;
};

/*! A main class of game
 */
class Game
{
 private:
	int  m_highscore;		         //!< Defines a highest score, gained by player
	bool m_ispaused;		         //!< Whether game is paused
	fsm::Machine * m_machine;        //!< A state machine, which describes all transitions
	Player       * m_player;         //!< A link to in-game player
	p2d::World   * m_world;          //!< A physical engine world
	p2d::WorldStepTask * m_steptask; //!< A main step task, used to work with world
	TimePeriodicalTask * m_spawntask;//!< A task for spawn an enemies
	Walls        * m_walls;          //!< A new walls
 protected:
	/*! Create walls for game
	 */
    void createWalls();
	/*! Sets a new highscore for game, sets in if it's largest that current
		score.
		\param
	 */
	void trySetHighscore(int score);
	/*! Returns current in-game scene. A helper function for renderer's scene
		\return scene from renderer
	 */
	sad::Scene * scene();
	/*! A callback for leaving starting screen
	 */
	void leaveStartingScreen();
	/*! A callback for leaving playing screen
	 */
	void leavePlayingScreen();
	/*! A callback for entering a starting screen
	 */
	void enterStartingScreen();
	/*! A callback for entering a playing screen
	 */
	void enterPlayingScreen();
	/*! Deletes an object if not player, when collision with wall
		\param[in] ev event
	 */
	void onWallCollision(const p2d::CollisionEvent<Wall, GameObject> & ev);
	/*! Creates new physical world for working with optional bodies
	  */
	void createWorld();
 public:
	/*! Creates a new non-paused game with no highscore
	 */
	Game();
	/*! Tries to toggle fullscreen mode if not paused
	 */
	void tryToggleFullscreen();
	/*! Quits a game 
	 */
	void quit();
	/*! Increases a score for player
	 */
	void increasePlayerScore(int delta);
	/*! Increases player's health by specified points
	 */
	void increasePlayerHealth(int by);
	/*! Decreases player's health by specified points. May end
		the game, when health goes below zero.
	 */
	void decreasePlayerHealth(int by);
	/*! Returns a highest score for player
		\return highscore
	 */
	int highscore() const;
	/*! Tests whether game is paused
		\return whether game is paused
	 */
	bool isPaused() const;
	/*! Tests whether game is paused
		\return whether game is paused
	 */
	bool isPlaying() const;
	/*! Changes game to paused or not
	 */ 
	void togglePaused();
	/*! Switches to playing mode
 	 */
	void startPlaying();
	
	/*! Returns ingame player
	 */
	Player * player();
	/*! Removes a game object from game
	 */
	void removeObject(GameObject * o);
	/*! Runs a game
	 */
	void run();
	/*! Returns current state of game
		\return current game state
	 */
	const hst::string & state();
	/*! Adds a game object
		\param[in] o game object
	 */
	void addObject(GameObject * o);
	/*! Produces game object of specified type
	 */
	GameObject * produce(Objects type);
	~Game();
	 
};
