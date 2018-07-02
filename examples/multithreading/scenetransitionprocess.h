/*! \file scenetransitionprocess.h
    
    A process, determining a scene transition work
 */
#pragma once
#include "scenetransitionoptions.h"
#include <renderer.h>
#include <sadthread.h>
#include <sprite2d.h>
#include <sadsleep.h>

class Game;

/*! A class, that makes transitions between scenes easy
 */
class SceneTransitionProcess
{
public:
/*!  A thread data for process 
 */
struct ThreadData
{
    /*! A renderer part
     */
    sad::Renderer* Renderer;
    /*! A thread, used for loading data and synchronization
     */
    sad::Thread* Thread;
    /*! A local texture, used to store data, needed for sprite
     */
    sad::Texture* Texture;
    /*! A sprite for brightening or darkening objects
     */
    sad::Sprite2D* Sprite;
    /*! Whether thread data executed on loaded actions
     */
    bool ExecutedOnLoaded;
    /*!  Whether thread data executed on finished actions
     */
    bool ExecutedOnFinished;
};
    /*! Constructs new process, based on game
     *  \param[in] game a game object
     */
    SceneTransitionProcess(Game* game);
    /*! Execute process with specified options
     */
    void start(const SceneTransitionOptions& options);
    /*! Unloads textures for main thread
     */
    void unloadTexturesForMainThread() const;
    /*! Unloads textures for inventory thread
     */
    void unloadTexturesForInventoryThread() const;
private:
    /*! Unloads texture if it's on GPU
        \param[in] t texture
     */
    static void unloadTextureIfOnGPU(sad::Texture* t);
    /*! A parent game options
     */
    Game* m_game;
    /*! A data for main thread
     */
    ThreadData m_main_thread_data;
    /*! An inventory thread data
     */
    ThreadData m_inventory_thread_data;
    /*! An options for scene transitions
     */
    SceneTransitionOptions m_options;
};
