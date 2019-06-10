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
m_init(false),
m_parent(NULL)
{

}

void sad::os::ExtensionFunctions::setParent(sad::OpenGL* ogl)
{
    m_parent = ogl;
}

#define TRY_GET_PROC_ADDRESS(TYPE, VARIABLE)  m_##VARIABLE = ( TYPE )getProcAdress(#VARIABLE); m_init = m_init && (m_##VARIABLE != NULL); if (m_##VARIABLE == NULL) this->showGetProcAddressFailedError(#VARIABLE);

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
    if (this->m_glUniform2iv)
    {
        (this->m_glUniform2iv)(location, count, value);
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
        (this->glUniform4ui)(location, v0, v1, v2, v3);
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
        (this->glUniform3ui)(location, v0, v1, v2);
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
        (this->glUniform2ui)(location, v0, v1);
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
        (this->glUniform1ui)(location, v0);
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
        (this->glUniform4i)(location, v0, v1, v2, v3);
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
        (this->glUniform3i)(location, v0, v1, v2);
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
        (this->glUniform2i)(location, v0, v1);
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
        (this->glUniform1i)(location, v0);
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
        (this->glUniform4f)(location, v0, v1, v2, v3);
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
        (this->glUniform3f)(location, v0, v1, v2);
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
        (this->glUniform2f)(location, v0, v1);
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
        (this->glUniform1f)(location, v0);
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

// ===================================== PRIVATE METHODS =====================================

void sad::os::ExtensionFunctions::showGetProcAddressFailedError(const sad::String& name)
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

