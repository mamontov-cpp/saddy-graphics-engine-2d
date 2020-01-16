#include "primitiverenderer.h"

#include "os/glheaders.h"
#include "os/gluntexturedgeometry2d.h"

#include "db/dbtypename.h"

#include "shaderfunction.h"
#include "scene.h"
#include "renderer.h"
#include "glcontext.h"
#include "camera.h"

sad::PrimitiveRenderer::PrimitiveRenderer()
{
    
}

sad::PrimitiveRenderer::~PrimitiveRenderer()
{
    
}

void sad::PrimitiveRenderer::line(
    sad::Scene* scene,
    const sad::Point2D & p1,
    const sad::Point2D & p2,
    const sad::AColor & c,
    sad::ShaderFunction* fun,
    bool no_shader_fun
)
{
    if (!scene)
    {
        return;
    }
    sad::Renderer* r = scene->renderer();
    if (!r)
    {
        r = sad::Renderer::ref();
    }
    if (r->context()->isOpenGL3compatible())
    {
        sad::ShaderFunction* f = fun;
        if (!f)
        {
            f = r->defaultShaderFunctionWithoutTextures2d();
        }
        if (!no_shader_fun)
        {
            scene->getCamera()->moveMatricesIntoCameraBuffer();
            f->apply(scene, &c);
        }
        sad::os::GLUntexturedGeometry2D* geometry = r->untexturedGeometry2DForPoints(2);
        geometry->drawLine(p1, p2);
        if (!no_shader_fun)
        {
            f->disable();
        }
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        GLint   clr[4] = {};
        glGetIntegerv(GL_CURRENT_COLOR, clr);
        glColor4ub(c.r(), c.g(), c.b(), 255 - c.a());

        glBegin(GL_LINES);

        glVertex2f(static_cast<GLfloat>(p1.x()), static_cast<GLfloat>(p1.y()));
        glVertex2f(static_cast<GLfloat>(p2.x()), static_cast<GLfloat>(p2.y()));

        glEnd();

        glColor4iv(clr);
        glEnable(GL_TEXTURE_2D);
    }
}

void sad::PrimitiveRenderer::rectangle(
    sad::Scene* scene,
    const sad::Rect2D & r,
    const sad::AColor & c,
    sad::ShaderFunction* fun
)
{
    if (!scene)
    {
        return;
    }
    sad::Renderer* renderer = scene->renderer();
    if (!renderer)
    {
        renderer = sad::Renderer::ref();
    }
    if (renderer->context()->isOpenGL3compatible())
    {
        sad::ShaderFunction* f = fun;
        if (!f)
        {
            f = renderer->defaultShaderFunctionWithoutTextures2d();
        }
        scene->getCamera()->moveMatricesIntoCameraBuffer();
        f->apply(scene, &c);
        sad::os::GLUntexturedGeometry2D* geometry = renderer->untexturedGeometry2DForPoints(4);
        geometry->drawRectLines(r);
        f->disable();
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        GLint   clr[4] = {};
        glGetIntegerv(GL_CURRENT_COLOR, clr);
        glColor4ub(c.r(), c.g(), c.b(), 255 - c.a());

        glBegin(GL_LINES);
        glVertex2f(static_cast<GLfloat>(r[0].x()), static_cast<GLfloat>(r[0].y()));
        glVertex2f(static_cast<GLfloat>(r[1].x()), static_cast<GLfloat>(r[1].y()));

        glVertex2f(static_cast<GLfloat>(r[1].x()), static_cast<GLfloat>(r[1].y()));
        glVertex2f(static_cast<GLfloat>(r[2].x()), static_cast<GLfloat>(r[2].y()));

        glVertex2f(static_cast<GLfloat>(r[2].x()), static_cast<GLfloat>(r[2].y()));
        glVertex2f(static_cast<GLfloat>(r[3].x()), static_cast<GLfloat>(r[3].y()));

        glVertex2f(static_cast<GLfloat>(r[3].x()), static_cast<GLfloat>(r[3].y()));
        glVertex2f(static_cast<GLfloat>(r[0].x()), static_cast<GLfloat>(r[0].y()));

        glEnd();
        glColor4iv(clr);
        glEnable(GL_TEXTURE_2D);
    }
}

DECLARE_COMMON_TYPE(sad::PrimitiveRenderer)