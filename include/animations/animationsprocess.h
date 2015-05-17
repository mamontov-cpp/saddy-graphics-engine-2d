/*! \file animations/animationsprocess.h
	\author HiddenSeeker

	Defines a process for animations as element of animations pipeline
 */ 
#pragma once

namespace sad
{
	
namespace animations
{
class Animations;
/*! A process for animations as element of animations pipeline
 */
class Process
{
public:
	/*! Must be inherited
	 */
	virtual ~Process();
	/*! Restarts an animation process
        \param[in] animations an animations, which will invoke sad::animations::Process::process on object
	 */
    virtual void restart(sad::animations::Animations* animations) = 0;
	/*! Clears finished flag
     */
    virtual void clearFinished() = 0;
    /*! Whether animation is finished
     */
    virtual bool finished() const = 0;
    /*! Called on every step of instance work
        \param[in] animations an animation pipeline, which should hold a state cache
     */
    virtual void process(sad::animations::Animations* animations) = 0;
    /*! Pauses an instance
     */
    virtual void pause() = 0;
    /*! Resumes an instance
     */
    virtual void resume() = 0;
    /*! Cancels an animation
		\param[in] animations an animations to be used
     */
    virtual void cancel(sad::animations::Animations* animations) = 0;
	/*! Called, when process is added to pipeline
	 */
	virtual void addedToPipeline() = 0;
	/*! Called, when process is removed from pipeline
	 */
	virtual void removedFromPipeline() = 0;
};

}

}
