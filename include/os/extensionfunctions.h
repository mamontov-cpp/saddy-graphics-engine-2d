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
private:
    /*! Appends info that get proc address if failed to renderer (main if OpenGL) is not set
     */
    void showGetProcAddressFailedError(const sad::String& name);
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
    /*! glDeleteShader prdocedure
     */
    PFNGLDELETESHADERPROC m_glDeleteShader;
    /*! glCreateProgram procedure
     */
    PFNGLCREATEPROGRAMPROC m_glCreateProgram;
    /*! glDeleteProgram procedure
     */
    PFNGLDELETEPROGRAMPROC m_glDeleteProgram;
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