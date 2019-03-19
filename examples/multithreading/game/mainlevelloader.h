/*! \file mainlevelloader.h
 *
 *  A main loader level data
 */
#pragma once
#include <renderer.h>
#include <sadthread.h>

namespace game
{

/*! A loader for main level
 */
class MainLevelLoader
{
public:
    /*! Constructs new loader for main level
     *  \param[in] renderer a renderer data
     */
    MainLevelLoader(sad::Renderer* renderer);
    /*! Frees thread data
     */
    ~MainLevelLoader();
    /*! Runs loader thread for level
     */
    void runLoaderThread();
    /*! Loads game screen from data
     */
    void loadGameScreen();
private:
    /*! Whether we already loaded game screen
     */
    bool m_loaded_game_screen;
    /*! Whether we loaded game screen first time
     */
    bool m_first_time_loaded_game_screen;
    /*! A main thread for loading level in background
     */
    sad::Thread* m_thread;
    /*! A renderer for loader
     */
    sad::Renderer* m_renderer;
};

}
