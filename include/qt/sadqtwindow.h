/*! \file sadqtwindow.h
    
    A window stub, used to work with Qt's window
 */
#pragma once
#include <window.h>
#include <QWidget>
#include <sadstring.h>
#include <QStack>

namespace sad
{
    
namespace qt
{
    
/*! A window stub, used to work with Qt's window
 */
class Window: public sad::Window
{
public:	
    /*! Constructs new window object
     */
    Window();
    /*! Can be inherited
     */
    virtual ~Window();
    /*! Always returns true 
        \return true
     */
    virtual bool valid() const override;

    /*! Returns true, since by default widget has fixed size
        \return true
     */
    virtual bool fixed() const override;
    /*! Does nothing, since we cannot change how Qt window works
     */
    virtual void makeFixedSize() override;
    /*! Does nothing, since we cannot change how Qt window works
    */
    virtual void makeResizeable() override;

    /*! Tests, whether window is on fullscreen mode
    */
    virtual bool fullscreen() const override;
    /*! Enters fullscreen
    */
    virtual void enterFullscreen() override;
    /*! Leaves fullscreen
    */
    virtual void leaveFullscreen() override;

    /*! Tests, whether window is hidden. This does not detect minimized windows.
    */
    virtual bool hidden() const override;
    /*! Shows a window
    */
    virtual void show() override;
    /*! Hides a window
    */
    virtual void hide() override;

    /*! Sets rectangle to specified implementation
        \param[in] rect a new window rectangle
    */
    virtual void setRect(const sad::Rect2I& rect) override;
    /*! Pushes window rectangle down to stack of sizes
        \param[in] rect a new window rectangle
    */
    virtual void pushRect(const sad::Rect2I& rect) override;
    /*! Pops window rectangle from stack of sizes
    */
    virtual void popRect() override;
    /*! Returns window rectangle
        \return window rectangle
    */
    virtual sad::Rect2I rect() const override;

    /*! Maps a point from window coordinates to client coordinates
        \param[in] p point
        \return client point
    */
    virtual sad::Point2D toClient(const sad::Point2D & p) override;

    /*! Returns a handles for a window
        \return handles for a window
    */
    virtual sad::os::WindowHandles * handles() override;


    /*! Returns a title for a window
        \return title for a window
     */
    virtual const sad::String & title() const override;
    /*! Sets title for a window
     */
    virtual void setTitle(const sad::String & s) override;
    /*! Tests, whether implementation is compatible with OpenGL 3
        \return whether OpenGL 3+ context can be created in this window
     */
    virtual bool isGL3compatible() const override;
    /*! Creates a new window. Note, that you should not call this functions, since
        sad::Renderer automatically does it for you.
        \return whether creation was successfull. DO NOT USE IT!
    */
    virtual bool create() override;
    /*! Closes a window. Note, that you should not call this function, use
        sad::Renderer instead for shutting down everything. DO NOT USE IT!
    */
    virtual void close() override;
    /*! Destroys a window, freeing all resources.
        Note, that you should not call this functions, since
        sad::Renderer automatically does it for you. DO NOT USE IT!
        \return whether destruction was successfull
    */
    virtual void destroy() override;
protected: 
    /*! Returns parent widget for a window
        \return widget
     */
    QWidget* widget() const;
    /*! Returns Qt window for this window
        \return window
     */
    QWidget* window() const;
    /*! A cached window title value for window
     */
    sad::String m_window_title;
    /*! Whether it's fullscreen
     */
    bool m_fullscreen;
    /*! A sizes stack for window
     */
    QStack<sad::Rect2I> m_sizes;
};


}

}
