/*! \file fpsinterpolation.h
	\author HiddenSeeker

	A class for computing FPS, with timing and interpolation between frames
 */
#pragma once
#include "timer.h"
#include "fpsinterpolation.h"
#include "maybe.h"
namespace sad
{

class Renderer;

/*! Performs computing FPS, with timing and interpolation of FPS between frames
 */
class ObjectDependentFPSInterpolation: public sad::FPSInterpolation
{
public:
	/*! Creates new non-started FPS computation
	 */
	ObjectDependentFPSInterpolation();
	/*! Can be inherited
	 */
	virtual ~ObjectDependentFPSInterpolation();
	/*! Starts new ObjectFPSInterpolation iteration. Must be called at beginning
		of new main loop
	 */
	virtual void reset();
	/*! Starts measuring time for computing time interval
	 */
	virtual void start();
	/*! Stops measuring time for computing time interval
	 */
	virtual void stop();
	/*! Resets timing for FPS interpolation. Used to remove FPS jumps, when
		window is not active
	 */
	virtual void resetTimer();
	/*! Returns current FPS
		\return FPS
	 */
	virtual double fps();
	/*! Sets a renderer for interpolation
	 */
	void setRenderer(sad::Renderer * renderer);
protected:
	/*! A timer, for computng FPS
	 */
	sad::Timer		     m_timer;  
	/*! An elapsed frames count
	 */
	int                  m_frames;      
	/*! Whether timer should be reset on next iteration
	 */
	bool                 m_reset;          
	/*! Whether we should immediately set FPS 
	 */
	bool				 m_setimmediately; 
	/*!  Current FPS value
	 */
	double               m_fps; 

	/*! Returns amount of rendererd items per time
	 */
	unsigned int m_total_renderer_items;
	/*! Returns an interval of renderer per item
	 */
	sad::Maybe<double> m_interval_per_item;
	/*! Sets a renderer for interpolation
	 */
	sad::Renderer * m_renderer;
};

}
