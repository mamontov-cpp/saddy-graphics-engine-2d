/*! \file shaderimpl.h
 *  
 *  Implementation of simple shader
 */
#pragma once

#include "../sadstring.h"
#include "../maybe.h"

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

class Renderer;

namespace os
{

class ExtensionFunctions;

/*! A simple shader implementation for data
 */
class ShaderImpl
{
public:
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
    /*! Sets renderer for data
     *  \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r);
    /*! Returns renderer for data
     *  \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Sets vertex program for shader
     *  \param[in] vertexProgram a vertex program
     */
    void setVertexProgram(const sad::String& vertexProgram);
    /*! Clears vertex program
     */
    void clearVertexProgram();
    /*! Sets fragment program for shader
     *  \param[in] fragmentProgram a fragment program
     */
    void setFragmentProgram(const sad::String& fragmentProgram);
    /*! Clears fragment program
     */
    void clearFragmentProgram();
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadVertexProgramFromFile(const sad::String& fileName);
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadFragmentProgramFromFile(const sad::String& fileName);
    /*! Tries to upload shader on GPU
     */
    void tryUpload();
    /*! Uses shader implementation 
     */
    void use();
    /*! Disables shader
     */
    void disable();
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1,  unsigned int v2);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2, int v3);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2, float v3);
    /*! Methods for freeing resources on GPU
     */
    void tryDestroy();
private:
    /*! Tries to log OpenGL error if occured
     */
    void tryLogGlError(const char* op);
    /*! Tries to compile shader
     *  \param[in] shader_type type of shader
     *  \param[in] program_text a program text
     */
    GLuint tryCompileShader(GLenum shader_type, const sad::String& program_text) const;
    /*! Uses program, denoted by identifier, or disables it, using 0
     *  \param[in] program a program
     *  \param[in] force forces program, disable if needed
     */
    void useProgram(GLuint program, bool force = false);
    /*! Returns extension functions
     *  \return extension functions
     */
    sad::os::ExtensionFunctions* f() const;
    /*! Invokes extension functions with callback
     *  \param[in] a argument
     *  \param[in] location_name a name for location
     *  \param[in] v0 first argument
     */
    template<typename T, typename A> 
    void invoke(T a, const sad::String& location_name, A v0);
    /*! Invokes extension functions with callback
     *  \param[in] a argument
     *  \param[in] location_name a name for location
     *  \param[in] v0 first argument
     *  \param[in] v1 second argument
     */
    template<typename T, typename A>
    void invoke(T a, const sad::String& location_name, A v0, A v1);
    /*! Invokes extension functions with callback
     *  \param[in] a argument
     *  \param[in] location_name a name for location
     *  \param[in] v0 first argument
     *  \param[in] v1 second argument
     *  \param[in] v2 third argument
     */
    template<typename T, typename A>
    void invoke(T a, const sad::String& location_name, A v0, A v1, A v2);
    /*! Invokes extension functions with callback
     *  \param[in] a argument
     *  \param[in] location_name a name for location
     *  \param[in] v0 first argument
     *  \param[in] v1 second argument
     *  \param[in] v2 third argument
     *  \param[in] v3 fourth argument
     */
    template<typename T, typename A>
    void invoke(T a, const sad::String& location_name, A v0, A v1, A v2, A v3);
    /*! Test if shader is on GPU
     */
    bool m_is_on_gpu;
    /*! A local program identifier
     */
    GLuint m_program;
    /*! A vertex shader program text
     */
    sad::Maybe<sad::String> m_vertex_program;
    /*! A fragment shader program text
     */
    sad::Maybe<sad::String> m_fragment_program;
    /*! A renderer for implementation
     */
    sad::Renderer* m_renderer;
};

}

}