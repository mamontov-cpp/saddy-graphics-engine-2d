/*! \file windowimpl.h
	\author HiddenSeeker

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
	~WindowImpl();

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

	/*! Creates a new window
		\return whether creation was successfull
	 */
	bool create();
	/*! Forcefully closes a window 
	 */
	void close();
	/*! Destroys a window
		\return whether destruction was successfull 
	 */
	void destroy();
	/*! Tests, whether window is valid
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
		\param[in] rect a rectangle parameters
	 */
	void setRect(const sad::Rect2I& rect);
	/*! Pushes current window rectangle down to stack of sizes and sets new 
		\param[in] rect a rectangle parameters
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
	/*! Used by loop to mark window as active or not
		\param[in] active whether window is active
	 */
	void setActive(bool active);
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
	/*! Size, that should be set, when creating a windo
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
private:

#ifdef WIN32
	/*! Registers WNDCLASS object via RegisterWindowClass, if last result is true
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool registerWindowClass(bool lastresult);
	/*! Adjust window rectangle for creation, if last result is true. Note, that 
		size is being taken from size
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool adjustWindowRect(bool lastresult);
	/*! Makes window and obtains device context
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool makeWindowAndObtainDeviceContext(bool lastresult);
	/*! Tries to unregister window class, also checking that it correctly filled
	 */
	void unregisterWindowClass();
	/*! Releases contexts and destroys window
	 */
	void releaseContextAndDestroyWindow();
	/*! Chooses and sets a pixel format descriptor for a window
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool chooseAndSetPixelFormatDescriptor(bool lastresult);
#endif

#ifdef X11
	/*! Opens a connection to X11 server and sets data from it
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool openConnectionAndScreen(bool lastresult);
	/*! Chooses visual information for result
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool chooseVisualInfo(bool lastresult);
	/*! Creates a window for result
		\param[in] lastresult result of last operation, whether it was successfull
		\return whether it was successfull
	 */
	bool createWindow(bool result);
	/*! Closes a connection for a window
	 */
	void closeConnection();
	/*! Sends WM-specific fullscreen event to window to
		turn it into fullscreen
		\param[in] fullscreen whether we are entering fullscreen (false if leaving)
	 */
	void sendNetWMFullscreenEvent(bool fullscreen);
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
