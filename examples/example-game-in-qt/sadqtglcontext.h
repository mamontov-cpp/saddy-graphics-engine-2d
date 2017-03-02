/*! \file sadqtglcontext.h

     Describes an OpenGL context, that is used with Qt windows
*/
#pragma once
#include "glcontext.h"


namespace sad
{
	
namespace qt
{

/*! Describes an OpenGL context, that is used with Qt windows
 */	
class GLContext: public sad::GLContext
{
public:	
	/*! Creates new context
	 */
	GLContext();
	/*! Frees resources, associated with context
	 */
	virtual ~GLContext();
	/*! Returns whether createFor() has ben called
	    \return flag
	 */
	virtual bool valid() const;
	/*! Does nothing, since Qt swaps buffers for us
	 */
	virtual void swapBuffers();
	/*! Maps a point from client coordinates to OpenGL viewport
	    \param[in] p point
	    \param[in] ztest whether ztesting should be performed
	    \return mapped point in vierport
	 */
	virtual sad::Point3D mapToViewport(const sad::Point2D & p, bool ztest = false);
	/*! Returns NULL
	    \return NULL
	 */
	virtual sad::os::GLContextHandle * handle() const;
	/*! Returns a window, attached to context
	    \return window, attached to context
	 */
	virtual sad::Window * window() const;
	/*! Always returns true
	    \return true
	 */
	virtual bool isOpenGL3compatible() const;
	/*! Sets current window to win. Returns true
	    \param[in] win a window
	    \return true
	 */
	virtual bool createFor(sad::Window * win);
	/*! Does nothing
	 */
	virtual void destroy();
	/*! Does nothing
	 */
	virtual void makeCurrent();
	/*! Returns a renderer, associated with context
	    \return renderer
	 */
	virtual sad::Renderer * renderer() const;
protected:
	/*! A pointer to window
	 */
	sad::Window* m_window;
	/*! A valid flag for context
	 */
	bool m_valid;
};


}

}


