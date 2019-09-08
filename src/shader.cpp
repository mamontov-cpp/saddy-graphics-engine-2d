#include "shader.h"

#include "os/shaderimpl.h"

DECLARE_SOBJ(sad::Shader)



sad::Shader::Shader() : m_impl(new sad::os::ShaderImpl())
{

}

sad::Shader::Shader(const sad::Shader& o) : m_impl(new sad::os::ShaderImpl(*(o.m_impl)))  // NOLINT(bugprone-copy-constructor-init)
{
}

sad::Shader& sad::Shader::operator=(const sad::Shader& o)
{
    delete m_impl;
    m_impl = new sad::os::ShaderImpl(*(o.m_impl));
    return *this;
}

sad::Shader::~Shader()
{
    delete m_impl;
}

void sad::Shader::setRenderer(sad::Renderer* r) const
{
    m_impl->setRenderer(r);
}

sad::Renderer* sad::Shader::renderer() const
{
    return m_impl->renderer();
}

void sad::Shader::setVertexProgram(const sad::String& vertexProgram) const
{
    m_impl->setVertexProgram(vertexProgram);
}

void sad::Shader::clearVertexProgram() const
{
    m_impl->clearVertexProgram();
}

void sad::Shader::setFragmentProgram(const sad::String& fragmentProgram) const
{
    m_impl->setFragmentProgram(fragmentProgram);
}

void sad::Shader::clearFragmentProgram() const
{
    m_impl->clearFragmentProgram();
}

bool sad::Shader::loadVertexProgramFromFile(const sad::String& fileName) const
{
    return m_impl->loadVertexProgramFromFile(fileName);
}

bool sad::Shader::loadFragmentProgramFromFile(const sad::String& fileName) const
{
    return m_impl->loadFragmentProgramFromFile(fileName);
}

void sad::Shader::tryUpload() const
{
    m_impl->tryUpload();
}

void sad::Shader::tryDestroy()
{
    m_impl->tryDestroy();
}

void sad::Shader::use() const
{
    m_impl->use();
}

void sad::Shader::disable() const
{
    m_impl->disable();
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, int v0) const
{
    m_impl->setUniformVariable(loc_name, v0);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, unsigned int v0) const
{
    m_impl->setUniformVariable(loc_name, v0);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, float v0) const
{
    m_impl->setUniformVariable(loc_name, v0);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, int v0, int v1) const
{
    m_impl->setUniformVariable(loc_name, v0, v1);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1) const
{
    m_impl->setUniformVariable(loc_name, v0, v1);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, float v0, float v1) const
{
    m_impl->setUniformVariable(loc_name, v0, v1);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2, int v3) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2, v3);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2, v3);
}

void sad::Shader::setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2, float v3) const
{
    m_impl->setUniformVariable(loc_name, v0, v1, v2, v3);
}

void sad::Shader::tryDestroy() const
{
    m_impl->tryDestroy();
}

int sad::Shader::getUniformLocation(const sad::String& name)
{
    return m_impl->getUniformLocation(name);
}

void sad::Shader::setUniformMatrix4x3(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix4x3(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix3x4(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix3x4(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix4x2(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix4x2(location, count, transpose, value);
}

void  sad::Shader::setUniformMatrix2x4(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix2x4(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix3x2(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix3x2(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix2x3(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix2x3(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix4(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix4(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix3(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix3(location, count, transpose, value);
}

void sad::Shader::setUniformMatrix2(int location, int count, bool transpose,const float* value)
{
    m_impl->setUniformMatrix2(location, count, transpose, value);
}

void sad::Shader::setUniform4(int location, int count, const unsigned int* value)
{
    m_impl->setUniform4(location, count, value);
}

void sad::Shader::setUniform3(int location, int count, const unsigned int* value)
{
    m_impl->setUniform3(location, count, value);
}

void sad::Shader::setUniform2(int location, int count, const unsigned int* value)
{
    m_impl->setUniform2(location, count, value);
}

void sad::Shader::setUniform1(int location, int count, const unsigned int* value)
{
    m_impl->setUniform1(location, count, value);
}

void sad::Shader::setUniform4(int location, int count, const int* value)
{
    m_impl->setUniform4(location, count, value);
}

void sad::Shader::setUniform3(int location, int count, const int* value)
{
    m_impl->setUniform3(location, count, value);
}

void sad::Shader::setUniform2(int location, int count, const int* value)
{
    m_impl->setUniform2(location, count, value);
}

void sad::Shader::setUniform1(int location, int count, const int* value)
{
    m_impl->setUniform1(location, count, value);
}


void sad::Shader::setUniform4(int location, int count, const float* value)
{
    m_impl->setUniform4(location, count, value);
}

void sad::Shader::setUniform3(int location, int count, const float* value)
{
    m_impl->setUniform3(location, count, value);
}

void sad::Shader::setUniform2(int location, int count, const float* value)
{
    m_impl->setUniform2(location, count, value);
}

void sad::Shader::setUniform1(int location, int count, const float* value)
{
    m_impl->setUniform1(location, count, value);
}

void sad::Shader::setUniform(int location, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3)
{
    m_impl->setUniform(location, v0, v1, v2, v3);
}

void sad::Shader::setUniform(int location, unsigned int v0, unsigned int v1, unsigned int v2)
{
    m_impl->setUniform(location, v0, v1, v2);
}

void sad::Shader::setUniform(int location, unsigned int v0, unsigned int v1)
{
    m_impl->setUniform(location, v0, v1);
}

void sad::Shader::setUniform(int location, unsigned int v0)
{
    m_impl->setUniform(location, v0);
}

void sad::Shader::setUniform(int location, int v0, int v1, int v2, int v3)
{
    m_impl->setUniform(location, v0, v1, v2, v3);
}

void sad::Shader::setUniform(int location, int v0, int v1, int v2)
{
    m_impl->setUniform(location, v0, v1, v2);
}

void sad::Shader::setUniform(int location, int v0, int v1)
{
    m_impl->setUniform(location, v0, v1);
}

void sad::Shader::setUniform(int location, int v0)
{
    m_impl->setUniform(location, v0);
}

void sad::Shader::setUniform(int location, float v0, float v1, float v2, float v3)
{
    m_impl->setUniform(location, v0, v1, v2, v3);
}

void sad::Shader::setUniform(int location, float v0, float v1, float v2)
{
    m_impl->setUniform(location, v0, v1, v2);
}

void sad::Shader::setUniform(int location, float v0, float v1)
{
    m_impl->setUniform(location, v0, v1);
}

void sad::Shader::setUniform(int location, float v0)
{
    m_impl->setUniform(location, v0);
}

void sad::Shader::tryLogGlError(const char* op)
{
    m_impl->tryLogGlError(op);
}
