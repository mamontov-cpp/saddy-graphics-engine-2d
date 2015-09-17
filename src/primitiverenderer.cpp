#include "primitiverenderer.h"

#include "os/glheaders.h"

sad::PrimitiveRenderer::PrimitiveRenderer()
{
    
}

sad::PrimitiveRenderer::~PrimitiveRenderer()
{
    
}

void sad::PrimitiveRenderer::line(
        const sad::Point2D & p1,
        const sad::Point2D & p2,
        const sad::AColor & c
)
{
    glDisable(GL_TEXTURE_2D);
    GLint   clr[4]={};
    glGetIntegerv(GL_CURRENT_COLOR,clr);
    glColor4ub(c.r(),c.g(),c.b(),c.a());
    
    glBegin(GL_LINES);
    
    glVertex2f((GLfloat)p1.x(), (GLfloat)p1.y());
    glVertex2f((GLfloat)p2.x(), (GLfloat)p2.y());

    glEnd();
    
    glColor4iv(clr);
    glEnable(GL_TEXTURE_2D);
}

void sad::PrimitiveRenderer::rectangle(
        const sad::Rect2D & r,
        const sad::AColor & c
)
{
    glDisable(GL_TEXTURE_2D);
    GLint   clr[4]={};
    glGetIntegerv(GL_CURRENT_COLOR,clr);
    glColor4ub(c.r(),c.g(),c.b(),c.a());
    
    glBegin(GL_LINES);
    glVertex2f((GLfloat)r[0].x(), (GLfloat)r[0].y());
    glVertex2f((GLfloat)r[1].x(), (GLfloat)r[1].y());

    glVertex2f((GLfloat)r[1].x(), (GLfloat)r[1].y());
    glVertex2f((GLfloat)r[2].x(), (GLfloat)r[2].y());

    glVertex2f((GLfloat)r[2].x(), (GLfloat)r[2].y());
    glVertex2f((GLfloat)r[3].x(), (GLfloat)r[3].y());	

    glVertex2f((GLfloat)r[3].x(), (GLfloat)r[3].y());	
    glVertex2f((GLfloat)r[0].x(), (GLfloat)r[0].y());

    glEnd();
    glColor4iv(clr);
    glEnable(GL_TEXTURE_2D);
}
