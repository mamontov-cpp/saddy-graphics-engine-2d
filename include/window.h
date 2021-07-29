/*! \file window.h
    
    
    Describes a window, where rendering should be performed
 */ 
#pragma once
#include "sadsize.h"
#include "sadrect.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "sadvector.h"
#include "sadstring.h"

namespace sad
{

class Renderer;

namespace os
{
class WindowHandles;
class WindowImpl;
class MainLoopImpl;
}

/*! A window, where rendering should be performed
 */
class Window
{
friend class sad::Renderer;
friend class sad::os::MainLoopImpl;
public:
    /*! Creates a new default window
     */
    Window();
    /*! Frees inner implementation pointer
     */
    virtual ~Window();

    /*! Sets a linked renderer
        \param[in] renderer linked renderer
     */
    void setRenderer(sad::Renderer * renderer);
    /*! Returns a renderer for a window
        \return linked renderer
     */
    sad::Renderer * renderer() const;


    /*! Sets a size, which window will be created with
        \param[in] size a window size
     */ 
    void setCreationSize(const sad::Size2I& size);
    /*! Sets a size, which window will be created with
        \param[in] width a width for a window
        \param[in] height a height for a window
     */ 
    void setCreationSize(unsigned int width, unsigned int height);
    /*! Tests, whether window is valid. Valid window - is a window, for each a
        sad::Window::create was called successfully
        \return whether window is valid
     */
    virtual bool valid() const;


    /*! Tests, whether window is fixed size
     */
    virtual bool fixed() const;
    /*! Toggles fixed size window on
     */
    virtual void makeFixedSize();
    /*! Toggles fixed size window off
     */
    virtual void makeResizeable();

    /*! Tests, whether window is on fullscreen mode
     */
    virtual bool fullscreen() const;
    /*! Enters fullscreen
     */
    virtual void enterFullscreen();
    /*! Leaves fullscreen
     */
    virtual void leaveFullscreen();

    /*! Tests, whether window is hidden. This does not detect minimized windows.
     */
    virtual bool hidden() const;
    /*! Shows a window
     */
    virtual void show();
    /*! Hides a window
     */
    virtual void hide();

    /*! Sets rectangle to specified implementation
        \param[in] rect a new window rectangle
     */
    virtual void setRect(const sad::Rect2I& rect);
    /*! Pushes window rectangle down to stack of sizes
        \param[in] rect a new window rectangle
     */
    virtual void pushRect(const sad::Rect2I& rect);
    /*! Pops window rectangle from stack of sizes
     */
    virtual void popRect();
    /*! Returns window rectangle 
        \return window rectangle
     */
    virtual sad::Rect2I rect() const;

    /*! Maps a point from window coordinates to client coordinates
        \param[in] p point
        \return client point
     */
    virtual sad::Point2D toClient(const sad::Point2D & p);
    /*! Returns a handles for a window
        \return handles for a window
     */
    virtual sad::os::WindowHandles * handles();
    /*! Returns a title for a window
        \return title for a window
     */
    virtual const sad::String & title() const;
    /*! Sets title for a window
     */
    virtual void setTitle(const sad::String & s);
    /*! Whether window is active
        \return whether window is active
     */
    virtual bool active() const;
    /*! Tests, whether implementation is compatible with OpenGL 3
        \return whether OpenGL 3+ context can be created in this window
     */
    virtual bool isGL3compatible() const;
    /*! Sets implementation. Use it to replace default implementation, to
        replace construction step and other. Don't do it, when already created
        window!
        \param[in] i implementation
     */
    void setImplementation(sad::os::WindowImpl * i);
    /*! Called by a renderer to initialize created window settings, enter fullscreen,
        make window fixed. DO NOT USE IT!
     */
    virtual void initialize();
    /*! Creates a new window. Note, that you should not call this functions, since
        sad::Renderer automatically does it for you.
        \return whether creation was successfull. DO NOT USE IT!
     */
    virtual bool create();
    /*! Closes a window. Note, that you should not call this function, use
        sad::Renderer instead for shutting down everything. DO NOT USE IT!
     */
    virtual void close();
    /*! Destroys a window, freeing all resources. 
        Note, that you should not call this functions, since
        sad::Renderer automatically does it for you. DO NOT USE IT!
     */
    virtual void destroy();
    /*! Used by loop to mark window as active or not
        \param[in] active whether window is active
     */
    virtual void setActive(bool active);
    /*! Sets hidden flag in window 
        \param[in] hidden whether window is hidden
     */
    virtual void setHidden(bool hidden);
    /*! Tests, whether window is minimized
        \return whether window is minimized
     */
    virtual bool minimized() const;
    /*! Sets minimized flag in window
        \param[in] minimized  whether window is minimized
     */
    virtual void setMinimized(bool minimized);
private:
    /*! A pointer to implementation of window
     */
    sad::os::WindowImpl * m_dptr;
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
     */
    Window(const sad::Window & o);
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
        \return self-reference
     */	
    sad::Window & operator=(const sad::Window & o);
};

}
