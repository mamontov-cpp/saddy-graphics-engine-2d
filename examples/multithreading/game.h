/*! \file game.h

    Main game class
 */
#pragma once
#include <sadthread.h>

#include <functional>

#include <sprite2d.h>

#include <sadhash.h>

#include <hfsm/hfsmmachine.h>

#include <irrklang/singlesound.h>
#include <irrklang/engine.h>

#include <keymouseconditions.h>

#include "game/conditions.h"

#include "scenetransitionprocess.h"


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

    /*! Returns renderer for main thread
     */
    sad::Renderer* rendererForMainThread() const;
    /*! Returns renderer for inventory thread
     */
    sad::Renderer* rendererForInventoryThread() const;
private:
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
    /*! Loaded options database
     */
    bool m_loaded_options_database[2];

    /*! A theme sound, that should be played
     */
    sad::irrklang::SingleSound m_theme;
    /*! A theme playing song
     */
    ::irrklang::ISound* m_theme_playing;

    /*! A transition process
     */
    SceneTransitionProcess* m_transition_process;

    /*! A global game options
     */
    game::Options m_options;
    /*! An ingame used key conditions
     */
    game::Conditions m_conditions;
};
