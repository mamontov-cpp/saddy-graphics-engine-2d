/*! \file game.h

    Main game class
 */
#pragma once

#include <functional>

namespace threads
{
class GameThread;
}

/*! Main game class
 */
class Game
{
public:
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
	/*! Quits game
	*/
    void quitGame();
    /*! Transitions the game from current scene to the next one
        \param[in] darkeningTime period of time during wich the screen will darken, ms
        \param[in] loadNewData functon for loading elements for the new scene
        \param[in] actionsAfterTransition function with actions that will be done after loading data
    */
    void changeScene(long darkeningTime, std::function<void()> loadNewData, std::function<void()> actionsAfterTransition);	
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
};
