#include "shaderfunction.h"
#include "renderer.h"
#include "opengl.h"
#include "camera.h"
#include "os/extensionfunctions.h"

sad::ShaderFunction::ShaderFunction() : m_shader(NULL)
{
    
}

sad::ShaderFunction::ShaderFunction(const sad::ShaderFunction& fun) : m_shader(fun.m_shader) // NOLINT(bugprone-copy-constructor-init)
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



void sad::ShaderFunction::apply(sad::SceneNode* node, sad::Texture* tex, sad::AColor* clr)
{
    if (!node || !m_shader)
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
        m_shader->tryLogGlError("sad::ShaderFunction::apply: on start filling data");
        int matrixId = m_shader->getUniformLocation("_sglProjectionMatrix");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: glGetUniformLocation(_sglProjectionMatrix)");
        if (matrixId != -1)
        {
            f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->projectionMatrix());
            m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->projectionMatrix());");
        }

        matrixId = m_shader->getUniformLocation( "_sglModelViewMatrix");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: glGetUniformLocation(_sglModelViewMatrix)");
        if (matrixId != -1)
        {
            f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->modelViewMatrix());
            m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniformMatrix4fv(matrixId, 1, GL_FALSE, cam->modelViewMatrix()");
        }
        if (tex != NULL)
        {
            f->glActiveTexture(GL_TEXTURE0);
            tex->bind();
            m_shader->tryLogGlError("sad::ShaderFunction::apply: tex->bind()");
            int texId =  m_shader->getUniformLocation( "_defaultTexture");
            m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_defaultTexture)");
            if (texId != -1)
            {
                f->glUniform1i(texId, 0);
                m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniform1i(texId, 0);");
            }
        }
        if (clr != NULL)
        {
            int clrId = m_shader->getUniformLocation("_gl_Color");
            m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_gl_Color)");
            if (clrId != -1)
            {
                f->glUniform4f(clrId, static_cast<float>(clr->r()) / 255.0f, static_cast<float>(clr->g()) / 255.0f, static_cast<float>(clr->b()) / 255.0f, 1.0f - static_cast<float>(clr->a()) / 255.0f);
                m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniform4f(clrId, ...);");
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
