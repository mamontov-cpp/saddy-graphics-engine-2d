/*! \file shader.h
 *
 *  Interface of simple shader
 */
#pragma once
#include "maybe.h"
#include "sadstring.h"
#include "object.h"

namespace sad
{

class Renderer;

namespace os
{
class ExtensionFunctions;
class ShaderImpl;

}

/*! A simple shader interface
 */
class Shader: public sad::Object
{
SAD_OBJECT
public:
    /*! Makes new implementation
     */
    Shader();
    /*! Copies shader implementation
     *  \param[in] o other implementation
     */
    Shader(const sad::Shader& o);
    /*! Copies other shader implementation
     *  \param[in] o other implementation
     *  \return self-reference
     */
    sad::Shader& operator=(const sad::Shader& o);
    /*! Frees an implementation
     */
    ~Shader();
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
    /*! Clears vertex program
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
    /*! Tries to destroy shader from GPU
     */
    void tryDestroy();
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
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2);
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
    void tryDestroy() const;
    /*! Returns uniform location by name, or null if not found
     *  \param[in] name a name for location
     *  \return  location or 0 if not found
     */
   int getUniformLocation(const sad::String& name);
   /*! Returns uniform block index
       \param[in] name name of index
       \return result (GL_INVALID_INDEX)
    */
   unsigned int getUniformBlockIndex(const sad::String& name);
   /*! Assigns a binding point to an active uniform block
      \param[in] uniformBlockIndex a block index
      \param[in] uniformBlockBinding a binding point
    */
   void uniformBlockBinding(unsigned int uniformBlockIndex, unsigned int uniformBlockBinding);
   /*! Analogue for glUniformMatrix4x3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix4x3(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix3x4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix3x4(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix4x2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix4x2(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix2x4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix2x4(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix3x2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix3x2(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix2x3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix2x3(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix4(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix3(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniformMatrix2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] transpose transpose
    * \param[in] value value array
    */
   void setUniformMatrix2(int location, int count, bool transpose,const float* value);
   /*! Analogue for glUniform4uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform4(int location, int count, const unsigned int* value);
   /*! Analogue for glUniform3uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform3(int location, int count, const unsigned int* value);
   /*! Analogue for glUniform2uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform2(int location, int count, const unsigned int* value);
   /*! Analogue for glUniform1uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform1(int location, int count, const unsigned int* value);
   /*! Analogue for glUniform4iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform4(int location, int count, const int* value);
   /*! Analogue for glUniform3iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform3(int location, int count, const int* value);
   /*! Analogue for glUniform2iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform2(int location, int count, const int* value);
   /*! Analogue for glUniform1iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform1(int location, int count, const int* value);
   /*! Analogue for glUniform4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform4(int location, int count, const float* value);
   /*! Analogue for glUniform3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform3(int location, int count, const float* value);
   /*! Analogue for glUniform2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform2(int location, int count, const float* value);
   /*! Analogue for glUniform1fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location a location
    * \param[in] count a count
    * \param[in] value value array
    */
   void setUniform1(int location, int count, const float* value);
   /*! Analogue for glUniform4ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    * \param[in] v3 v3
    */
   void setUniform(int location, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
   /*! Analogue for glUniform3ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    */
   void setUniform(int location, unsigned int v0, unsigned int v1, unsigned int v2);
   /*! Analogue for glUniform2ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    */
   void setUniform(int location, unsigned int v0, unsigned int v1);
   /*! Analogue for glUniform1ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    */
   void setUniform(int location, unsigned int v0);
   /*! Analogue for glUniform4i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    * \param[in] v3 v3
    */
   void setUniform(int location, int v0, int v1, int v2, int v3);
   /*! Analogue for glUniform3i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    */
   void setUniform(int location, int v0, int v1, int v2);
   /*! Analogue for glUniform2i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    */
   void setUniform(int location, int v0, int v1);
   /*! Analogue for glUniform1i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    */
   void setUniform(int location, int v0);
   /*! Analogue for glUniform4f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    * \param[in] v3 v3
    */
   void setUniform(int location, float v0, float v1, float v2, float v3);
   /*! Analogue for glUniform3f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    * \param[in] v2 v2
    */
   void setUniform(int location, float v0, float v1, float v2);
   /*! Analogue for glUniform2f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    * \param[in] v1 v1
    */
   void setUniform(int location, float v0, float v1);
   /*! Analogue for glUniform1f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
    * \param[in] location location
    * \param[in] v0 v0
    */
   void setUniform(int location, float v0);
   /*! Tries to log OpenGL error if occurred
    */
   void tryLogGlError(const char* op) const;
   /*! Returns shader id
       \return shader id
    */
   unsigned int shaderId() const;
private:
    /*! Tries to compile shader
      *  \param[in] shader_type type of shader
      *  \param[in] program_text a program text
      */
    unsigned int tryCompileShader(unsigned int shader_type, const sad::String& program_text) const;
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
    unsigned int m_program;
    /*! A vertex shader program text
     */
    sad::Maybe<sad::String> m_vertex_program;
    /*! A fragment shader program text
     */
    sad::Maybe<sad::String> m_fragment_program;
    /*! A renderer for implementation
     */
    sad::Renderer* m_renderer;
    /*!  A functions for calls
     */
    sad::os::ExtensionFunctions* m_f;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Shader)
