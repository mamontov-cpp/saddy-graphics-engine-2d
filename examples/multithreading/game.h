/*! \file game.h

    Main game class
 */
#pragma once
#include <sprite2d.h>
#include <sadhash.h>
#include <sadmutex.h>

#include <hfsm/hfsmmachine.h>

#include <irrklang/singlesound.h>
#include <irrklang/engine.h>

#include <dukpp-03/context.h>

#include <p2d/world.h>
#include <p2d/worldsteptask.h>
#include <p2d/bouncesolver.h>

#include "game/conditions.h"
#include "game/player.h"

#include "scenetransitionprocess.h"
#include "optionsscreen.h"


namespace game
{
class Item;    
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
     */
    int highscore() const;
    /*! Sets new highscore for a game
        \param[in] highscore a highscore
     */
    void setHighscore(int highscore);
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
    /*! Constructs new item
    *  \param[in] icon an icon for item
    *  \param[in] title a title for item
    *  \param[in] description a description for item
    *  \param[in] delete_after_apply whether we should apply an item data
    *  \return new item
    */
    game::Item* makeItem(const sad::String& icon, const sad::String& title, const sad::String& description, bool delete_after_apply = false);
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
private:
    /*! Tries to get script for item
     *  \param[in] title item's title
     *  \return name for title
     */
    sad::String* tryGetScriptForItem(const sad::String& title);
    /*! Destroys physics world
     */
    void destroyWorld();
    /*! Inits in-game physics
     */
    void initGamePhysics();
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
    /*! A highestscore for game
     */
    int m_highscore;
    /*! Whether, we loaded options database
     */
    bool m_loaded_options_database[2];
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
    game::Player* m_player{NULL};
    /*! An options screen
     */
    OptionsScreen m_options_screen;
    /*! A context for game
     */
    sad::dukpp03::Context* m_eval_context;
    /*! An item names to related scripts for item
     */
    sad::Hash<sad::String, sad::String*> m_item_names_to_scripts;
    /*! A world, which is used in game
     */
    sad::p2d::World* m_physics_world;
    /*! A world step task for pipeline
     */
    sad::p2d::WorldStepTask* m_step_task;
    /*! A bounce solver for solving collisions
     */
    sad::p2d::BounceSolver* m_bounce_solver;
    /*! Whether we should render world bodies
     */
    bool m_is_rendering_world_bodies;
    /*! Maximal level x
     */ 
    double max_level_x;
    /*! Amount of running tasks in pipeline at the moment
     */
    size_t m_running_tasks;
    /*! A lock for running tasks counter
     */
    sad::Mutex m_running_tasks_lock;
};
