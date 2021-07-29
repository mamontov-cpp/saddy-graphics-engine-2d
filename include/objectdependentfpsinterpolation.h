/*! \file fpsinterpolation.h
    

    A class for computing FPS, with timing and interpolation between frames
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
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
    virtual ~ObjectDependentFPSInterpolation() override;
    /*! Starts new ObjectFPSInterpolation iteration. Must be called at beginning
        of new main loop
     */
    virtual void reset() override;
    /*! Starts measuring time for computing time interval
     */
    virtual void start() override;
    /*! Stops measuring time for computing time interval
     */
    virtual void stop() override;
    /*! Resets timing for FPS interpolation. Used to remove FPS jumps, when
        window is not active
     */
    virtual void resetTimer() override;
    /*! Returns current FPS
        \return FPS
     */
    virtual double fps() override;
    /*! Sets a renderer for interpolation
     */
    void setRenderer(sad::Renderer * renderer);
protected:
    /*! Returns amount of rendered items per time
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
