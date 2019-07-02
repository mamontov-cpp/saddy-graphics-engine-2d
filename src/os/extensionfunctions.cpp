#include "os/extensionfunctions.h"

#include "opengl.h"
#include "renderer.h"
#include "log/log.h"

#include <cstdlib>
#include <stdexcept>

// ===================================== PUBLIC METHODS =====================================

sad::os::ExtensionFunctions::ExtensionFunctions() :
m_glUniformMatrix4x3fv(NULL),
m_glUniformMatrix3x4fv(NULL),
m_glUniformMatrix4x2fv(NULL),
m_glUniformMatrix2x4fv(NULL),
m_glUniformMatrix3x2fv(NULL),
m_glUniformMatrix2x3fv(NULL),
m_glUniformMatrix4fv(NULL),
m_glUniformMatrix3fv(NULL),
m_glUniformMatrix2fv(NULL),
m_glUniform4uiv(NULL),
m_glUniform3uiv(NULL),
m_glUniform2uiv(NULL),
m_glUniform1uiv(NULL),
m_glUniform4iv(NULL),
m_glUniform3iv(NULL),
m_glUniform2iv(NULL),
m_glUniform1iv(NULL),
m_glUniform4fv(NULL),
m_glUniform3fv(NULL),
m_glUniform2fv(NULL),
m_glUniform1fv(NULL),
m_glUniform4ui(NULL),
m_glUniform3ui(NULL),
m_glUniform2ui(NULL),
m_glUniform1ui(NULL),
m_glUniform4i(NULL),
m_glUniform3i(NULL),
m_glUniform2i(NULL),
m_glUniform1i(NULL),
m_glUniform4f(NULL),
m_glUniform3f(NULL),
m_glUniform2f(NULL),
m_glUniform1f(NULL),
m_glGetUniformLocation(NULL),
m_glUseProgram(NULL),
m_glGetProgramInfoLog(NULL),
m_glGetProgramiv(NULL),
m_glLinkProgram(NULL),
m_glAttachShader(NULL),
m_glGetShaderInfoLog(NULL),
m_glGetShaderiv(NULL),
m_glCompileShader(NULL),
m_glShaderSource(NULL),
m_glCreateShader(NULL),
m_glDeleteShader(NULL),
m_glCreateProgram(NULL),
m_glDeleteProgram(NULL),
m_glActiveTexture(NULL),
m_glGenBuffers(NULL),
m_glBindBuffer(NULL),
m_glBufferData(NULL),
m_glDeleteBuffers(NULL),
m_glEnableVertexAttribArray(NULL),
m_glVertexAttribPointer(NULL),
m_glDisableVertexAttribArray(NULL),
m_glMapBuffer(NULL),
m_glUnmapBuffer(NULL),
m_glGenVertexArrays(NULL),
m_glBindVertexArray(NULL),
m_glDeleteVertexArrays(NULL),
m_init(false),
m_parent(NULL)
{

}

void sad::os::ExtensionFunctions::setParent(sad::OpenGL* ogl)
{
    m_parent = ogl;
}

#define TRY_GET_PROC_ADDRESS(TYPE, VARIABLE)  m_##VARIABLE = reinterpret_cast< TYPE >(getProcAdress(#VARIABLE)); m_init = m_init && (m_##VARIABLE != NULL); if (m_##VARIABLE == NULL) this->showGetProcAddressFailedError(#VARIABLE);

void sad::os::ExtensionFunctions::tryInit()
{
    if (!m_init) 
    {
        m_init_mtx.lock();
        if (!m_init)
        {
            m_init = true;
            
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX4X3FVPROC, glUniformMatrix4x3fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX3X4FVPROC, glUniformMatrix3x4fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX4X2FVPROC, glUniformMatrix4x2fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX2X4FVPROC, glUniformMatrix2x4fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX3X2FVPROC, glUniformMatrix3x2fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX2X3FVPROC, glUniformMatrix2x3fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMMATRIX2FVPROC, glUniformMatrix2fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4UIVPROC, glUniform4uiv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3UIVPROC, glUniform3uiv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2UIVPROC, glUniform2uiv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1UIVPROC, glUniform1uiv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4IVPROC, glUniform4iv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3IVPROC, glUniform3iv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2IVPROC, glUniform2iv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1IVPROC, glUniform1iv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4FVPROC, glUniform4fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3FVPROC, glUniform3fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2FVPROC, glUniform2fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1FVPROC, glUniform1fv);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4UIPROC, glUniform4ui);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3UIPROC, glUniform3ui);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2UIPROC, glUniform2ui);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1UIPROC, glUniform1ui);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4IPROC, glUniform4i);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3IPROC, glUniform3i);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2IPROC, glUniform2i);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1IPROC, glUniform1i);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM4FPROC, glUniform4f);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM3FPROC, glUniform3f);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM2FPROC, glUniform2f);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORM1FPROC, glUniform1f);
            TRY_GET_PROC_ADDRESS(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
            TRY_GET_PROC_ADDRESS(PFNGLUSEPROGRAMPROC, glUseProgram);
            TRY_GET_PROC_ADDRESS(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
            TRY_GET_PROC_ADDRESS(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
            TRY_GET_PROC_ADDRESS(PFNGLLINKPROGRAMPROC, glLinkProgram);
            TRY_GET_PROC_ADDRESS(PFNGLATTACHSHADERPROC, glAttachShader);
            TRY_GET_PROC_ADDRESS(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
            TRY_GET_PROC_ADDRESS(PFNGLGETSHADERIVPROC, glGetShaderiv);
            TRY_GET_PROC_ADDRESS(PFNGLCOMPILESHADERPROC, glCompileShader);
            TRY_GET_PROC_ADDRESS(PFNGLSHADERSOURCEPROC, glShaderSource);
            TRY_GET_PROC_ADDRESS(PFNGLCREATESHADERPROC, glCreateShader);
            TRY_GET_PROC_ADDRESS(PFNGLDELETESHADERPROC, glDeleteShader);
            TRY_GET_PROC_ADDRESS(PFNGLCREATEPROGRAMPROC, glCreateProgram);
            TRY_GET_PROC_ADDRESS(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
            TRY_GET_PROC_ADDRESS(PFNGLACTIVETEXTUREPROC, glActiveTexture);

            TRY_GET_PROC_ADDRESS(PFNGLGENBUFFERSPROC, glGenBuffers);
            TRY_GET_PROC_ADDRESS(PFNGLBINDBUFFERPROC, glBindBuffer);
            TRY_GET_PROC_ADDRESS(PFNGLBUFFERDATAPROC, glBufferData);
            TRY_GET_PROC_ADDRESS(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
            TRY_GET_PROC_ADDRESS(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
            TRY_GET_PROC_ADDRESS(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
            TRY_GET_PROC_ADDRESS(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
            TRY_GET_PROC_ADDRESS(PFNGLMAPBUFFERPROC, glMapBuffer);
            TRY_GET_PROC_ADDRESS(PFNGLUNMAPBUFFERPROC, glUnmapBuffer);
            TRY_GET_PROC_ADDRESS(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
            TRY_GET_PROC_ADDRESS(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
            TRY_GET_PROC_ADDRESS(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
        }
        m_init_mtx.unlock();
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix4x3fv)
    {
        (this->m_glUniformMatrix4x3fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix4x3fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix3x4fv)
    {
        (this->m_glUniformMatrix3x4fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix3x4fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix4x2fv)
    {
        (this->m_glUniformMatrix4x2fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix4x2fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix2x4fv)
    {
        (this->m_glUniformMatrix2x4fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix2x4fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix3x2fv)
    {
        (this->m_glUniformMatrix3x2fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix3x2fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix2x3fv)
    {
        (this->m_glUniformMatrix2x3fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix2x3fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix4fv)
    {
        (this->m_glUniformMatrix4fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix4fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix3fv)
    {
        (this->m_glUniformMatrix3fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix3fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniformMatrix2fv)
    {
        (this->m_glUniformMatrix2fv)(location, count, transpose, value);
    }
    else
    {
        throw std::logic_error("glUniformMatrix2fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4uiv(GLint location, GLsizei count, const GLuint* value)
{
    this->tryInit();
    if (this->m_glUniform4uiv)
    {
        (this->m_glUniform4uiv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform4uiv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3uiv(GLint location, GLsizei count, const GLuint* value)
{
    this->tryInit();
    if (this->m_glUniform3uiv)
    {
        (this->m_glUniform3uiv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform3uiv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2uiv(GLint location, GLsizei count, const GLuint* value)
{
    this->tryInit();
    if (this->m_glUniform2uiv)
    {
        (this->m_glUniform2uiv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform2uiv() is unavailable on this platform");
    }
}


void sad::os::ExtensionFunctions::glUniform1uiv(GLint location, GLsizei count, const GLuint* value)
{
    this->tryInit();
    if (this->m_glUniform1uiv)
    {
        (this->m_glUniform1uiv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform1uiv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4iv(GLint location, GLsizei count, const GLint* value)
{
    this->tryInit();
    if (this->m_glUniform4iv)
    {
        (this->m_glUniform4iv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform4iv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3iv(GLint location, GLsizei count, const GLint* value)
{
    this->tryInit();
    if (this->m_glUniform3iv)
    {
        (this->m_glUniform3iv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform3iv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2iv(GLint location, GLsizei count, const GLint* value)
{
    this->tryInit();
    if (this->m_glUniform2iv)
    {
        (this->m_glUniform2iv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform2iv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform1iv(GLint location, GLsizei count, const GLint* value)
{
    this->tryInit();
    if (this->m_glUniform1iv)
    {
        (this->m_glUniform1iv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform1iv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniform4fv)
    {
        (this->m_glUniform4fv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform4fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniform3fv)
    {
        (this->m_glUniform3fv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform3fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniform2fv)
    {
        (this->m_glUniform2fv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform2fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform1fv(GLint location, GLsizei count, const GLfloat* value)
{
    this->tryInit();
    if (this->m_glUniform1fv)
    {
        (this->m_glUniform1fv)(location, count, value);
    }
    else
    {
        throw std::logic_error("glUniform2fv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    this->tryInit();
    if (this->m_glUniform4ui)
    {
        (this->m_glUniform4ui)(location, v0, v1, v2, v3);
    }
    else
    {
        throw std::logic_error("glUniform4ui() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
    this->tryInit();
    if (this->m_glUniform3ui)
    {
        (this->m_glUniform3ui)(location, v0, v1, v2);
    }
    else
    {
        throw std::logic_error("glUniform3ui() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2ui(GLint location, GLuint v0, GLuint v1)
{
    this->tryInit();
    if (this->m_glUniform2ui)
    {
        (this->m_glUniform2ui)(location, v0, v1);
    }
    else
    {
        throw std::logic_error("glUniform2ui() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform1ui(GLint location, GLuint v0)
{
    this->tryInit();
    if (this->m_glUniform1ui)
    {
        (this->m_glUniform1ui)(location, v0);
    }
    else
    {
        throw std::logic_error("glUniform1ui() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    this->tryInit();
    if (this->m_glUniform4i)
    {
        (this->m_glUniform4i)(location, v0, v1, v2, v3);
    }
    else
    {
        throw std::logic_error("glUniform4i() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
    this->tryInit();
    if (this->m_glUniform3i)
    {
        (this->m_glUniform3i)(location, v0, v1, v2);
    }
    else
    {
        throw std::logic_error("glUniform3i() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2i(GLint location, GLint v0, GLint v1)
{
    this->tryInit();
    if (this->m_glUniform2i)
    {
        (this->m_glUniform2i)(location, v0, v1);
    }
    else
    {
        throw std::logic_error("glUniform2i() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform1i(GLint location, GLint v0)
{
    this->tryInit();
    if (this->m_glUniform1i)
    {
        (this->m_glUniform1i)(location, v0);
    }
    else
    {
        throw std::logic_error("glUniform1i() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    this->tryInit();
    if (this->m_glUniform4f)
    {
        (this->m_glUniform4f)(location, v0, v1, v2, v3);
    }
    else
    {
        throw std::logic_error("glUniform4f() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    this->tryInit();
    if (this->m_glUniform3f)
    {
        (this->m_glUniform3f)(location, v0, v1, v2);
    }
    else
    {
        throw std::logic_error("glUniform3f() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    this->tryInit();
    if (this->m_glUniform2f)
    {
        (this->m_glUniform2f)(location, v0, v1);
    }
    else
    {
        throw std::logic_error("glUniform2f() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniform1f(GLint location, GLfloat v0)
{
    this->tryInit();
    if (this->m_glUniform1f)
    {
        (this->m_glUniform1f)(location, v0);
    }
    else
    {
        throw std::logic_error("glUniform1f() is unavailable on this platform");
    }
}

GLint sad::os::ExtensionFunctions::glGetUniformLocation(GLuint program, const GLchar* name)
{
    this->tryInit();
    if (this->m_glGetUniformLocation)
    {
        return (this->m_glGetUniformLocation)(program, name);
    }
    else
    {
        throw std::logic_error("glGetUniformLocation() is unavailable on this platform");
    }
    // ReSharper disable once CppUnreachableCode
    return 0;
}

void sad::os::ExtensionFunctions::glUseProgram(GLuint program)
{
    this->tryInit();
    if (this->m_glUseProgram)
    {
        (this->m_glUseProgram)(program);
    }
    else
    {
        throw std::logic_error("glUseProgram() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
{
    this->tryInit();
    if (this->m_glGetProgramInfoLog)
    {
        (this->m_glGetProgramInfoLog)(program, maxLength, length, infoLog);
    }
    else
    {
        throw std::logic_error("glGetProgramInfoLog() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGetProgramiv(GLuint program, GLenum pname, GLint* params)
{
    this->tryInit();
    if (this->m_glGetProgramiv)
    {
        (this->m_glGetProgramiv)(program, pname, params);
    }
    else
    {
        throw std::logic_error("glGetProgramiv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glLinkProgram(GLuint program)
{
    this->tryInit();
    if (this->m_glLinkProgram)
    {
        (this->m_glLinkProgram)(program);
    }
    else
    {
        throw std::logic_error("glLinkProgram() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glAttachShader(GLuint program, GLuint shader)
{
    this->tryInit();
    if (this->m_glAttachShader)
    {
        (this->m_glAttachShader)(program, shader);
    }
    else
    {
        throw std::logic_error("glAttachShader() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog)
{
    this->tryInit();
    if (this->m_glGetShaderInfoLog)
    {
        (this->m_glGetShaderInfoLog)(shader, maxLength, length, infoLog);
    }
    else
    {
        throw std::logic_error("glGetShaderInfoLog() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
    this->tryInit();
    if (this->m_glGetShaderiv)
    {
        (this->m_glGetShaderiv)(shader, pname, params);
    }
    else
    {
        throw std::logic_error("glGetShaderiv() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glCompileShader(GLuint shader)
{
    this->tryInit();
    if (this->m_glCompileShader)
    {
        (this->m_glCompileShader)(shader);
    }
    else
    {
        throw std::logic_error("glCompileShader() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length)
{
    this->tryInit();
    if (this->m_glShaderSource)
    {
        (this->m_glShaderSource)(shader, count, string, length);
    }
    else
    {
        throw std::logic_error("glShaderSource() is unavailable on this platform");
    }
}

GLuint sad::os::ExtensionFunctions::glCreateShader(GLenum shaderType)
{
    this->tryInit();
    if (this->m_glCreateShader)
    {
        return (this->m_glCreateShader)(shaderType);
    }
    else
    {
        throw std::logic_error("glShaderSource() is unavailable on this platform");
    }
    // ReSharper disable once CppUnreachableCode
    return 0;
}

void sad::os::ExtensionFunctions::glDeleteShader(GLuint shader)
{
    this->tryInit();
    if (this->m_glDeleteShader)
    {
        (this->m_glDeleteShader)(shader);
    }
    else
    {
        throw std::logic_error("glDeleteShader() is unavailable on this platform");
    }
}

GLuint sad::os::ExtensionFunctions::glCreateProgram()
{
    this->tryInit();
    if (this->m_glCreateProgram)
    {
        return (this->m_glCreateProgram)();
    }
    else
    {
        throw std::logic_error("glShaderSource() is unavailable on this platform");
    }
    // ReSharper disable once CppUnreachableCode
    return 0;
}

void sad::os::ExtensionFunctions::glDeleteProgram(GLuint program)
{
    this->tryInit();
    if (this->m_glDeleteProgram)
    {
        (this->m_glDeleteProgram)(program);
    }
    else
    {
        throw std::logic_error("glDeleteProgram() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glActiveTexture(GLenum tex)
{
    this->tryInit();
    if (this->m_glActiveTexture)
    {
        (this->m_glActiveTexture)(tex);
    }
    else
    {
        throw std::logic_error("glActiveTexture() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGenBuffers(GLsizei n, GLuint* buffers)
{
    this->tryInit();
    if (this->m_glGenBuffers)
    {
        (this->m_glGenBuffers)(n, buffers);
    }
    else
    {
        throw std::logic_error("glGenBuffers() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBindBuffer(GLenum target, GLuint buffer)
{
    this->tryInit();
    if (this->m_glBindBuffer)
    {
        (this->m_glBindBuffer)(target, buffer);
    }
    else
    {
        throw std::logic_error("glBindBuffer() is unavailable on this platform");
    }
}


void sad::os::ExtensionFunctions::glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
    this->tryInit();
    if (this->m_glBufferData)
    {
        (this->m_glBufferData)(target, size, data, usage);
    }
    else
    {
        throw std::logic_error("glBufferData() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
    this->tryInit();
    if (this->m_glDeleteBuffers)
    {
        (this->m_glDeleteBuffers)(n, buffers);
    }
    else
    {
        throw std::logic_error("glDeleteBuffers() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glEnableVertexAttribArray(GLuint index)
{
    this->tryInit();
    if (this->m_glEnableVertexAttribArray)
    {
        (this->m_glEnableVertexAttribArray)(index);
    }
    else
    {
        throw std::logic_error("glEnableVertexAttribArray() is unavailable on this platform");
    }
}


void sad::os::ExtensionFunctions::glDisableVertexAttribArray(GLuint index)
{
    this->tryInit();
    if (this->m_glDisableVertexAttribArray)
    {
        (this->m_glDisableVertexAttribArray)(index);
    }
    else
    {
        throw std::logic_error("glDisableVertexAttribArray() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glVertexAttribPointer(GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid* pointer
)
{
    this->tryInit();
    if (this->m_glVertexAttribPointer)
    {
        (this->m_glVertexAttribPointer)(index, size, type, normalized, stride, pointer);
    }
    else
    {
        throw std::logic_error("glVertexAttribPointer() is unavailable on this platform");
    }
}

void* sad::os::ExtensionFunctions::glMapBuffer(GLenum target, GLenum access)
{
    this->tryInit();
    if (this->m_glMapBuffer)
    {
        return (this->m_glMapBuffer)(target, access);
    }
    else
    {
        throw std::logic_error("glMapBuffer() is unavailable on this platform");
    }
    return NULL;
}

void sad::os::ExtensionFunctions::glUnmapBuffer(GLenum target)
{
    this->tryInit();
    if (this->m_glUnmapBuffer)
    {
        (this->m_glUnmapBuffer)(target);
    }
    else
    {
        throw std::logic_error("glUnmapBuffer() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glGenVertexArrays(GLsizei n, GLuint* arrays)
{
    this->tryInit();
    if (this->m_glGenVertexArrays)
    {
        (this->m_glGenVertexArrays)(n, arrays);
    }
    else
    {
        throw std::logic_error("glGenVertexArrays() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBindVertexArray(GLuint array)
{
    this->tryInit();
    if (this->m_glBindVertexArray)
    {
        (this->m_glBindVertexArray)(array);
    }
    else
    {
        throw std::logic_error("glBindVertexArray() is unavailable on this platform");
    }
}


void sad::os::ExtensionFunctions::glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
    this->tryInit();
    if (this->m_glDeleteVertexArrays)
    {
        (this->m_glDeleteVertexArrays)(n, arrays);
    }
    else
    {
        throw std::logic_error("glDeleteVertexArrays() is unavailable on this platform");
    }
}
// ===================================== PRIVATE METHODS =====================================

void sad::os::ExtensionFunctions::showGetProcAddressFailedError(const sad::String& name) const
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_parent)
    {
        if (m_parent->renderer())
        {
            r = m_parent->renderer();
        }
    }
    sad::String message = "glGetProcAddress: function";
    message += "\"" + name +  "\"";
    message += " not found";
    SL_LOCAL_FATAL(message, *r);
}

