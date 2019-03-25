/*! \file gamethread.h

    Describes a main thread for a game
 */
#pragma once
#include <renderer.h>
#include <log/log.h>
#include <sadmutex.h>
#include <maybe.h>

#include "gamestate.h"

namespace threads
{
/*! A game thread object, that is used to perform
    communications between games
 */
class GameThread
{
public:
    /*! A new game thread
     */
    GameThread();
    /*! Destroys a thread
     */
    ~GameThread();
    /*! Marks, that this thread should be notified later
     */
    void needsToBeNotifiedFromLater();
    /*! Blocks called thread, until this threads calls notify()
     */
    void waitForNotify();
    /*! Notifies a thread, that we finished waiting
     */
    void notify();
    /*! Whether some errors occured
     */
    bool hasErrors() const;
    /*! Returns errors, occurred in thread
     */
    const sad::String& errors() const;
    /*! Whether this thread is killed
     */
    bool isKilled() const;
    /*! Tries to initialize a current thread
        \param[in] log_prefix a log prefix for console
        \param[in] log_name a name for log item
        \param[in] config_name a config item
        \param[in] window_title a title for window
     */
    void tryInitialize(
        const sad::String& log_prefix,
        const sad::String& log_name,
        const sad::String& config_name,
        const sad::String& window_title
    );
    /*! Kills a thread
        \param[in] other other thread, to call notify upon
     */
    void sendKillSignalFrom(threads::GameThread* other);
    /*! Forces thread to resume
        \param[in] other other thread, to call notify upon
     */
    void sendResumeSignalFrom(threads::GameThread* other);
    /*! Returns new Renderer
        \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Marks that renderer is started
     */
    void markAsRendererStarted();
    /*! Sets a gane state
     */
    void setState(threads::GameState state);
    /*! Returns a state
     */
    threads::GameState state() const;
protected:
    /*! An inner game state for a thread
     */
    threads::GameState m_state;
    /*! Whether there was an error in some thread
     */
    sad::Maybe<sad::String> m_error;
    /*! Whether thread is killed
     */
    bool m_killed;
    /*! A renderer part
     */
    sad::Renderer* m_renderer;
    /*! A mutex for locking m_need_to_wait
     */
    sad::Mutex m_need_to_wait_lock;
    /*! Whether other thread, that called wait needs to wait
     */
    bool m_need_to_wait;
    /*! Whether renderer for this thread already started
     */
    bool m_renderer_started;
};

}
