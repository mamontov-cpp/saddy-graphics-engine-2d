/*! \file game.h
    

    Describes a game state with all references and other stuff
 */
#pragma once
#include <hfsm/hfsmmachine.h>
#include <periodicalevent.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <scene.h>
#include <p2d/app/app.h>
#include <p2d/walls.h>

/*! An enumerations for creating factory objects
 */
enum class Objects: int
{
    O_BONUS = 0,
    O_ENEMY = 1,
    O_SHOOTING_ENEMY = 2,
    O_SUPER_SHOOTING_ENEMY = 3
};

class GameObject;
class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;

/*! Represents enumeration of game state data
 */
class GameState
{
 public:
	/*! Starting game state
	 */
    static const sad::String START;
	/*! An in-game state
	 */
    static const sad::String PLAYING;
};

/*! A main class of game
 */
class Game: public sad::p2d::app::App
{
public:
    /*! Creates a new non-paused game with no high score
        \param[in] renderer a renderer for initialization
     */
    Game(sad::Renderer* renderer);
    /*! Tries to toggle fullscreen mode if not paused
     */
    void tryToggleFullscreen();
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
    /*! Kills all enemies in a field
     */
    void killEnemies();
    /*! Returns a highest score for player
        \return high score
     */
    int highscore() const;
	/*! Returns true, if game is in playing state
	    \return true if game is in playing state
	 */ 
	bool isInPlayingState() const;
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
    /*! Returns renderer of game
     */
    sad::Renderer* renderer() const;
    
    /*! Returns in-game player
     */
    Player* player();
    /*! Tries to load assets and setup renderer's setting
        \return whether it was successfull
     */
    bool trySetup();
    /*! Initialize a game
     */
    void initialize();
    /*! Runs a game
     */
    void run() override;
    /*! Returns current state of game
        \return current game state
     */
    const sad::String & state();
    /*! Produces game object of specified type
     */
    GameObject * produce(Objects type);
    ~Game() override;
    /*! Sets a new highscore for game, sets in if it's largest that current
        score.
        \param[in] score  a new score
     */
    void trySetHighscore(int score);
    /*! Removes an object
        \param[in] o object
     */
    void removeObject(sad::p2d::app::Object * o) override;
protected:
    /*! Create walls for game
     */
    void createWalls();
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
    /*! Moves to starting screen. Used to not break execution chain when
        called from enterStartingScreen from playing state
     */
    void moveToStartingScreen();
    /*! Deletes an object if not player, when collision with wall
        \param[in] ev event
     */
    void onWallCollision(const sad::p2d::CollisionEvent<sad::p2d::Wall, GameObject> & ev);
    /*! Increases score and hp of bonus and player
        \param[in] ev event
     */
    void onBonusCollision(const sad::p2d::CollisionEvent<Player, Bonus> & ev);
    /*! Increases score of player and decrements hp of enemy
        \param[in] ev event
     */
    void onPlayerBulletEnemy(const sad::p2d::CollisionEvent<PlayerBullet, Enemy> & ev);
    /*! Increases score of player and decrements hp of enemy
        \param[in] ev event
     */
    void onPlayerBulletSEnemy(const sad::p2d::CollisionEvent<PlayerBullet, ShootingEnemy> & ev);
    /*! Increases score of player and decrements hp of enemy
        \param[in] ev event
     */
    void onPlayerBulletSuperEnemy(const sad::p2d::CollisionEvent<PlayerBullet, SuperShootingEnemy> & ev);
    /*! Decreases player's hp and destroys bullet
        \param[in] ev event
     */
    void onPlayerEnemyBullet(const sad::p2d::CollisionEvent<Player, EnemyBullet> & ev);
    /*! Decreases player's hp and destroys enemy
        \param[in] ev event
     */
    void onPlayerEnemy(const sad::p2d::CollisionEvent<Player, Enemy> & ev);
    /*! Decreases player's hp and destroys shooting enemy
        \param[in] ev event
     */
    void onPlayerShootingEnemy(const sad::p2d::CollisionEvent<Player, ShootingEnemy> & ev);
    /*! Decreases player's hp and destroys super shooting enemy
        \param[in] ev event
     */
    void onPlayerSuperShootingEnemy(const sad::p2d::CollisionEvent<Player, SuperShootingEnemy> & ev);
private:
    int                 m_highscore; //!< Defines a highest score, gained by player
    bool                m_is_paused;  //!< Whether game is paused
    sad::hfsm::Machine * m_machine;   //!< A state machine, which describes all transitions
    Player            * m_player;    //!< A link to in-game player
    sad::PeriodicalEventPollProcess * m_spawn_task;//!< A task for spawn an enemies
    sad::p2d::Walls                 * m_walls;     //!< A new walls
    int            m_registered_super_shooting_enemies_count; //!< Current count of super shooting enemies, active in game
    // ReSharper disable once CppInconsistentNaming
    sad::Renderer* m_renderer; //!< A renderer for game  // NOLINT(clang-diagnostic-shadow-field)
};
