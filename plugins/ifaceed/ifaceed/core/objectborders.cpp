#include "objectborders.h"
#include "../objects/abstractscreenobject.h"
#include <renderer.h>

bool ActiveObjectBorder::tryPerform()
{
	AbstractScreenObject * o = this->m_data->activeObject();
	if (o) 
	{
		hRectF r = o->region();
		glDisable(GL_TEXTURE_2D);
		GLint   clr[4]={};
		glGetIntegerv(GL_CURRENT_COLOR,clr);
		glColor4ub(0,255,255,255);
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
	return false;
}


bool SelectedObjectBorder::tryPerform()
{
	AbstractScreenObject * o = this->m_data->selectedObject();
	if (o) 
	{
		hRectF r = o->region();
		glDisable(GL_TEXTURE_2D);
		GLint   clr[4]={};
		glGetIntegerv(GL_CURRENT_COLOR,clr);
		glColor4ub(255,0,0,255);
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
	return false;
}
