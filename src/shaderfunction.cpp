#include "shaderfunction.h"
#include "renderer.h"
#include "opengl.h"
#include "camera.h"
#include "os/extensionfunctions.h"
#include "os/ubo.h"
#include "opticksupport.h"

sad::ShaderFunction::ShaderFunction() 
: m_shader(nullptr), 
m_locations_are_cached(false),
m_gl_camera_info_loc_id(-1),
m_tex_loc_id(-1),
m_clr_loc_id(-1)
{
    PROFILER_EVENT;
    
}

sad::ShaderFunction::ShaderFunction(const sad::ShaderFunction& fun) 
: m_shader(fun.m_shader),
m_locations_are_cached(false),
m_gl_camera_info_loc_id(-1),
m_tex_loc_id(-1),
m_clr_loc_id(-1)// NOLINT(bugprone-copy-constructor-init)
{
    PROFILER_EVENT;
    if (m_shader)
    {
        m_shader->addRef();
    }
}

sad::ShaderFunction& sad::ShaderFunction::operator=(const sad::ShaderFunction& fun)
{
    PROFILER_EVENT;
    if (m_shader)
    {
        m_shader->delRef();
    }

    m_shader = fun.m_shader;
    m_locations_are_cached = false;

    if (m_shader)
    {
        m_shader->addRef();
    }

    return *this;
}

void sad::ShaderFunction::setShader(sad::Shader* shader)
{
    PROFILER_EVENT;
    if (m_shader)
    {
        m_shader->delRef();
    }

    m_shader = shader;
    m_locations_are_cached = false;

    if (m_shader)
    {
        m_shader->addRef();
    }
}

sad::Shader* sad::ShaderFunction::shader() const
{
    PROFILER_EVENT;
    return m_shader;
}



void sad::ShaderFunction::apply(sad::SceneNode* node, sad::Bindable* tex, const sad::AColor* clr)
{
    PROFILER_EVENT;
    if (!node || !m_shader)
    {
        return;
    }
    if (node->scene() == nullptr)
    {
        return;
    }
    this->apply(node->scene(), tex, clr);
}

void sad::ShaderFunction::apply(sad::Scene* scene, sad::Bindable* tex, const sad::AColor* clr)
{
    PROFILER_EVENT;
    if (!scene || !m_shader)
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
    this->tryCacheLocations();
    try
    {
        if (m_gl_camera_info_loc_id != GL_INVALID_INDEX)
        {
            sad::os::UBO* ubo = scene->cameraBufferObject();
            m_shader->uniformBlockBinding(m_gl_camera_info_loc_id, 0);
            m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniformBlockBinding(m_gl_camera_info_loc_id, 0)");

            ubo->bind(0, 0);
        }

        f->glActiveTexture(GL_TEXTURE0);
        tex->bind();
        m_shader->tryLogGlError("sad::ShaderFunction::apply: tex->bind()");

        if (m_tex_loc_id != -1)
        {
            f->glUniform1i(m_tex_loc_id, 0);
            m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniform1i(texId, 0);");
        }

        if (clr != nullptr)
        {
            if (m_clr_loc_id != -1)
            {
                f->glUniform4f(m_clr_loc_id, static_cast<float>(clr->r()) / 255.0f, static_cast<float>(clr->g()) / 255.0f, static_cast<float>(clr->b()) / 255.0f, 1.0f - static_cast<float>(clr->a()) / 255.0f);
                m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniform4f(clrId, ...);");
            }
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

void sad::ShaderFunction::setColor(const sad::AColor& clr) const
{
    PROFILER_EVENT;
    sad::Renderer* r = sad::Renderer::ref();
    if (m_shader->renderer())
    {
        r = m_shader->renderer();
    }
    sad::os::ExtensionFunctions* f = r->opengl()->extensionFunctions();
    if(m_clr_loc_id != -1)
    {
        f->glUniform4f(m_clr_loc_id, static_cast<float>(clr.r()) / 255.0f, static_cast<float>(clr.g()) / 255.0f, static_cast<float>(clr.b()) / 255.0f, 1.0f - static_cast<float>(clr.a()) / 255.0f);
        m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniform4f(clrId, ...);");
    }
}

void sad::ShaderFunction::apply(sad::SceneNode* node, const sad::AColor* clr)
{
    PROFILER_EVENT;
    if (!node || !m_shader)
    {
        return;
    }
    if (node->scene() == nullptr)
    {
        return;
    }
    this->apply(node->scene(), clr);
}


void sad::ShaderFunction::apply(sad::Scene* scene, const sad::AColor* clr)
{
    PROFILER_EVENT;
    if (!scene || !m_shader)
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
    this->tryCacheLocations();
    try
    {
        m_shader->tryLogGlError("sad::ShaderFunction::apply: on start filling data");
        if (m_gl_camera_info_loc_id != GL_INVALID_INDEX)
        {
            sad::os::UBO* ubo = scene->cameraBufferObject();
            m_shader->uniformBlockBinding(m_gl_camera_info_loc_id, 0);
            m_shader->tryLogGlError("sad::ShaderFunction::apply: f->glUniformBlockBinding(m_gl_camera_info_loc_id, 0)");

            ubo->bind(0, 0);
        }

        if (clr != nullptr)
        {
            if (m_clr_loc_id != -1)
            {
                f->glUniform4f(m_clr_loc_id, static_cast<float>(clr->r()) / 255.0f, static_cast<float>(clr->g()) / 255.0f, static_cast<float>(clr->b()) / 255.0f, 1.0f - static_cast<float>(clr->a()) / 255.0f);
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
    PROFILER_EVENT;
    m_shader->disable();
}

bool sad::ShaderFunction::canBeUsedForFonts() const
{
    PROFILER_EVENT;
    return false;
}

sad::ShaderFunction::~ShaderFunction()
{
    PROFILER_EVENT;
    if (m_shader)
    {
        m_shader->delRef();
    }
}


void sad::ShaderFunction::tryCacheLocations()
{
    PROFILER_EVENT;
    if (!m_locations_are_cached)
    {
        m_locations_are_cached = true;

        m_gl_camera_info_loc_id =  m_shader->getUniformBlockIndex("_SGLCameraInfo");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: glGetUniformBlockIndex(_SGLCameraInfo)");

        m_tex_loc_id = m_shader->getUniformLocation("_defaultTexture");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_defaultTexture)");

        m_clr_loc_id = m_shader->getUniformLocation("_gl_Color");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_gl_Color)");
    }
}
