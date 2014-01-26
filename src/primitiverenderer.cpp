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
	
	glVertex2f(p1.x(), p1.y());
	glVertex2f(p2.x(), p2.y());

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
	glVertex2f(r[0].x(), r[0].y());
	glVertex2f(r[1].x(), r[1].y());

	glVertex2f(r[1].x(), r[1].y());
	glVertex2f(r[2].x(), r[2].y());

	glVertex2f(r[2].x(), r[2].y());
	glVertex2f(r[3].x(), r[3].y());	

	glVertex2f(r[3].x(), r[3].y());	
	glVertex2f(r[0].x(), r[0].y());

	glEnd();
	glColor4iv(clr);
	glEnable(GL_TEXTURE_2D);
}
