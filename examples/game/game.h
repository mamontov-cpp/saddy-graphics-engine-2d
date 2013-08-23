/*! \file game.h
	\author HiddenSeeker

	Describes a game state with all references and other stuff
 */
#pragma once
#include <extra/statemachine.h>
#include <scene.h>

class Player;
class GameObject;
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
	int  m_highscore;		     //!< Defines a highest score, gained by player
	bool m_ispaused;		     //!< Whether game is paused
	fsm::Machine * m_machine;    //!< A state machine, which describes all transitions
	Player       * m_player;     //!< A link to in-game player
 protected:
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
	~Game();
	 
};
