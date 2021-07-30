/* \file sadqtrenderer.h

    Describes a renderer, compatible with sad::qt::OpenGLWidget
*/
#pragma once
#include <renderer.h>
#include <input/events.h>
// ReSharper disable once CppUnusedIncludeDirective
#include "sadqtglcontext.h"

namespace sad
{
    
namespace qt
{

class OpenGLWidget;
    
class Renderer: public sad::Renderer
{
public:
    /*! Makes new renderer, compatible with widget
     */
    Renderer();
    /*! Could be inherited
     */
    virtual ~Renderer() override;
    /*! Returns whether initializing call has been performed
        \return whether renderer was initialized
     */
    bool initialized() const;
    /*! Initializes renderer with specified settings
        \param[in] _settings settings
        \return Success of operation
    */
    virtual void init(const sad::Settings& _settings) override;
    /*! Initializes renderer before loop. Called by widget to initialize OpenGL
        \return true
     */
    virtual bool initRendererBeforeLoop() override;
    /*! Runs a renderer's loop only once
     */
    virtual void runOnce() override;
    /*! Disabled. User should not call this function, due to event loop being handled by Qt
     */
    virtual bool run() override;
    /*! Destroys window and context, makes required cleanups, when renderer is done.
        Call this, after main loop of renderer is finished
    */
    virtual void deinitRendererAfterLoop() override;
    /*! Sets linked widget
        \param[in] widget a widget
     */
    virtual void setWidget(sad::qt::OpenGLWidget* widget);
    /*! Returns linked widget
        \return linked widget
     */
    sad::qt::OpenGLWidget* widget() const;
    /*! Called, when starting rendering
     */
    void startRendering() override;
    /*! Called, when finishing rendering
     */
    void finishRendering() override;
    /*! Initializes OpenGL rendering
     */
    // ReSharper disable once CppInconsistentNaming
    // ReSharper disable once CppHidingFunction
    void initGLRendering();
    /*! Reshape a system of coordinates to deal with width and height
        \param[in] width Needed width
        \param[in] height Needed height
     */
    virtual void reshape(int width, int height) override;
    /*! Returns viewport matrix
        \return viewport matrix
     */
    inline int* viewport()
    {
        return m_viewport;
    }
    /*! Submits new event into main loop
        \param[in] t event type
        \param[in] ev event
        \param[in] now whether we should handle event now
     */
    virtual void submitEvent(sad::input::EventType t, sad::input::AbstractEvent* ev, bool now);
    /*! Returns default camera for projection
        \return camera
     */
    sad::Camera* getDefaultCamera() const;
    /*! Submits event of specified type
        \param[in] ev event
        \param[in] now whether we should handle event now
     */
    template<
        typename T
    >
    void submitEvent(T* ev, bool now = false)
    {
        submitEvent(sad::input::EnumValueForEventType<T>::Type, ev, now);
    }
protected:
    /*! A cached viewport
     */
    int m_viewport[4];
    /*! Whether renderer was initialized, via init
     */
    bool m_initialized;
    /*! A widget
     */
    sad::qt::OpenGLWidget* m_widget;
};

}

}

