#include "shaderfunction.h"
#include "renderer.h"
#include "opengl.h"
#include "camera.h"
#include "os/extensionfunctions.h"

sad::ShaderFunction::ShaderFunction() : m_shader(NULL), m_texture(NULL)
{
    
}

sad::ShaderFunction::ShaderFunction(const sad::ShaderFunction& fun) : m_shader(fun.m_shader), m_texture(fun.m_texture)  // NOLINT(bugprone-copy-constructor-init)
{
    if (m_shader)
    {
        m_shader->addRef();
    }
}

sad::ShaderFunction& sad::ShaderFunction::operator=(const sad::ShaderFunction& fun)
{
    if (m_shader)
    {
        m_shader->delRef();
    }

    m_shader = fun.m_shader;
    m_texture = fun.m_texture;

    if (m_shader)
    {
        m_shader->addRef();
    }

    return *this;
}

void sad::ShaderFunction::setShader(sad::Shader* shader)
{
    if (m_shader)
    {
        m_shader->delRef();
    }

    m_shader = shader;

    if (m_shader)
    {
        m_shader->addRef();
    }
}

sad::Shader* sad::ShaderFunction::shader() const
{
    return m_shader;
}

void sad::ShaderFunction::setTexture(sad::Texture* tex)
{
    m_texture = tex;
}

void sad::ShaderFunction::apply(sad::SceneNode* node)
{
    if (!node || !shader)
    {
        return;
    }
    if (node->scene() == NULL)
    {
        return;
    }
    m_shader->use();
    sad::Renderer* r = sad::Renderer::ref();
    if (m_shader->renderer())
    {
        r = m_shader->renderer();
    }
    sad::os::ExtensionFunctions* f = r->opengl()->extensionFunctions();
    sad::Camera* cam = node->scene()->getCamera();
    try
    {
        int matrixId = m_shader->getUniformLocation("_sglProjectionMatrix");
        if (matrixId != 0)
        {
            f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->projectionMatrix());
            m_shader->tryLogGlError("f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->projectionMatrix());");
        }

        matrixId = m_shader->getUniformLocation( "_sglModelViewMatrix");
        if (matrixId != 0)
        {
            f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->modelViewMatrix());
            m_shader->tryLogGlError("f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->modelViewMatrix()");
        }
        if (m_texture != NULL)
        {
            f->glActiveTexture(GL_TEXTURE0);
            int texId =  m_shader->getUniformLocation( "_defaultTexture");
            if (texId != 0)
            {
                f->glUniform1i(texId, 0);
                m_shader->tryLogGlError(" f->glUniform1i(texId, 0);");
            }
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

void sad::ShaderFunction::disable()
{
    m_shader->disable();
}

sad::ShaderFunction::~ShaderFunction()
{
    if (m_shader)
    {
        m_shader->delRef();
    }
}
