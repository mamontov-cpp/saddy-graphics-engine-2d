#include "os/extensionfunctions.h"

#include "opengl.h"
#include "renderer.h"
#include "log/log.h"

#include <cstdlib>
#include <stdexcept>

// ===================================== PUBLIC METHODS =====================================

sad::os::ExtensionFunctions::ExtensionFunctions() :
m_glUniformMatrix4x3fv(nullptr),
m_glUniformMatrix3x4fv(nullptr),
m_glUniformMatrix4x2fv(nullptr),
m_glUniformMatrix2x4fv(nullptr),
m_glUniformMatrix3x2fv(nullptr),
m_glUniformMatrix2x3fv(nullptr),
m_glUniformMatrix4fv(nullptr),
m_glUniformMatrix3fv(nullptr),
m_glUniformMatrix2fv(nullptr),
m_glUniform4uiv(nullptr),
m_glUniform3uiv(nullptr),
m_glUniform2uiv(nullptr),
m_glUniform1uiv(nullptr),
m_glUniform4iv(nullptr),
m_glUniform3iv(nullptr),
m_glUniform2iv(nullptr),
m_glUniform1iv(nullptr),
m_glUniform4fv(nullptr),
m_glUniform3fv(nullptr),
m_glUniform2fv(nullptr),
m_glUniform1fv(nullptr),
m_glUniform4ui(nullptr),
m_glUniform3ui(nullptr),
m_glUniform2ui(nullptr),
m_glUniform1ui(nullptr),
m_glUniform4i(nullptr),
m_glUniform3i(nullptr),
m_glUniform2i(nullptr),
m_glUniform1i(nullptr),
m_glUniform4f(nullptr),
m_glUniform3f(nullptr),
m_glUniform2f(nullptr),
m_glUniform1f(nullptr),
m_glGetUniformLocation(nullptr),
m_glUseProgram(nullptr),
m_glGetProgramInfoLog(nullptr),
m_glGetProgramiv(nullptr),
m_glLinkProgram(nullptr),
m_glAttachShader(nullptr),
m_glGetShaderInfoLog(nullptr),
m_glGetShaderiv(nullptr),
m_glCompileShader(nullptr),
m_glShaderSource(nullptr),
m_glCreateShader(nullptr),
m_glDeleteShader(nullptr),
m_glCreateProgram(nullptr),
m_glDeleteProgram(nullptr),
m_glActiveTexture(nullptr),
m_glGenBuffers(nullptr),
m_glBindBuffer(nullptr),
m_glBufferData(nullptr),
m_glDeleteBuffers(nullptr),
m_glEnableVertexAttribArray(nullptr),
m_glVertexAttribPointer(nullptr),
m_glDisableVertexAttribArray(nullptr),
m_glMapBuffer(nullptr),
m_glUnmapBuffer(nullptr),
m_glGenVertexArrays(nullptr),
m_glBindVertexArray(nullptr),
m_glDeleteVertexArrays(nullptr),
m_glGetUniformBlockIndex(nullptr),
m_glUniformBlockBinding(nullptr),
m_glBufferSubData(nullptr),
m_glBindBufferBase(nullptr),
m_glBindBufferRange(nullptr),
m_glBlendEquation(nullptr),
m_glBindSampler(nullptr),
m_glBlendEquationSeparate(nullptr),
m_glBlendFuncSeparate(nullptr),
m_glGetAttribLocation(nullptr),
m_glDetachShader(nullptr),
m_init(false),
m_parent(nullptr)
{

}

void sad::os::ExtensionFunctions::setParent(sad::OpenGL* ogl)
{
    m_parent = ogl;
}

#define TRY_GET_PROC_ADDRESS(TYPE, VARIABLE)  m_##VARIABLE = reinterpret_cast< TYPE >(getProcAdress(#VARIABLE)); m_init = m_init && (m_##VARIABLE != nullptr); if (m_##VARIABLE == nullptr) this->showGetProcAddressFailedError(#VARIABLE);

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

            TRY_GET_PROC_ADDRESS(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex);
            TRY_GET_PROC_ADDRESS(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding);
            TRY_GET_PROC_ADDRESS(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
            TRY_GET_PROC_ADDRESS(PFNGLBINDBUFFERBASEPROC, glBindBufferBase);
            TRY_GET_PROC_ADDRESS(PFNGLBINDBUFFERRANGEPROC, glBindBufferRange);

            TRY_GET_PROC_ADDRESS(PFNGLBLENDEQUATIONPROC, glBlendEquation);
            TRY_GET_PROC_ADDRESS(PFNGLBINDSAMPLERPROC, glBindSampler);
            TRY_GET_PROC_ADDRESS(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate);
            TRY_GET_PROC_ADDRESS(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate);
            TRY_GET_PROC_ADDRESS(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
            TRY_GET_PROC_ADDRESS(PFNGLDETACHSHADERPROC, glDetachShader);
        }
        m_init_mtx.unlock();
    }
}

void sad::os::ExtensionFunctions::glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose,const GLfloat* value)
{
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
    
    if (this->m_glMapBuffer)
    {
        return (this->m_glMapBuffer)(target, access);
    }
    else
    {
        throw std::logic_error("glMapBuffer() is unavailable on this platform");
    }
    return nullptr;
}

void sad::os::ExtensionFunctions::glUnmapBuffer(GLenum target)
{
    
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
    
    if (this->m_glDeleteVertexArrays)
    {
        (this->m_glDeleteVertexArrays)(n, arrays);
    }
    else
    {
        throw std::logic_error("glDeleteVertexArrays() is unavailable on this platform");
    }
}

GLuint sad::os::ExtensionFunctions::glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName)
{
    if (this->m_glGetUniformBlockIndex)
    {
        return (this->m_glGetUniformBlockIndex)(program, uniformBlockName);
    }
    else
    {
        throw std::logic_error("glGetUniformBlockIndex() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
{
    if (this->m_glUniformBlockBinding)
    {
        (this->m_glUniformBlockBinding)(program, uniformBlockIndex, uniformBlockBinding);
    }
    else
    {
        throw std::logic_error("glUniformBlockBinding() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    if (this->m_glBufferSubData)
    {
        (this->m_glBufferSubData)(target, offset, size, data);
    }
    else
    {
        throw std::logic_error("glBufferSubData() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
    if (this->m_glBindBufferBase)
    {
        (this->m_glBindBufferBase)(target, index, buffer);
    }
    else
    {
        throw std::logic_error("glBindBufferBase() is unavailable on this platform");
    }
}


void sad::os::ExtensionFunctions::glBindBufferRange(
    GLenum target,
    GLuint index,
    GLuint buffer,
    GLintptr offset,
    GLsizeiptr size
)
{
    if (this->m_glBindBufferRange)
    {
        (this->m_glBindBufferRange)(target, index, buffer, offset, size);
    }
    else
    {
        throw std::logic_error("glBindBufferRange() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBlendEquation(GLenum mode)
{
    if (this->m_glBlendEquation)
    {
        (this->m_glBlendEquation)(mode);
    }
    else
    {
        throw std::logic_error("glBlendEquation() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBindSampler(GLuint unit, GLuint sampler)
{
    if (this->m_glBindSampler)
    {
        (this->m_glBindSampler)(unit, sampler);
    }
    else
    {
        throw std::logic_error("glBindSampler() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    if (this->m_glBlendEquationSeparate)
    {
        (this->m_glBlendEquationSeparate)(modeRGB, modeAlpha);
    }
    else
    {
        throw std::logic_error("glBlendEquationSeparate() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    if (this->m_glBlendFuncSeparate)
    {
        (this->m_glBlendFuncSeparate)(srcRGB, dstRGB, srcAlpha, dstAlpha);
    }
    else
    {
        throw std::logic_error("glBlendFuncSeparate() is unavailable on this platform");
    }
}

GLint sad::os::ExtensionFunctions::glGetAttribLocation(GLuint program, const GLchar* name)
{
    if (this->m_glGetAttribLocation)
    {
        return (this->m_glGetAttribLocation)(program, name);
    }
    else
    {
        throw std::logic_error("glGetAttribLocation() is unavailable on this platform");
    }
}

void sad::os::ExtensionFunctions::glDetachShader(GLuint program, GLuint shader)
{
    if (this->m_glDetachShader)
    {
        (this->m_glDetachShader)(program, shader);
    }
    else
    {
        throw std::logic_error("glDetachShader() is unavailable on this platform");
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

