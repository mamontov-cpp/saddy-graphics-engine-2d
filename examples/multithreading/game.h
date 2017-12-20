/*! \file game.h

    Main game class
 */
#pragma once
#include <renderer.h>
#include <mutex.h>
#include <sadsleep.h>

/*! Main game class
 */
class Game
{
// TOO MUCH LOGIC HERE! THIS IS BAD
/*! A result of initializing thread state
 */
struct ThreadStateInitializationResult
{
     sad::Renderer* Renderer; //!< A main renderer
     sad::String Error;       //!< A error information for log
     bool        Ok;          //!< Whether initialization was successfull

     /*! Constructs new result
      */
     inline ThreadStateInitializationResult() : Renderer(NULL), Ok(true)
     {

     }

     /*! Copying moves value of renderer to out state
         \param[in] o other object
      */
     inline ThreadStateInitializationResult(const Game::ThreadStateInitializationResult& o) : Renderer(o.Renderer), Error(o.Error), Ok(o.Ok)
     {
        const_cast<Game::ThreadStateInitializationResult&>(o).Renderer = NULL;
     }

     inline Game::ThreadStateInitializationResult& operator=()

     /*! Destroys result
      */
     inline ~ThreadStateInitializationResult()
     {
          delete Renderer;
     }
};
/*! A state of game initialization
 */
struct GameStateInitializationResult
{
    Game::ThreadStateInitializationResult MainThreadResult;       //!< A result for main thread
    Game::ThreadStateInitializationResult InventoryThreadResult;  //!< A result for inventory thread
};
/*! A local awaitable thread state
 */
struct ThreadState
{
    sad::Renderer* Renderer; //!< A  main renderer for state

    // This is actually complicated, but used to perform basic thread
    // synchronization. Basically, when thread wants to wait for other thread
    // it calls waitForSignal() on this threads state.
    // An another thread, when it wonts tp notify this thread it calls this state's
    // notify.

    sad::Mutex Lock;  //!  A lock for atomic change of awaitable flag
    bool NeedToWait; //!< A flag which notifies, that we are waiting for flag

    /*! Makes a new thread state
     */
    inline ThreadState() : Renderer(NULL), NeedToWait(false)
    {

    }

    /*! Makes called thread sleep, until other thread calls notify()
     */
    void waitForSignal()
    {
        Lock.lock();
        NeedToWait = true;
        Lock.unlock();
        bool need_to_wait = true;
        while (need_to_wait)
        {
            sad::sleep(100);

            Lock.lock();
            need_to_wait = NeedToWait;
            Lock.unlock();
        }
    }

    /*! Notifies other thread, that we may not need to wait anymore
     */
    void notify()
    {
        Lock.lock();
        NeedToWait = false;
        Lock.unlock();
    }


    /*! Destroys a thread state, frees memory from renderer
     */
    inline ~ThreadState()
    {
        delete Renderer;
    }

    /*! Sets renderer from state
     */
    inline void setRendererFromState(Game:ThreadStateInitializationResult& state)
    {
        this->Renderer = state.Renderer;
        state.Renderer = NULL;
    };
};
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
private:
    /*! Disabled constructor
     */
    Game(const Game&);
    /*! Disabled copy operator
        \return self-reference
     */
    Game& operator=(const Game&);
    /*! Makes a renderer for a game
        \param[in] log_name a name for a log file
        \param[in] window_title a title for window
     */
    sad::Renderer* makeRenderer(
        const sad::String& log_name,
        const sad::String& window_title
    );
    /*! A renderer for main game
     */
    sad::Renderer* m_main_game_renderer;
    /*! A result of renderer initialization
     */
    Game::GameStateInitializationResult m_init_result;
    /*! A state of main thread
     */
    Game::ThreadState m_main_thread_state;
    /*! A state for inventory thread
     */
    Game::ThreadState m_inventory_thread_state;

    sad::Renderer* tryInitializeThreadState(
        const sad::String& log_name,
        const sad::String& window_title
    );

};
