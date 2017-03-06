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
	virtual bool valid() const;

	/*! Returns true, since by default widget has fixed size
	    \return true
	 */
	virtual bool fixed() const;
	/*! Does nothing, since we cannot change how Qt window works
	 */
	virtual void makeFixedSize();
	/*! Does nothing, since we cannot change how Qt window works
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
	/*! Tests, whether implementation is compatible with OpenGL 3
		\return whether OpenGL 3+ context can be created in this window
	 */
	virtual bool isGL3compatible() const;
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
		\return whether destruction was successfull
	*/
	virtual void destroy();
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
