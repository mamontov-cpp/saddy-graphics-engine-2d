/*! \file game.h

    Main game class
 */
#pragma once
#include <sadthread.h>

#include <functional>

#include <sprite2d.h>

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
    /*! Transitions the game from current scene to the next one
        \param[in] renderer renderer for animation
        \param[in] time time period for animation
        \param[in] dark if true - transition for bolder color, false - sprite will fade into nothing
        \return animation
    */
    sad::animations::Instance* setAnimationForScreenTransition(sad::Renderer &renderer, long time, bool dark);
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
    /*! For storing loading data lambda functions
     */
    void loadingDataFunction();
    /*! Function object for storing lambdas
     */
    std::function<void()> local_actions;
    /*! Additional thread for data loading
     */
    sad::Thread *loadDataThread;
};
