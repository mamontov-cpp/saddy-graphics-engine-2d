/*! \file cameralockanimation.h
 *   
 *  A camera locking animation
 */
#pragma once
#include <pipeline/pipelinestep.h>
#include <pausabletimer.h>

class Game;
class CameraMovement;

class CameraLockAnimation: public sad::pipeline::Step
{
public:
    /*! Constructs new animation
     *  \param[in] g game
     *  \param[in] finishing_offset a finishing offset for lock
     *  \param[in] total_time a total time
     */
    CameraLockAnimation(Game* g, double finishing_offset, double total_time);
    /*!  Fress local resources
     */
    virtual ~CameraLockAnimation();
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing();
private:
    /*! Reimplement this function to create your own step actions
     */
    virtual void _process();
    /*! A game data
     */
    Game* m_game;
    /*! A starting offset
     */
    double m_starting_offset;
    /*! A finishing offset
     */
    double m_finishing_offset;
    /*! A total time
     */
    double m_total_time;
    /*! An inner paused timer
     */
    sad::PausableTimer m_pausable_timer;
    /*! Whether animation if finished?
     */
    bool m_finished;
};