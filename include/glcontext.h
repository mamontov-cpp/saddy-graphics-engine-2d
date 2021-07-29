/*! \file glcontext.h
    

    Contains implementation of OpenGL context
 */
#pragma once
#include "sadpoint.h"

namespace sad
{

namespace os
{
class GLContextImpl;
class GLContextHandle;
class MainLoopImpl;
}

class Renderer;
class Window;

/*! An OpenGL context, which could be created for window 
    or destroyed. Also contains swap buffers function
 */
class GLContext
{
friend class sad::Renderer;
friend class sad::os::MainLoopImpl;
public:
    /*! Creates new invalid context
     */
    GLContext();
    /*! Frees resources, associated with context
     */
    virtual ~GLContext();
    /*! Checks, whether context is valid
     */
    virtual bool valid() const;
    /*! Swaps buffers. Must be called, after scene is rendered
     */
    virtual void swapBuffers();
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
    /*! Sets implementation. Use it to replace default implementation, to
        replace construction step and other. Don't do it, when already created
        context!
        \param[in] i implementation
     */
    void setImplementation(sad::os::GLContextImpl * i);
    /*! Tries to create context for specified window. Does nothing, if it's
        already created. DO NOT USE IT!
        \param[in] win a window
        \return whether it was successful
     */
    virtual bool createFor(sad::Window * win);
    /*! Destroys a context. DO NOT USE IT!
     */
    virtual void destroy();
    /*! Makes context current 
     */
    virtual void makeCurrent();
    /*! Returns a renderer, associated with context
        \return renderer
     */
    virtual sad::Renderer * renderer() const;
private:
    /*! A pointer to implementation of OpenGL context
     */
    sad::os::GLContextImpl * m_dptr;
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
     */
    GLContext(const sad::GLContext & o);
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other window
        \return self-reference
     */	
    sad::GLContext & operator=(const sad::GLContext & o);
};

}