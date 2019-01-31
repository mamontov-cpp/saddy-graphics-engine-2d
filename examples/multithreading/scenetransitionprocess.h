/*! \file scenetransitionprocess.h

    A process, determining a scene transition work
 */
#pragma once
#include "scenetransitionoptions.h"
#include <renderer.h>
#include <sadthread.h>
#include <sprite2d.h>
#include <sadmutex.h>
#include <animations/animationscolor.h>
#include <animations/animationsinstance.h>


class Game;
/*! A constant for making animation, true if we want darkening
 */
#define DARKENING (true)
/*! A constant for making animation, false if we want lightening
 */
#define LIGHTENING (false)

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
    /*! A lightening animation for thread
     */
    sad::animations::Color* LighteningAnimation;
    /*! A lightening animation instance for thread
     */
    sad::animations::Instance* LighteningAnimationInstance;
    /*! A darkening animation for thread
     */
    sad::animations::Color* DarkeningAnimation;
    /*! A darkening animation instance for thread
     */
    sad::animations::Instance* DarkeningAnimationInstance;
    /*! Whether thread finished waiting for loading
    */
    sad::Mutex LoadWaitingLock;
    /*! Whether thread data executed on loaded actions
     */
    bool ExecutedOnLoaded;
    /*!  Whether thread data executed on finished actions
     */
    bool FinishedDarkening;
};
    /*! Constructs new process, based on game
     *  \param[in] game a game object
     */
    SceneTransitionProcess(Game* game);
    /*! Fress data for transition process
     */
    ~SceneTransitionProcess();
    /*! Execute process with specified options.
        \param[in] options an options for call
     */
    void start(const SceneTransitionOptions& options);
    /*! Unloads textures for main thread
     */
    void unloadTexturesForMainThread() const;
    /*! Unloads textures for inventory thread
     */
    void unloadTexturesForInventoryThread() const;
    /*! Returns last active scene for renderer
     *  \param[in] r renderer
     *  \return last active scene
     */
    static sad::Scene* lastActiveScene(sad::Renderer* r);
private:
    /*! Unloads texture if it's on GPU
        \param[in] t texture
     */
    static void unloadTextureIfOnGPU(sad::Texture* t);
    /*! Makes white 2x2 texture for renderer
        \param[in] r renderer
        \return a
     */
    static sad::Texture* makeTextureForRenderer(sad::Renderer* r);
    /*! Fills thread data process with relevant stuff
        \param[out] data a filled data
        \param[in] r renderer
     */
    static void fillThreadData(SceneTransitionProcess::ThreadData* data, sad::Renderer* r);
    /*! Destroys a thread data
        \param[in] data a data
     */
    static void destroyThreadData(SceneTransitionProcess::ThreadData* data);
    /*! Makes covering sprite for texture
        \param[in] t texture
        \return sprite
     */
    static sad::Sprite2D* makeCoveringSpriteForTexture(sad::Texture* t);
    /*! Makes animation for transition, use constants DARKENING or LIGHTENING
        to define type
        \param[in] darkening whether animation should be darkening
        \return animation
     */
    static sad::animations::Color* makeAnimation(bool darkening);
    /*! Makes instance with specified animation and sprite
        \param[in] c color
        \param[in] s sprite
     */
    static sad::animations::Instance* makeInstance(sad::animations::Color* c, sad::Sprite2D* s);
    /*!  A process is non-copyable
         \param[in] p process
     */
    SceneTransitionProcess(const SceneTransitionProcess& p);
    /*!  A process is non-copyable
         \param[in] p process
         \return self-reference
     */
    SceneTransitionProcess& operator=(const SceneTransitionProcess& p) const;
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
