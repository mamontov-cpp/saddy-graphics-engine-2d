/*  \file glcontextimpl.h
	\author HiddenSeeker

	Contains implementation for OpenGL context for window
 */
#pragma once
#include "glcontexthandle.h"
#include "../window.h"


namespace sad
{

namespace os
{

/*! An OpenGL context, which could be created for window 
	or destroyed. Also contains swap buffers function
 */
class GLContextImpl
{
public:
	/*! Creates new invalid context
	 */
	GLContextImpl();

	/*! Destroys system implementation for gl context
	 */
	~GLContextImpl();

	/*! Tries to create context for specified window. Does nothing, if it's
		already created
		\param[in] win a window
		\return whether it was successfull
	 */
	bool createFor(sad::Window * win);
	/*! Destroys a context
	 */
	void destroy();
	/*! Checks, whether context is valid
	 */
	bool valid() const;
	/*! Swaps buffers. Must be called, after scene is rendered
	 */
	void swapBuffers();
	/*! Maps a point from client coordinates to OpenGL viewport
		\param[in] p point
		\param[in] ztest whether ztesting should be performed
		\return mapped point in vierport
	 */
	sad::Point3D mapToViewport(const sad::Point2D & p, bool ztest = false);
	/*! Returns current OpenGL context
		\return OpenGL context
	 */
	sad::os::GLContextHandle * handle() const;
	/*! Returns a window, attached to context
		\return window, attached to context
	 */
	sad::Window * window() const;
private:
	/*! A handle for implementation
	 */
	sad::os::GLContextHandle m_handle;
	/*! A window, attached when sad::os::GLContextImpl::createFor called
	 */
	sad::Window * m_win;
	/*! Cannot be copied, so this is disabled and not implemented
		\param[in] o other window
	 */
	GLContextImpl(const sad::os::GLContextImpl & o);
	/*! Cannot be copied, so this is disabled and not implemented
		\param[in] o other window
		\return self-reference
	 */	
	sad::os::GLContextImpl & operator=(const sad::os::GLContextImpl & o);
};

}

}
