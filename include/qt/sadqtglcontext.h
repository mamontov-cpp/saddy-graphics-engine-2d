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
    virtual ~GLContext() override;
    /*! Returns whether createFor() has ben called
        \return flag
     */
    virtual bool valid() const override;
    /*! Does nothing, since Qt swaps buffers for us
     */
    virtual void swapBuffers() override;
    /*! Maps a point from client coordinates to OpenGL viewport
        \param[in] p point
        \param[in] ztest whether ztesting should be performed
        \return mapped point in viewport
     */
    virtual sad::Point3D mapToViewport(const sad::Point2D & p, bool ztest = false) override;
    /*! Returns nullptr
        \return nullptr
     */
    virtual sad::os::GLContextHandle * handle() const override;
    /*! Returns a window, attached to context
        \return window, attached to context
     */
    virtual sad::Window * window() const override;
    /*! Always returns true
        \return true
     */
    virtual bool isOpenGL3compatible() const override;
    /*! Sets current window to win. Returns true
        \param[in] win a window
        \return true
     */
    virtual bool createFor(sad::Window * win) override;
    /*! Does nothing
     */
    virtual void destroy() override;
    /*! Does nothing
     */
    virtual void makeCurrent() override;
    /*! Returns a renderer, associated with context
        \return renderer
     */
    virtual sad::Renderer * renderer() const override;
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


