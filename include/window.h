/*! \file window.h
	\author HiddenSeeker
	
	Describes a window, where rendering should be performed
 */ 
#pragma once
#include "sadsize.h"
#include "sadrect.h"
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
	~Window();

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
	/*! Tests, whether window is valid. Valid window - is a window, for each a
		sad::Window::create was called successfully
		\return whether window is valid
	 */
	bool valid() const;


	/*! Tests, whether window is fixed size
	 */
	bool fixed() const;
	/*! Toggles fixed size window on
	 */
	void makeFixedSize();
	/*! Toggles fixed size window off
	 */
	void makeResizeable();

	/*! Tests, whether window is on fullscreen mode
	 */
	bool fullscreen() const;
	/*! Enters fullscreen
	 */
	void enterFullscreen();
	/*! Leaves fullscreen
	 */
	void leaveFullscreen();

	/*! Tests, whether window is hidden. This does not detect minimized windows.
	 */
	bool hidden() const;
	/*! Shows a window
	 */
	void show();
	/*! Hides a window
	 */
	void hide();

	/*! Sets rectangle to specified implementation
		\param[in] rect a new window rectangle
	 */
	void setRect(const sad::Rect2I& rect);
	/*! Pushes window rectangle down to stack of sizes
		\param[in] rect a new window rectangle
	 */
	void pushRect(const sad::Rect2I& rect);
	/*! Pops window rectangle from stack of sizes
	 */
	void popRect();
	/*! Returns window rectangle 
		\return window rectangle
	 */
	sad::Rect2I rect() const;

	/*! Maps a point from window coordinates to client coordinates
		\param[in] p point
		\return client point
	 */
	sad::Point2D toClient(const sad::Point2D & p);
	/*! Returns a handles for a window
		\return handles for a window
	 */
	sad::os::WindowHandles * handles();
	/*! Returns a title for a window
		\return title for a window
	 */
	const sad::String & title() const;
	/*! Sets title for a window
	 */
	void setTitle(const sad::String & s);
	/*! Whether window is active
		\return whether window is active
	 */
	bool active() const;
	/*! Sets implementation. Use it to replace default implementation, to
		replace construction step and other. Don't do it, when already created
		window!
		\param[in] i implementation
	 */
	void setImplementation(sad::os::WindowImpl * i);
// TODO: Uncomment, when integrating in renderer
//#ifndef DO_NOT_HIDE_RENDERER_METHODS
//protected:
//#endif
	/*! Called by a renderer to initialize created window settings, enter fullscreen,
		make window fixed
	 */
	void initialize();
	/*! Creates a new window. Note, that you should not call this functions, since
		sad::Renderer automatically does it for you.
		\return whether creation was successfull
	 */
	bool create();
	/*! Closes a window. Note, that you should not call this function, use
		sad::Renderer instead for shutting down everything.
	 */
	void close();
	/*! Destroys a window, freeing all resources. 
	    Note, that you should not call this functions, since
		sad::Renderer automatically does it for you.
		\return whether destruction was successfull 
	 */
	void destroy();
	/*! Used by loop to mark window as active or not
		\param[in] active whether window is active
	 */
	void setActive(bool active);
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
