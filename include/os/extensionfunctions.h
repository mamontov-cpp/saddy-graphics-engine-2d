/*! \file extensionfunctions.h
    
    An extensions functions, dynamically loaded from libraries
 */
#pragma once
#include "glheaders.h"
#include "../sadstring.h"
#include "../sadmutex.h"

namespace sad
{
    
class OpenGL;
    
namespace os
{

/*! An extension functions, dynamically loaded from libraries
 */
class ExtensionFunctions
{
public:
    /*! Makes new extension functions
     */
    ExtensionFunctions();
    /*! Sets parent object for functions
        \param[in] ogl opengl functions
     */
    void setParent(sad::OpenGL* ogl);
    /*! Attempts to initialize functions
     */
    void tryInit();
    /*! Invokes glUniformMatrix4x3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix3x4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix4x2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix2x4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix3x2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix2x3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniformMatrix2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] transpose transpose
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value);
    /*! Invokes glUniform4uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform4uiv(GLint location, GLsizei count, const GLuint* value);
    /*! Invokes glUniform3uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform3uiv(GLint location, GLsizei count, const GLuint* value);
    /*! Invokes glUniform2uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform2uiv(GLint location, GLsizei count, const GLuint* value);
    /*! Invokes glUniform1uiv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform1uiv(GLint location, GLsizei count, const GLuint* value);
    /*! Invokes glUniform4iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform4iv(GLint location, GLsizei count, const GLint* value);
    /*! Invokes glUniform3iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform3iv(GLint location, GLsizei count, const GLint* value);
    /*! Invokes glUniform2iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform2iv(GLint location, GLsizei count, const GLint* value);
    /*! Invokes glUniform1iv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform1iv(GLint location, GLsizei count, const GLint* value);
    /*! Invokes glUniform4fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
    /*! Invokes glUniform3fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform3fv(GLint location, GLsizei count, const GLfloat* value);
    /*! Invokes glUniform2fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform2fv(GLint location, GLsizei count, const GLfloat* value);
    /*! Invokes glUniform1fv. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] count count
        \param[in] value value
        \throws exception if cannot be invoked
     */
    void glUniform1fv(GLint location, GLsizei count, const GLfloat* value);
    /*! Invokes glUniform4ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \param[in] v3 v3
        \throws exception if cannot be invoked
     */
    void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    /*! Invokes glUniform3ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \throws exception if cannot be invoked
     */
    void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
    /*! Invokes glUniform2ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \throws exception if cannot be invoked
     */
    void glUniform2ui(GLint location, GLuint v0, GLuint v1);
    /*! Invokes glUniform1ui. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \throws exception if cannot be invoked
     */
    void glUniform1ui(GLint location, GLuint v0);
    /*! Invokes glUniform4i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \param[in] v3 v3
        \throws exception if cannot be invoked
     */
    void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    /*! Invokes glUniform3i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \throws exception if cannot be invoked
     */
    void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
    /*! Invokes glUniform2i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \throws exception if cannot be invoked
     */
    void glUniform2i(GLint location, GLint v0, GLint v1);
    /*! Invokes glUniform1i. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \throws exception if cannot be invoked
     */
    void glUniform1i(GLint location, GLint v0);
    /*! Invokes glUniform4f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \param[in] v3 v3
        \throws exception if cannot be invoked
     */
    void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    /*! Invokes glUniform3f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \param[in] v2 v2
        \throws exception if cannot be invoked
     */
    void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    /*! Invokes glUniform2f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \param[in] v1 v1
        \throws exception if cannot be invoked
     */
    void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
    /*! Invokes glUniform1f. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniform.xhtml for details
        \param[in] location location
        \param[in] v0 v0
        \throws exception if cannot be invoked
     */
    void glUniform1f(GLint location, GLfloat v0);
    /*! Invokes glGetUniformLocation. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
        \param[in] program Specifies the program object to be queried
        \param[in] name Points to a null terminated string containing the name of the uniform variable whose location is to be queried
        \return uniform location
        \throws exception if cannot be invoked
     */
    GLint glGetUniformLocation(GLuint program, const GLchar* name);
    /*! Invokes glUseProgram. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
        \param[in] program Specifies the handle of the program object whose executables are to be used as part of current rendering state.
        \throws exception if cannot be invoked
     */
    void glUseProgram(GLuint program);
    /*! Invokes glGetProgramInfoLog. @see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetProgramInfoLog.xml
        \param[in] program   Specifies the program object whose information log is to be queried.
        \param[in] maxLength Specifies the size of the character buffer for storing the returned information log.
        \param[in] length Returns the length of the string returned in infoLog (excluding the null terminator).
        \param[in] infoLog Specifies an array of characters that is used to return the information log.
        \throws exception if cannot be invoked
     */
    void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    /*! Invokes glGetProgramiv. @see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetProgramiv.xml
        \param[in] program Specifies the program object to be queried.
        \param[in] pname   Specifies the object parameter.
        \param[in] params Returns the requested object parameter.
        \throws exception if cannot be invoked
     */
    void glGetProgramiv(GLuint program, GLenum pname, GLint* params);
    /*! Invokes glLinkProgram. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glLinkProgram.xhtml
        \param[in] program Specifies the handle of the program object to be linked.
        \throws exception if cannot be invoked
     */
    void glLinkProgram(GLuint program);
    /*! Invokes glAttachShader. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glAttachShader.xhtml
        \param[in] program Specifies the program object to which a shader object will be attached.
        \param[in] shader Specifies the shader object that is to be attached.
        \throws exception if cannot be invoked
     */
    void glAttachShader(GLuint program, GLuint shader);
    /*! Invokes glGetShaderInfoLog. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetShaderInfoLog.xhtml
        \param[in] shader    Specifies the shader object whose information log is to be queried.
        \param[in] maxLength Specifies the size of the character buffer for storing the returned information log.
        \param[in] length Returns the length of the string returned in infoLog (excluding the null terminator).
        \param[in] infoLog Specifies an array of characters that is used to return the information log.
        \throws exception if cannot be invoked
     */
    void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    /*! Invokes glGetShaderiv. @see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetShaderiv.xml
        \param[in] shader    Specifies the shader object whose information log is to be queried.
        \param[in] pname     Specifies the object parameter. Accepted symbolic names are GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH. 
        \param[in] params    Returns the requested object parameter.
        \throws exception if cannot be invoked
     */
    void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
    /*! Specifies the shader object to be compiled. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCompileShader.xhtml
        \param[in] shader Specifies the shader object to be compiled.
     */
    void glCompileShader(GLuint shader);
    /*! Replaces the source code in a shader object. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
        \param[in] shader Specifies the handle of the shader object whose source code is to be replaced.
        \param[in] count  Specifies the number of elements in the string and length arrays.
        \param[in] string Specifies an array of pointers to strings containing the source code to be loaded into the shader.
        \param[in] length Specifies an array of string lengths.
     */
    void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
    /*! Invokes glCreateShader. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
        \param[in] shaderType Specifies the type of shader to be created. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
        \return empty shader's handle
     */
    GLuint glCreateShader(GLenum shaderType);
    /*! Invokes glDeleteShader. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
        \param[in] shader Specifies the shader object to be deleted.
     */
    void glDeleteShader(GLuint shader);
    /*! Invokes glCreateProgram. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCreateProgram.xhtml
        \return empty program's handle
     */
    GLuint glCreateProgram();
    /*! Invokes glDeleteProgram. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
        \param[in] program Specifies the program object to be deleted.
     */
    void glDeleteProgram(GLuint program);
    /*! Invokes glActiveTexture. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glActiveTexture.xhtml
        \param[in] tex Specifies unit to be activated.
     */
    void glActiveTexture(GLenum tex);
    /*! Invokes glGenBuffers. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml
     *  \param[in] n Specifies the number of buffer object names to be generated.
     *  \param[out] buffers Specifies an array in which the generated buffer object names are stored.
     */
    void glGenBuffers(GLsizei n, GLuint* buffers);
    /*! Invokes glBindBuffer. @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
     *  \param[in] target a target buffer type
     *  \param[in] buffer Specifies the name of a buffer object.
     */
    void glBindBuffer(GLenum target, GLuint buffer);
    /*! Invokes glBufferData, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
     *  \param[in] target a target buffer type
     *  \param[in] size Specifies the size in bytes of the buffer object's new data store.
     *  \param[in] data Specifies a pointer to data that will be copied into the data store for initialization, or nullptr if no data is to be copied.
     *  \param[in] usage Specifies the expected usage pattern of the data store.
     */
    void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
    /*! Invokes glDeleteBuffers, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
     *  \param[in] n Specifies the number of buffer objects to be deleted.
     *  \param[in] buffers Specifies an array of buffer objects to be deleted.
     */
    void glDeleteBuffers(GLsizei n, const GLuint* buffers);
    /*! Invokes glEnableVertexAttribArray, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml
     *  \param[in] index Specifies the index of the generic vertex attribute to be enabled or disabled.
     */
    void glEnableVertexAttribArray(GLuint index);
    /*! Invokes glDisableVertexAttribArray, @see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glDisableVertexAttribArray.xml
     *  \param[in] index Specifies the index of the generic vertex attribute to be enabled or disabled.
     */
    void glDisableVertexAttribArray(GLuint index);
    /*! Invokes glVertexAttribPointer, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
     *  \param[in] index Specifies the index of the generic vertex attribute to be modified.
     *  \param[in] size Specifies the number of components per generic vertex attribute.
     *  \param[in] type Specifies the data type of each component in the array
     *  \param[in] normalized For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
     *  \param[in] stride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
     *  \param[in] pointer Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0
     */
    void glVertexAttribPointer(GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const GLvoid* pointer
    );
    /*! Invokes glMapBuffer, @see https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMapBuffer.xml
     *  \param[in] target Specifies the target buffer object being mapped. The symbolic constant must be GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, or GL_PIXEL_UNPACK_BUFFER.
     *  \param[in] access Specifies the access policy, indicating whether it will be possible to read from, write to, or both read from and write to the buffer object's mapped data store. The symbolic constant must be GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE.
     *  \return pointer to mapped memory
     */
    void* glMapBuffer(GLenum target, GLenum access);
    /*! Invokes, glUnmapBuffer, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUnmapBuffer.xhtml
     * \param target target Specifies the target buffer object being mapped. The symbolic constant must be GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, or GL_PIXEL_UNPACK_BUFFER.
     */
    void glUnmapBuffer(GLenum target);
    /*! Invokes glGenVertexArrays, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml
     * \param[in] n Specifies the number of vertex array object names to generate.
     * \param[out] arrays Specifies an array in which the generated vertex array object names are stored.
     */
    void glGenVertexArrays(GLsizei n, GLuint* arrays);
    /*! Invokes glBindVertexArray, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindVertexArray.xhtml
     * \param[in] array Specifies the name of the vertex array to bind.
     */
    void glBindVertexArray(GLuint array);
    /*! Invokes glDeleteVertexArrays, @see  https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml
     *  \param[in] n Specifies the number of vertex array objects to be deleted.
     *  \param[in] arrays Specifies the address of an array containing the n names of the objects to be deleted.
     */
    void glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
    /*! Invokes glGetUniformBlockIndex, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformBlockIndex.xhtml
     *  \param[in] program Specifies the name of a program containing the uniform block.
     *  \param[in] uniformBlockName Specifies the address an array of characters to containing the name of the uniform block whose index to retrieve.
     *  \return index of a uniform block within program
     */
    GLuint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);
    /*! Assign a binding point to an active uniform block, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniformBlockBinding.xhtml
     *  \param[in] program The name of a program object containing the active uniform block whose binding to assign.
     *  \param[in] uniformBlockIndex The index of the active uniform block within program whose binding to assign.
     *  \param[in] uniformBlockBinding Specifies the binding point to which to bind the uniform block with index uniformBlockIndex within program.
     */
    void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
    /*! Updates a subset of a buffer object's data store, @see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml
     *  \param[in] target Specifies the target to which the buffer object is bound for glBufferSubData
     *  \param[in] offset Specifies the offset into the buffer object's data store where data replacement will begin, measured in bytes
     *  \param[in] size   Specifies the size in bytes of the data store region being replaced
     *  \param[in] data   Specifies a pointer to the new data that will be copied into the data store
     */
    void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
    /*! Bind a buffer object to an indexed buffer target
     *  \param[in] target Specifies the target of the bind operation. target must be one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER
     *  \param[in] index  Specifies the index of the binding point within the array specified by target
     *  \param[in] buffer The name of a buffer object to bind to the specified binding point
     */
    void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
    /*! Bind a range within a buffer object to an indexed buffer target
     *  \param[in] target Specifies the target of the bind operation. target must be one of GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER, or GL_SHADER_STORAGE_BUFFER.
     *  \param[in] index  Specifies the index of the binding point within the array specified by target
     *  \param[in] buffer The name of a buffer object to bind to the specified binding point
     *  \param[in] offset The starting offset in basic machine units into the buffer object buffer
     *  \param[in] size   The amount of data in machine units that can be read from the buffer object while used as an indexed target
     */
    void glBindBufferRange(
        GLenum target,
        GLuint index,
        GLuint buffer,
        GLintptr offset,
        GLsizeiptr size
    );
    /*! Specifies the equation used for both the RGB blend equation and the Alpha blend equation
        \param[in] mode Specifies how source and destination colors are combined
     */
    void glBlendEquation(GLenum mode);
    /*! Binds a named sampler to texturing target
        \param[in] unit texture unit
        \param[in] sampler sampler
     */
    void glBindSampler(GLuint unit, GLuint sampler);
    /*! Sets the RGB blend equation and the alpha blend equation separately
        \param[in] modeRGB a mode for RGB
        \param[in] modeAlpha a mode for alpha
     */
    void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    /*! Specifies pixel arithmetic for RGB and alpha components separately
        \param[in] srcRGB Specifies how the red, green, and blue blending factors are computed. The initial value is GL_ONE.
        \param[in] dstRGB Specifies how the red, green, and blue destination blending factors are computed. The initial value is GL_ZERO.
        \param[in] srcAlpha Specifies how the alpha source blending factor is computed. The initial value is GL_ONE
        \param[in] dstAlpha Specifies how the alpha destination blending factor is computed. The initial value is GL_ZERO
     */
    void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    /*! Returns the location of an attribute variable
        \param[in] program a program object
        \param[in] name a name
     */
    GLint glGetAttribLocation(GLuint program, const GLchar* name);
    /*! Detaches shader from program
        \param[in] program a program name
        \param[in] shader a shader name
     */
    void glDetachShader(GLuint program, GLuint shader);
private:
    /*! Appends info that get proc address if failed to renderer (main if OpenGL) is not set
     */
    void showGetProcAddressFailedError(const sad::String& name) const;
    /*! glUniformMatrix4x3fv procedure
     */
    PFNGLUNIFORMMATRIX4X3FVPROC m_glUniformMatrix4x3fv;
    /*! glUniformMatrix3x4fv procedure
     */
    PFNGLUNIFORMMATRIX3X4FVPROC m_glUniformMatrix3x4fv;
    /*! glUniformMatrix4x2аv procedure
     */
    PFNGLUNIFORMMATRIX4X2FVPROC m_glUniformMatrix4x2fv;
    /*! glUniformMatrix2x4fv procedure
     */
    PFNGLUNIFORMMATRIX2X4FVPROC m_glUniformMatrix2x4fv;
    /*! glUniformMatrix3x2fv procedure
     */
    PFNGLUNIFORMMATRIX3X2FVPROC m_glUniformMatrix3x2fv;
    /*! glUniformMatrix2x3fv procedure
     */
    PFNGLUNIFORMMATRIX2X3FVPROC m_glUniformMatrix2x3fv;
    /*! glUniformMatrix4fv procedure
     */
    PFNGLUNIFORMMATRIX4FVPROC m_glUniformMatrix4fv;
    /*! glUniformMatrix3fv procedure
     */
    PFNGLUNIFORMMATRIX3FVPROC m_glUniformMatrix3fv;
    /*! glUniformMatrix2fv procedure
     */
    PFNGLUNIFORMMATRIX2FVPROC m_glUniformMatrix2fv;
    /*! glUniform4uiv procedure
     */
    PFNGLUNIFORM4UIVPROC m_glUniform4uiv;
    /*! glUniform3uiv procedure
     */
    PFNGLUNIFORM3UIVPROC m_glUniform3uiv;
    /*! glUniform2uiv procedure
     */
    PFNGLUNIFORM2UIVPROC m_glUniform2uiv;
    /*! glUniform1uiv procedure
     */
    PFNGLUNIFORM1UIVPROC m_glUniform1uiv;
    /*! glUniform4iv procedure
     */
    PFNGLUNIFORM4IVPROC m_glUniform4iv;
    /*! glUniform3iv procedure
     */
    PFNGLUNIFORM3IVPROC m_glUniform3iv;
    /*! glUniform2iv procedure
     */
    PFNGLUNIFORM2IVPROC m_glUniform2iv;
    /*! glUniform1iv procedure
     */
    PFNGLUNIFORM1IVPROC m_glUniform1iv;
    /*! glUniform4fv procedure
     */
    PFNGLUNIFORM4FVPROC m_glUniform4fv;
    /*! glUniform3fv procedure
     */
    PFNGLUNIFORM3FVPROC m_glUniform3fv;
    /*! glUniform2fv procedure
     */
    PFNGLUNIFORM2FVPROC m_glUniform2fv;
    /*! glUniform1fv procedure
     */
    PFNGLUNIFORM1FVPROC m_glUniform1fv;
    /*! glUniform4ui procedure
     */
    PFNGLUNIFORM4UIPROC m_glUniform4ui;
    /*! glUniform3ui procedure
     */
    PFNGLUNIFORM3UIPROC m_glUniform3ui;
    /*! glUniform2ui procedure
     */
    PFNGLUNIFORM2UIPROC m_glUniform2ui;
    /*! glUniform1ui procedure
     */
    PFNGLUNIFORM1UIPROC m_glUniform1ui;
    /*! glUniform4i procedure
     */
    PFNGLUNIFORM4IPROC m_glUniform4i;
    /*! glUniform3i procedure
     */
    PFNGLUNIFORM3IPROC m_glUniform3i;
    /*! glUniform2i procedure
     */
    PFNGLUNIFORM2IPROC m_glUniform2i;
    /*! glUniform1i procedure
     */
    PFNGLUNIFORM1IPROC m_glUniform1i;
    /*! glUniform4f procedure
     */
    PFNGLUNIFORM4FPROC m_glUniform4f;
    /*! glUniform3f procedure
     */
    PFNGLUNIFORM3FPROC m_glUniform3f;
    /*! glUniform2f procedure
     */
    PFNGLUNIFORM2FPROC m_glUniform2f;
    /*! glUniform1f procedure
     */
    PFNGLUNIFORM1FPROC m_glUniform1f;
    /*! glGetUniformLocation procedure;
     */
    PFNGLGETUNIFORMLOCATIONPROC m_glGetUniformLocation;
    /*! glUseProgram procedure;
     */
    PFNGLUSEPROGRAMPROC  m_glUseProgram;
    /*! glGetProgramInfoLog procedure
     */ 
    PFNGLGETPROGRAMINFOLOGPROC m_glGetProgramInfoLog;
    /*! glGetProgramiv procedure
     */
    PFNGLGETPROGRAMIVPROC m_glGetProgramiv;
    /*! glLinkProgram procedure
     */
    PFNGLLINKPROGRAMPROC m_glLinkProgram;
    /*! glAttachShader procedure
     */
    PFNGLATTACHSHADERPROC m_glAttachShader;
    /*! glGetShaderInfoLog procedure
     */
    PFNGLGETSHADERINFOLOGPROC m_glGetShaderInfoLog;
    /*! glGetShaderiv procedure
     */
    PFNGLGETSHADERIVPROC m_glGetShaderiv;
    /*! glCompileShader procedure
     */
    PFNGLCOMPILESHADERPROC m_glCompileShader;
    /*! glShaderSource procedure
     */
    PFNGLSHADERSOURCEPROC m_glShaderSource;
    /*! glCreateShader procedure
     */
    PFNGLCREATESHADERPROC m_glCreateShader;
    /*! glDeleteShader procedure
     */
    PFNGLDELETESHADERPROC m_glDeleteShader;
    /*! glCreateProgram procedure
     */
    PFNGLCREATEPROGRAMPROC m_glCreateProgram;
    /*! glDeleteProgram procedure
     */
    PFNGLDELETEPROGRAMPROC m_glDeleteProgram;
    /*! glActiveTexture procedure
     */
    PFNGLACTIVETEXTUREPROC m_glActiveTexture;
    /*! glGenBuffers procedure
     */
    PFNGLGENBUFFERSPROC m_glGenBuffers;
    /*! glBindBuffer procedure
     */
    PFNGLBINDBUFFERPROC m_glBindBuffer;
    /*! glBufferData procedure
     */
    PFNGLBUFFERDATAPROC m_glBufferData;
    /*! glDeleteBuffers procedure
     */
    PFNGLDELETEBUFFERSPROC m_glDeleteBuffers;
    /*! glEnableVertexAttribArray procedure
     */
    PFNGLENABLEVERTEXATTRIBARRAYPROC m_glEnableVertexAttribArray;
    /*! glEnableVertexAttribPointer procedure
     */
    PFNGLVERTEXATTRIBPOINTERPROC m_glVertexAttribPointer;
    /*! glDisableVertexAttribArray procedure
     */
    PFNGLDISABLEVERTEXATTRIBARRAYPROC m_glDisableVertexAttribArray;
    /*! glMapBuffer procedure
     */
    PFNGLMAPBUFFERPROC m_glMapBuffer;
    /*! glUnmapBuffer procedure
     */
    PFNGLUNMAPBUFFERPROC m_glUnmapBuffer;
    /*! glGenVertexArrays procedure
     */
    PFNGLGENVERTEXARRAYSPROC m_glGenVertexArrays;
    /*! glBindVertexArray procedure
     */
    PFNGLBINDVERTEXARRAYPROC m_glBindVertexArray;
    /*! glDeleteVertexArrays procedure
     */
    PFNGLDELETEVERTEXARRAYSPROC m_glDeleteVertexArrays;

    /*! glGetUniformBlockIndex procedure
     */
    PFNGLGETUNIFORMBLOCKINDEXPROC m_glGetUniformBlockIndex;
    /*! glUniformBlockBinding procedure
     */
    PFNGLUNIFORMBLOCKBINDINGPROC  m_glUniformBlockBinding;
    /*! glBufferSubData procedure
     */
    PFNGLBUFFERSUBDATAPROC m_glBufferSubData;
    /*! glBindBufferBase procedure
     */
    PFNGLBINDBUFFERBASEPROC m_glBindBufferBase;
    /*! glBindBufferRange procedure 
     */
    PFNGLBINDBUFFERRANGEPROC m_glBindBufferRange;
    /*! glBlendEquation procedure
     */
    PFNGLBLENDEQUATIONPROC m_glBlendEquation;
    /*! glBindSampler procedure
     */
    PFNGLBINDSAMPLERPROC  m_glBindSampler;
    /*! glBlendEquationSeparate procedure
     */
    PFNGLBLENDEQUATIONSEPARATEPROC m_glBlendEquationSeparate;
    /*! glBlendFuncSeparate procedure
     */
    PFNGLBLENDFUNCSEPARATEPROC m_glBlendFuncSeparate;
    /*! glGetAttribLocation procedure
     */
    PFNGLGETATTRIBLOCATIONPROC m_glGetAttribLocation;
    /*! glDetachShader procedure
     */
    PFNGLDETACHSHADERPROC m_glDetachShader;
    /*! Whether it was initialized
     */
    bool m_init;
    /*! A parent object for Logging
     */
    sad::OpenGL* m_parent;
    /*! Initialization mutex
     */
    sad::Mutex m_init_mtx;
};

    
}
    
}
