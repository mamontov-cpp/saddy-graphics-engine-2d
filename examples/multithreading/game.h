/*! \file game.h

    Main game class
 */
#pragma once
#include <sprite2d.h>
#include <sadhash.h>

#include <hfsm/hfsmmachine.h>

#include <irrklang/singlesound.h>
#include <irrklang/engine.h>

#include <dukpp-03/context.h>

#include <p2d/world.h>
#include <p2d/worldsteptask.h>
#include <p2d/bouncesolver.h>

#include "threads/tasklock.h"

#include "game/conditions.h"
#include "game/player.h"
#include "game/actoroptions.h"
#include "game/movingplatformregistry.h"
#include "game/triggers.h"
#include "game/actors.h"
#include "game/walls.h"
#include "game/unanimatedcoins.h"

#include "weapons/bullet.h"

#include "bots/botregistry.h"

#include "scenetransitionprocess.h"
#include "delayedtasks.h"
#include "optionsscreen.h"
namespace sad
{

namespace animations
{

class Blinking;

}

}


namespace game
{
class Item;
class ScoreBar;
}

namespace nodes
{
class InventoryNode;
class InventoryPopup;
}

namespace threads
{
class GameThread;
}

/*! Main game class
 */
class Game  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
    /*! A main menu state
     */
    enum MainMenuState
    {
        GMMS_PLAY     = 0,  //!< A new game is selected
        GMMS_OPTIONS  = 1,  //!< Options are selected
        GMMS_EXIT     = 2   //!< An exit state is selected
    };
    /*! A default gravity forcevalue
     */
    static const sad::Point2D GravityForceValue;
    /*! Amount of lives for basic enemy
     */
    static const int BasicEnemyLivesCount;
    /*! Amount of lives for player
     */
    static const int BasicPlayerLivesCount;
    /*! Constructs game object
     */
    Game();
    /*! Destructs game object
     */
    ~Game();
    /*! Runs main game thread
     */
    void runMainGameThread();
    /*! Runs inventorty thread
     */
    void runInventoryThread();

    /*! For starting screen synchronizes state of player's picked point with current menu ite
        \param[in] state picked option
     */
    void putPlayerPickAccordingToMenuState(Game::MainMenuState state);
    /*! Sets controls for main thread
        \param[in] renderer a renderer for main thread
        \param[in] db database
     */
    void setControlsForMainThread(sad::Renderer* renderer, sad::db::Database* db);
    /*! Sets controls for inventory thread
        \param[in] renderer a renderer for main thread
     */
    void setControlsForInventoryThread(sad::Renderer* renderer);
    /*! Quits game
     */
    void quitGame();
    /*! Returns highest score for a game
     *  \return score
     */
    int highscore() const;
    /*! Sets new highscore for a game
        \param[in] highscore a highscore
     */
    void setHighscore(int highscore);
    /*! Returns score for a game
     *  \return score
     */
    int score() const;
    /*! Sets score for a game
     *  \param[in] score a score for a game
     */
    void setScore(int score);
    /*! Increments score for a game
     *  \param[in] dscore score delta
     */
    void incrementScore(int dscore);
    /*! Decrements score for a game
     *  \param[in] dscore score delta
     */
    void decrementScore(int dscore);
    /*! Start starting state
     */
    void tryStartStartingState();
    /*! Inits start screen for main thread
     */
    void initStartScreenForMainThread();
    /*! Inits start screen for inventory thread
     */
    void initStartScreenForInventoryThread();
    /*! Plays theme for a game
        \parma[in] theme a theme for game
     */
    void playTheme(const sad::String& theme);
    /*! Sets music volume
        \param[in] volume a volume
     */
    void setThemeVolume(double volume);
    /*! Enters playing state on paused state machine
     */
    void enterPlayingState();
    /*! Enter transiioning state on paused state machine
     */
    void enterTransitioningState();
    /*! Enter paused state on paused state machine
     */
    void enterPausedState();
    /*! Triggers win game
     */
    void triggerWinGame();
    /*! Transitions the game from current scene to the next one
        \param[in] opts options
     */
    void changeScene(const SceneTransitionOptions& opts) const;
    /*! Change scene to playing screen
     */
    void changeSceneToPlayingScreen();
    /*! Changes scene to options screen
     */
    void changeSceneToOptions();
    /*! Changes scene to starting screen
     */
    void changeSceneToStartingScreen();
    /*! Changes screen to lose screen
     */
    void changeSceneToLoseScreen();
    /*! Changes screen to win screen
     */
    void changeSceneToWinScreen();
    /*! Enters starting state
     */
    void enterStartScreenState();
    /*! Enters options state
     */
    void enterOptionsState();
    /*! Enters playing game screen state
     */
    void enterPlayingScreenState();
    /*! Tries loading options screen
        \param[in] is_inventory_thread whether it's inventory thread
     */
    void tryLoadOptionsScreen(bool is_inventory_thread);
    /*! Tries loading lose screen
        \param[in] is_inventory_thread whether it's inventory thread
     */
    void tryLoadLoseScreen(bool is_inventory_thread);
    /*! Tries loading win screen
        \param[in] is_inventory_thread whether it's inventory thread
     */
    void tryLoadWinScreen(bool is_inventory_thread);
    /*! Tries loading game screen
     */
    void tryLoadGameScreen();
    /*! Returns options screen
        \return options screen
     */
    OptionsScreen& optionsScreen();
    /*! Plays sound
        \param[in] sound_name a name of sound
     */
    void playSound(const sad::String& sound_name) const;
    /*! Plays walking sound
     */
    void playWalkingSound();
    /*! Stops walking sound
     */ 
    void stopWalkingSound();
    /*! Returns in-game options
        \return game options
     */
    game::Options* options();
    /*! A conditions for game
     */
    game::Conditions* conditions();

    /*! Returns renderer for main thread
     */
    sad::Renderer* rendererForMainThread() const;
    /*! Returns renderer for inventory thread
     */
    sad::Renderer* rendererForInventoryThread() const;
    /*! Tries to show inventory popup, which succeeds only if
        cursor is on item
        \param[in] p point
     */
    void tryShowInventoryPopup(const sad::Point2D& p) const;
    /*! Evaluates script, passed as JS
     *  \param[in] s string data
     *  \return whether evaluate was successfull
     */
    bool evalScript(const sad::String& s) const;
    /*! Evaluates initialization script
     */
    void evaluateInitializationScript() const;
    /*! Constructs new item
    *  \param[in] icon an icon for item
    *  \return new item
    */
    game::Item* makeItem(const sad::String& icon);
    /*! Makes actor unpickable
     *  \param[in] actor an item actor
     */
    void makeItemUnpickable(game::Actor* actor);
    /*! Enables gravity for body
     *  \param[in] b body
     */
    static void enableGravity(sad::p2d::Body* b);
    /*! Disables gravity for body
     *  \param[in] b body
     */
    static void disableGravity(sad::p2d::Body* b);
    /*! Sets gravity for body
     *  \param[in] b body
     *  \param[in] v value of force
     */
    static void setGravityForBody(sad::p2d::Body* b, const sad::p2d::Vector& v);
    /*! Wait for pipeline tasks to finish
     */
    void waitForPipelineTasks();
    /*! Returns a bounce solver for game
        \return solver
     */
    sad::p2d::BounceSolver* bounceSolver() const;
    /*! Returns a world for physics simulation
        \return world
     */
    sad::p2d::World* physicsWorld() const;
    /*! Kills actor by body
        \param[in] body a body for actor
     */
    void killActorByBody(sad::p2d::Body* body);
    /*! Kills actor, without killing sprite
        \param[in] actor actor to be killed
     */
    void killActorWithoutSprite(game::Actor* actor);
    /*! Returns a player
        \return player
     */
    game::Player* player() const;
    /*! Kill projectiles, related to actor
        \param[in] actor an actor
     */
    void killProjectilesRelatedToActor(game::Actor* actor);
    /*! Kills projectile object, removing it's data
        \param[in] projectile a projectile
     */
    void killProjectile(weapons::Projectile* projectile);
    /*! Adds new projectile into game
        \param[in] projectile a projectile
     */
    void addProjectile(weapons::Projectile* projectile);
    /*! Tests if actor is dead
        \param[in] actor an actor
        \return
     */
    bool isDead(game::Actor* actor) const;
    /*! Sets shooting strategy for bot
     *  \param[in] actor an actor
     *  \param[in] s strategy
     */
    void setStrategyForBot(game::Actor* actor, bots::shootingstrategies::ShootingStrategy* s);
    /*! Adds delayed task, which can be paused
        \param[in] time a time
        \param[in] fn a function
     */
    void addDelayedTask(double time, const std::function<void()>& fn);
    /*! Makes new enemy actor
        \param[in] optname a name for options for actor
        \param[in] middle a middle of enemy
     */
    game::Actor* makeEnemy(const sad::String& optname, const sad::Point2D& middle);
    /*! Makes new item
        \param[in] optname a name for sprite for actor (not an options for actor)
        \param[in] middle a middle of item
     */
    game::Actor* makeItemActor(const sad::String& optname, const sad::Point2D& middle);
    /*! Tries to fetch from registry abstract bot
        \param[in] bot_name a name of bot
        \return bot or  NULL if not found
     */
    bots::AbstractBot* getFromRegistry(const sad::String& bot_name) const;
    /*! Returns true if we have bot in registry
        \param[in] bot_name a name for bot
        \return true if bot is in registry
     */
    bool hasBotInRegistry(const sad::String& bot_name) const;
    /*! Adds new bot to game
        \param[in] actor an actor
        \param[in] bot a bot
     */
    void addActor(game::Actor* actor, bots::AbstractBot* bot);
    /*! Adds animation process into main renderer
        \param[in] p process
     */
    void addToMainRenderer(sad::animations::Process* p) const;
    /*! Returns actor options by name
        \param[in] name a name for options
        \return actor options by name
     */
    game::ActorOptions* getActorOptions(const sad::String& name);
    /*! Adds actor options to game
        \param[in] name a name for options
        \param[in] opts options
     */
    bool addActorOptions(const sad::String& name, const game::ActorOptions& opts);
    /*! Returns bounce solver for bullets
     * \return solver for bullets
     */
    sad::p2d::BounceSolver* bounceSolverForBullets() const;
    /*! Returns context for game
     *  \return context
     */
    sad::dukpp03::Context* context() const;
    /*! Clears item defintions
     */
    void clearItemDefinitions();
private:
    /*! Inits evaluation context
     */
    void initContext();
    /*! Clear stored local projectiles
     *  \param[in] kill whether we should call kill on each of them
     */
    void clearProjectiles(bool kill = true);
    /*! Destroys physics world
     */
    void destroyWorld();
    /*! Inits in-game physics
     */
    void initGamePhysics();
    /*! Renders debug game shapes
     */
    void tryRenderDebugShapes() const;
    /*! Update projectiles objects
     */
    void updateProjectiles() const;
    /*! Tries to load identical screen database 
        \param[in] loaded a flags for loaded databases
        \param[in] db_name a database name
        \param[in] file_name a file, from which we should load database
        \param[in] is_inventory_thread inventory thread
     */
    void tryLoadIdenticalScreenDatabase(bool* loaded, const sad::String& db_name, const sad::String& file_name, bool is_inventory_thread) const;
    /*! Disabled constructor
     */
    Game(const Game&);
    /*! Disabled copy operator
        \return self-reference
     */
    Game& operator=(const Game&);
    /*! A communication object for main thread
     */
    threads::GameThread* m_main_thread;
    /*! A communication object for inventory thread
     */
    threads::GameThread* m_inventory_thread;
    /*! Whether game is quitting
     */
    bool m_is_quitting;

    /*!  A main menu state for main menu
     */
    Game::MainMenuState  m_main_menu_state;
    /*! A mappping, that maps a state index from Game::MainMenuState
        to a label, where should be placed marker (player stuff)
     */
    sad::Hash<size_t, sad::String> m_main_menu_states_to_labels;

    /*! A state machine for searching data
     */
    sad::hfsm::Machine m_state_machine;
    /*! A state machine for paused state
     */
    sad::hfsm::Machine m_paused_state_machine;
    /*! Set score for game
     */
    int m_score;
    /*! A highest score for game
     */
    int m_highscore;
    /*! Whether, we loaded options database
     */
    bool m_loaded_options_database[2];
    /*! Whether, we loaded lose screen database
     */
    bool m_loaded_lose_screen_database[2];
    /*! Whether we loaded win screen database
     */
    bool m_loaded_win_screen_database[2];
    /*! Whether we loaded game screen
     */
    bool m_loaded_game_screen;

    /*! A theme sound, that should be played
     */
    sad::irrklang::SingleSound m_theme;
    /*! A theme playing song
     */
    ::irrklang::ISound* m_theme_playing;
    /*! A footsteps sound is playing
     */
    sad::irrklang::SingleSound m_footsteps;

    /*! A transition process
     */
    SceneTransitionProcess* m_transition_process;
    /*! An inventory node
     */
    nodes::InventoryNode* m_inventory_node;
    /*! An inventory popup
     */
    nodes::InventoryPopup* m_inventory_popup;

    /*! A global game options
     */
    game::Options m_options;
    /*! An ingame used key conditions
     */
    game::Conditions m_conditions;
    /*! A player, that will be used in game
     */
    game::Player* m_player;
    /*! A registry, which holds all data for moving platforms
     */
    game::MovingPlatformRegistry m_moving_platform_registry;
    /*! A local triggers for game
     */
    game::Triggers m_triggers;
    /*! A list of actors in game
     */
    game::Actors m_actors;
    /*! An in-game walls
     */
    game::Walls m_walls;
    /*! An options screen
     */
    OptionsScreen m_options_screen;
    /*! A context for game
     */
    sad::dukpp03::Context* m_eval_context;
    /*! An actor options list;
     */
    sad::Hash<sad::String, game::ActorOptions*> m_actor_options;
    /*! An item definitions
     */
    sad::Hash<sad::String, game::Item::Definition*> m_item_definitions;
    /*! A bot registry for storing all types for bots
     */
    bots::BotRegistry m_bot_registry;
    /*! A world, which is used in game
     */
    sad::p2d::World* m_physics_world;
    /*! A world step task for pipeline
     */
    sad::p2d::WorldStepTask* m_step_task;
    /*! A bounce solver for solving collisions
     */
    sad::p2d::BounceSolver* m_bounce_solver;
    /*! A bounce solver for bullets
     */
    sad::p2d::BounceSolver* m_bounce_solver_for_bullets;
    /*! Whether we should render world bodies
     */
    bool m_is_rendering_world_bodies;
    /*! Maximal level x
     */ 
    double m_max_level_x;
    /*! A task lock
     */
    threads::TaskLock m_task_lock;
    /*! A swings list
     */
    sad::Vector<weapons::Projectile*> m_projectiles;
    /*! An animation for hitting the enemies
     */
    sad::animations::Blinking* m_hit_animation_for_enemies;
    /*! An animation for hitting the players
     */
    sad::animations::Blinking* m_hit_animation_for_players;
    /*! Unanimated coins for game
     */
    game::UnanimatedCoins m_unanimated_coins;
    /*! A list of delayed tasks
     */
    DelayedTasks m_delayed_tasks;
    /*! A score bar
     */
    game::ScoreBar* m_score_bar;
};
