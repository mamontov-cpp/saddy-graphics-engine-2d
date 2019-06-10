#include "os/extensionfunctions.h"

#include "opengl.h"
#include "renderer.h"
#include "log/log.h"

#include <cstdlib>

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

