#include "fontshaderfunction.h"
#include "renderer.h"
#include "opengl.h"
#include "os/extensionfunctions.h"

// =============================== PUBLIC METHODS ===============================

sad::FontShaderFunction::FontShaderFunction() : ShaderFunction(), m_center_loc_id(-1), m_angle_loc_id(-1)
{

}

sad::FontShaderFunction::FontShaderFunction(const sad::ShaderFunction& fun) : ShaderFunction(fun), m_center_loc_id(-1), m_angle_loc_id(-1)
{

}

sad::FontShaderFunction& sad::FontShaderFunction::operator=(const sad::FontShaderFunction& fun)
{
    this->sad::ShaderFunction::operator=(fun);
    return *this;
}

void sad::FontShaderFunction::apply(sad::SceneNode* node, sad::Texture* tex, const sad::AColor* clr, const sad::Point2D& center, double angle)
{
    this->sad::ShaderFunction::apply(node, tex, clr);
    sad::Renderer* r = sad::Renderer::ref();
    if (m_shader->renderer())
    {
        r = m_shader->renderer();
    }
    sad::os::ExtensionFunctions* f = r->opengl()->extensionFunctions();
    try
    {
        if (m_center_loc_id != -1)
        {
            f->glUniform2f(m_center_loc_id, static_cast<float>(center.x()), static_cast<float>(center.y()));
            m_shader->tryLogGlError("sad::FontShaderFunction::apply: f->glUniform2f(m_center_loc_id, ...);");
        }

        if (m_angle_loc_id != -1)
        {
            f->glUniform1f(m_angle_loc_id, static_cast<float>(angle));
            m_shader->tryLogGlError("sad::FontShaderFunction::apply: f->glUniform2f(m_angle_loc_id, ...);");
        }
    }
    catch (std::logic_error& ex) {
        SL_LOCAL_FATAL(ex.what(), *r);
    }
}

bool sad::FontShaderFunction::canBeUsedForFonts() const
{
    return true;
}

sad::FontShaderFunction::~FontShaderFunction()
{

}

// =============================== PRIVATE METHODS ===============================

void sad::FontShaderFunction::tryCacheLocations()
{
    if (!m_locations_are_cahed)
    {
        m_locations_are_cahed = true;

        m_gl_camera_info_loc_id = m_shader->getUniformBlockIndex("_SGLCameraInfo");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: glGetUniformBlockIndex(_SGLCameraInfo)");

        m_tex_loc_id = m_shader->getUniformLocation("_defaultTexture");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_defaultTexture)");

        m_clr_loc_id = m_shader->getUniformLocation("_gl_Color");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(_gl_Color)");

        m_center_loc_id = m_shader->getUniformLocation("center");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(m_center_loc_id)");

        m_angle_loc_id = m_shader->getUniformLocation("angle");
        m_shader->tryLogGlError("sad::ShaderFunction::apply: sad::ShaderFunction::apply: glGetUniformLocation(m_angle_loc_id)");
    }
}