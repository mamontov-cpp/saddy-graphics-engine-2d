/*  \file glcontextimpl.h
    

    Contains implementation for OpenGL context for window
 */
#pragma once
#include "glcontexthandle.h"
#include "../window.h"

#ifdef X11
    typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*); 
#endif

namespace sad
{

class Renderer;

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
    virtual ~GLContextImpl();

    /*! Tries to create context for specified window. Does nothing, if it's
        already created
        \param[in] win a window
        \return whether it was successfull
     */
    virtual bool createFor(sad::Window * win);
    /*! Destroys a context
     */
    virtual void destroy();
    /*! Checks, whether context is valid
     */
    virtual bool valid() const;
    /*! Swaps buffers. Must be called, after scene is rendered
     */
    virtual void swapBuffers();
    /*! Makes context current for a window
     */
    virtual void makeCurrent();
    /*! Maps a point from client coordinates to OpenGL viewport
        \param[in] p point
        \param[in] ztest whether ztesting should be performed
        \return mapped point in viewport
     */
    virtual sad::Point3D mapToViewport(const sad::Point2D & p, bool ztest = false);
    /*! Returns current OpenGL context
        \return OpenGL context
     */
    virtual sad::os::GLContextHandle * handle() const;
    /*! Returns a window, attached to context
        \return window, attached to context
     */
    virtual sad::Window * window() const;
    /*! Tests, whether OpenGL context is compatible with OpenGL 3.0+ specs
        \return whether it's compatible
     */
    virtual bool isOpenGL3compatible() const;
    /*! Returns current attached renderer, if has one
     */
    virtual sad::Renderer * renderer() const;
protected:
    /*! A handle for implementation
     */
    sad::os::GLContextHandle m_handle;
    /*! A window, attached when sad::os::GLContextImpl::createFor called
     */
    sad::Window * m_win;
    /*! Tests, whether context is opengl 3 compatible
     */
    bool m_isopengl3compatible;
#ifdef X11
    /*! Tries to make default context
     */
    bool makeDefaultContext();


    /*! Tries to create opengl 3 context
     */
    bool tryCreateOpenGL3Context(glXCreateContextAttribsARBProc cc);
#endif
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
