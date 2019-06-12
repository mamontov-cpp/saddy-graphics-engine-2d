#include "shader.h"

#include "os/shaderimpl.h"

DECLARE_SOBJ(sad::Shader)

sad::Shader::Shader() : m_impl(new sad::os::ShaderImpl())
{
    m_on_use = []() {};
    m_on_destroy = []() {};
}

sad::Shader::Shader(const sad::Shader& o) : m_impl(new sad::os::ShaderImpl(*(o.m_impl))), m_on_use(o.m_on_use), m_on_destroy(o.m_on_destroy)
{
}

sad::Shader& sad::Shader::operator=(const sad::Shader& o)
{
    delete m_impl;
    m_impl = new sad::os::ShaderImpl(*(o.m_impl));
    m_on_use = o.m_on_use;
    m_on_destroy = o.m_on_destroy;
    return *this;
}

sad::Shader::~Shader()
{
    m_on_destroy();
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

void sad::Shader::setFragmentProgram(const sad::String& fragmentProgram) const
{
    m_impl->setFragmentProgram(fragmentProgram);
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

void sad::Shader::use() const
{
    m_impl->use();
    m_on_use();
}

void sad::Shader::disable() const
{
    m_impl->disable();
}

void sad::Shader::setOnUseCallback(const std::function<void()>& cb)
{
    m_on_use = cb;
}

void sad::Shader::setOnDestroyCallback(const std::function<void()>& cb)
{
    m_on_destroy = cb;
}

const std::function<void()>& sad::Shader::onUseCallback() const
{
    return m_on_use;
}

const std::function<void()>& sad::Shader::onDestroyCallback() const
{
    return m_on_destroy;
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
