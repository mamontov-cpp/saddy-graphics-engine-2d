/*! \file windowimpl.h
    

    Contains OS-dependent implementation of window
 */
#pragma once
#include "windowhandles.h"
#include "../sadsize.h"
#include "../sadrect.h"
#include "../sadvector.h"

namespace sad
{

class Window;
class Renderer;

namespace os
{

class GLContextImpl;

/*! Describes an OS-dependent implementation of window
 */
class WindowImpl
{
friend class sad::Window;
public:
    /*! Creates new non-created window implementation. 
        Default creation size will be 320x480
     */
    WindowImpl();
    /*! Frees all resources, which are not freed
     */
    virtual ~WindowImpl();

    /*! Sets a linked renderer
        \param[in] renderer linked renderer
     */
    virtual void setRenderer(sad::Renderer * renderer);
    /*! Returns a renderer for a window
        \return linked renderer
     */
    virtual sad::Renderer * renderer() const;

    /*! Sets a size, which window will be created with
        \param[in] size a window size
     */ 
    virtual void setCreationSize(const sad::Size2I& size);

    /*! Creates a new window
        \return whether creation was successfull
     */
    virtual bool create();
    /*! Forcefully closes a window 
     */
    virtual void close();
    /*! Destroys a window
        \return whether destruction was successfull 
     */
    virtual void destroy();
    /*! Tests, whether window is valid
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
        \param[in] rect a rectangle parameters
     */
    virtual void setRect(const sad::Rect2I& rect);
    /*! Pushes current window rectangle down to stack of sizes and sets new 
        \param[in] rect a rectangle parameters
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
    /*! Tests, whether implementation is compatible with OpenGL 3
        \return whether OpenGL 3+ context can be created in this window
     */
    virtual bool isGL3compatible() const;
private:
    /*! A handles for a window
     */
    sad::os::WindowHandles m_handles;
    /*! Whether window is active
     */
    bool m_active;
    /*! Whether window is fixed
     */
    bool m_fixed;
    /*! Whether window is in fullscreen mode
     */
    bool m_fullscreen;
    /*! Whether window is hidden
     */
    bool m_hidden;
    /*! Whether window is minimized
     */
    bool m_minimized;
    /*! Size, that should be set, when creating a window
     */
    sad::Size2I  m_creation_size;
    /*! A window title for window
     */
    sad::String  m_window_title;
    /*! A stack of window rectangles. Should be changed when 
        toggling fullscreen or other times
     */
    sad::Vector<sad::Rect2I> m_window_rect_stack;
    /*! A linked renderer
     */
    sad::Renderer * m_renderer;
protected:

#ifdef WIN32
    LONG_PTR m_style;
    /*! Registers WNDCLASS object via RegisterWindowClass, if last result is true
        \param[in] last_result result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool registerWindowClass(bool last_result);
    /*! Adjust window rectangle for creation, if last result is true. Note, that 
        size is being taken from size
        \param[in] last_result result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool adjustWindowRect(bool last_result);
    /*! Makes window and obtains device context
        \param[in] last_result result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool makeWindowAndObtainDeviceContext(bool last_result);
    /*! Tries to unregister window class, also checking that it correctly filled
     */
    virtual void unregisterWindowClass();
    /*! Releases contexts and destroys window
     */
    virtual void releaseContextAndDestroyWindow();
    /*! Chooses and sets a pixel format descriptor for a window
        \param[in] last_result result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool chooseAndSetPixelFormatDescriptor(bool last_result);
#endif

#ifdef X11

    bool m_gl3compatible;
    /*! Opens a connection to X11 server and sets data from it
        \param[in] lastresult result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool openConnectionAndScreen(bool lastresult);
    /*! Chooses visual information for result
        \param[in] lastresult result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool chooseVisualInfo(bool lastresult);
    /*! Creates a window for result
        \param[in] lastresult result of last operation, whether it was successfull
        \return whether it was successfull
     */
    virtual bool createWindow(bool result);
    /*! Closes a connection for a window
     */
    virtual void closeConnection();
    /*! Sends WM-specific fullscreen event to window to
        turn it into fullscreen
        \param[in] fullscreen whether we are entering fullscreen (false if leaving)
     */
    virtual void sendNetWMFullscreenEvent(bool fullscreen);
    /*! Picks best GLXFBConfig from a list
        \param[in] fbcount of configs
        \param[in] configs list of configs
        \return index of best config
     */
    int pickBestFBConfig(int fbcount, GLXFBConfig * configs);
#endif
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
     */
    WindowImpl(const sad::os::WindowImpl & o);
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
        \return self-reference
     */	
    sad::os::WindowImpl & operator=(const sad::os::WindowImpl & o);
};

}

}
