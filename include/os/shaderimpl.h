/*! \file shaderimpl.h
 *  
 *  Implementation of simple shader
 */
#pragma once

#include "../sadstring.h"

#ifdef WIN32
    #ifndef NOMINMAX
        #define NOMINMAX 
    #endif
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#endif

#ifdef X11
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glx.h>
#endif

#include <3rdparty/glext/glext.h>

namespace sad
{

namespace os
{

/*! A simple shader implementation for data
 */
class ShaderImpl
{
    /*! Makes new implementation
     */
    ShaderImpl();
    /*! Copies shader implementation
     *  \param[in] o other implementation
     */
    ShaderImpl(const sad::os::ShaderImpl& o);
    /*! Copies other shader implementation
     *  \param[in] o other implementation
     *  \return self-reference
     */
    sad::os::ShaderImpl& operator=(const sad::os::ShaderImpl& o);
    /*! Frees an implementation
     */
    ~ShaderImpl();
private:
    /*! Methods for freeing resources on GPU
     */
    void tryDestroy();
    /*! Test if shader is on GPU
     */
    bool m_is_on_gpu;
    /*! A local program identifier
     */
    GLuint m_program;
    /*! A vertex shader program text
     */
    sad::String m_vertex_program;
    /*! A fragment shader program text
     */
    sad::String m_fragment_program;
};

}

}