/*! \file systemeventdispatcher.h
    

    Defines a dispatcher, which dispatches a system events
 */
#pragma once
#include "../maybe.h"
#include "../sadsize.h"
#include "../keycodes.h"
#include "../sadpoint.h"
#include "../timer.h"

namespace sad
{

class Renderer;
class MainLoop;

namespace os
{

class SystemWindowEvent;
class KeyDecoder;

/*! Determines, whether event is dispatched, and also result code for
    this dispatch operation, when it's needed to be provided to main loop
 */
typedef sad::Maybe<long> SystemWindowEventDispatchResult;

/*! A system event dispatcher is a dispatcher, which dispatches
    window system events. Also it's used to handle some corner cases, which
    we shouldn't handle by input system but should handle to make other API stuff 
    useful (like fixed size windows on Windows).
 */ 
class SystemEventDispatcher
{
public:
    friend class sad::MainLoop;
    /*! Creates default event dispatcher
     */
    SystemEventDispatcher();
    /*! Destroys a dispatcher
     */
    ~SystemEventDispatcher();
    /*! Sets attached renderer to a dispatcher
     */
    void setRenderer(sad::Renderer * renderer);
    /*! Returns renderer, attached to dispatcher
     */
    sad::Renderer * renderer() const;
    /*! Starts new iteration of main loop for working with window properties.
        Must be called at start of main loop.
     */
    void reset();
    /*! Dispatches an event
        \param[in] e event, that is being dispatched
        \return result of event, that was dispatched
     */
    sad::os::SystemWindowEventDispatchResult dispatch(SystemWindowEvent & e);
protected:
    /*! Converts point to client point
     */
    sad::Point2D toClient(const sad::Point2D& p) const;
    /*! Processes event, when window is being closed
        \param[in] e system event
     */
    void processQuit(SystemWindowEvent & e);
    /*! Processes event, when window gains focus
        \param[in] e system event
     */ 
    void processActivate(SystemWindowEvent & e);
    /*! Processes event, when window loses focus
        \param[in] e system event
     */ 
    void processDeactivate(SystemWindowEvent & e);	
    /*! Processes event, when user moves a mount
        \param[in] e system event
     */
    void processMouseMove(SystemWindowEvent & e);
    /*! Processes event, when mouse leaves a window
        \param[in] e system event
     */
    void processMouseLeave(SystemWindowEvent & e);
    /*! Processes event, when user scrolls a wheel
        \param[in] e system event
     */
    void processMouseWheel(SystemWindowEvent & e);
    /*! Processes event, when user resizes window
        \param[in] e system event
     */
    void processResize(SystemWindowEvent & e);
    /*! Processes event, when user presses a keyboard button
        \param[in] e system event
     */
    void processKeyPress(SystemWindowEvent & e);
    /*! Processes event, when user releases a keyboard button
        \param[in] e system event
     */
    void processKeyRelease(SystemWindowEvent & e);
    /*! Processes event, when user presees a mouse button
        \param[in] e system event
     */ 
    void processMousePress(SystemWindowEvent & e);
    /*! Processes event, when user presees releases a mouse button
        \param[in] e system event
     */ 
    void processMouseRelease(SystemWindowEvent & e);
#ifdef X11
    /*! Processes event, when user presees enters a window
        \param[in] e system event
     */ 
    void processMouseEnter(SystemWindowEvent & e);
#endif
#ifdef WIN32
    /*! Processes event, when user performs a double click with a mouse button
        Note, that X11 cannot recognize double click, so we are switching to simple
        implementation via sensivity.
        \param[in] e system event
     */ 
    void processMouseDoubleClick(SystemWindowEvent & e);
    /*! Processes hit testing in Win32, making window non-resizable if needed
        \param[in] e event
        \return whether a new region if needed
     */
    sad::os::SystemWindowEventDispatchResult  processHitTest(SystemWindowEvent & e) const;
#endif
    /*! Attached renderer to a dispatcher
     */
    sad::Renderer * m_renderer;
    /*! A decoder for decoding keys for press events
     */
    sad::os::KeyDecoder * m_decoder_for_keypress_events;
    /*! A decoder for decoding keys for release events
     */
    sad::os::KeyDecoder * m_decoder_for_keyrelease_events;
#ifdef WIN32
    /*! Determines, whether window cursor is within window
     */
    bool m_is_in_window;
#endif
#ifdef X11
    /*! Determines, whether user holds alt button
     */
    bool m_alt_is_held;
    /*! A timer for working with doubleclick
     */
    sad::Timer m_doubleclick_timer;
    /*! Determines, whether we could perform double click (one click is performed)
     */
    bool m_in_doubleclick;
    /*! A button which could be double clicked in some key
     */
    sad::MouseButton m_doubleclick_button;
#endif
    /*! Old window size
     */
    sad::Size2I m_old_window_size;

};

}

}

