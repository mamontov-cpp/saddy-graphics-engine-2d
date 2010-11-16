#include "testnode.h"
#include <math.h>


#ifndef LINUX

#include <windows.h>

#endif

#include <gl/gl.h>														
#include <gl/glu.h>													

TestNode::~TestNode()
{

}

TestNode::TestNode(float x, float y, float r)
{
	m_x=x;
	m_y=y;
	m_r=r;
	m_angle=0;
	m_type=TestNode::Type;
}

int TestNode::Type=1;

void TestNode::render()
{
    glBegin(GL_QUADS);

	glColor3f(255,0,0);
		glVertex3f(m_x+m_r*sin(m_angle),m_y+m_r*cos(m_angle),0.5);
	glColor3f(255,255,0);
		glVertex3f(m_x+m_r*sin(1.57f+m_angle),m_y+m_r*cos(1.57f+m_angle),0.5);
	glColor3f(255,0,255);
		glVertex3f(m_x+m_r*sin(3.14f+m_angle),m_y+m_r*cos(3.14f+m_angle),0.5);
	glColor3f(0,255,255);
		glVertex3f(m_x+m_r*sin(4.71f+m_angle),m_y+m_r*cos(4.71f+m_angle),0.5);
 glEnd();
	m_angle+=0.01f;
}