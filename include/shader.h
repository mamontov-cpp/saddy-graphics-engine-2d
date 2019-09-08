/*! \file shader.h
 *
 *  Interface of simple shader
 */
#pragma once

#include "sadstring.h"

#include "object.h"

namespace sad
{

class Renderer;

namespace os
{

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
    void setRenderer(sad::Renderer* r) const;
    /*! Returns renderer for data
     *  \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Sets vertex program for shader
     *  \param[in] vertexProgram a vertex program
     */
    void setVertexProgram(const sad::String& vertexProgram) const;
    /*! Clears vertex program
     */
    void clearVertexProgram() const;
    /*! Sets fragment program for shader
     *  \param[in] fragmentProgram a fragment program
     */
    void setFragmentProgram(const sad::String& fragmentProgram) const;
    /*! Clears vertex program
     */
    void clearFragmentProgram() const;
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadVertexProgramFromFile(const sad::String& fileName) const;
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadFragmentProgramFromFile(const sad::String& fileName) const;
    /*! Tries to upload shader on GPU
     */
    void tryUpload() const;
    /*! Tries to destroy shader from GPU
     */
    void tryDestroy();
    /*! Uses shader implementation
     */
    void use() const;
    /*! Disables shader
     */
    void disable() const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2, int v3) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2, float v3) const;
    /*! Methods for freeing resources on GPU
     */
    void tryDestroy() const;
    /*! Returns uniform location by name, or null if not found
     *  \param[in] name a name for location
     *  \return  location or 0 if not found
     */
   int getUniformLocation(const sad::String& name);
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
   /*! Tries to log OpenGL error if occured
    */
   void tryLogGlError(const char* op);
private:
    /*! Implementation of shader
     */
    sad::os::ShaderImpl* m_impl;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Shader)
