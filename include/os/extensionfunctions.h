/*! \file extensionfunctions.h
    
    An extensions functions, dynamically loaded from libraries
 */
#pragma once
#include "glheaders.h"
#include "../sadstring.h"

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
    
    void tryInit();
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
    /*! glUserProgram prcedure;
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
};

    
}
    
}