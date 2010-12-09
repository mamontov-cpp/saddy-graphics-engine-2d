#include "scene.h"
#ifndef LINUX

#include <windows.h>
#include <gl/gl.h>														
#include <gl/glu.h>

#else

#include <GL/gl.h>														
#include <GL/glu.h>

#endif


int sad::BasicNode::Type=0;
int sad::BasicNode::type() const { return m_type;}

sad::BasicNode::BasicNode()
{
	m_type=sad::BasicNode::Type;
}
sad::BasicNode::~BasicNode()
{

}
sad::Scene::Scene()
{
 m_clear=false;
}
sad::Scene::~Scene()
{
	for (unsigned long i=0;i<this->m_layers.count();i++)
		delete m_layers[i];
	for (unsigned long i=0;i<this->m_marked.count();i++)
		delete m_marked[i];
	for (unsigned long i=0;i<this->m_toadd.count();i++)
		delete m_marked[i];
}
void sad::Scene::add(
		             BasicNode * node, 
		             const hst::string & name,
		             unsigned long lay
		            )
{
	unsigned long pos;

	if (lay==(unsigned long)-1)
	{
		this->m_layers.add(node);
		pos=m_layers.count()-1;
	}
	else
	{
		this->m_layers.insert(node,lay);
		pos=lay;
	}
	
	if (name.empty()) return;
    this->m_nodehash.insert(name,pos);
}

void sad::Scene::remove(const hst::string & name)
{
	if (m_nodehash.contains(name))
	{
		m_layers.removeAt(m_nodehash[name]);
		m_nodehash.remove(name);
	}
}

void sad::Scene::performCleanup()
{
	m_clear=true;
}
void sad::Scene::render()
{
  m_camera.apply();

  for (unsigned long i=0;i<m_layers.count();++i)
  {
	  m_layers[i]->render();
  }
  
  if (!(m_marked.count() || m_toadd.count() || m_clear)) return;
	
  if (!m_clear)
  {
   for (unsigned long i=0;i<m_marked.count();i++)
   {
	   for (unsigned long j=0;j<m_layers.count();j++)
	   {
		  if (m_layers[j]==m_marked[i])
		  {
			  delete m_layers[j];
			  m_layers.removeAt(j);
			  break;
		  }
	   }
   }      
  }
  else
  {
	  for (unsigned long i=0;i<m_layers.count();i++)
		  delete m_layers[i];
	  m_layers.clear();
	  m_clear=false;
  }

  for (unsigned long i=0;i<m_toadd.count();i++)
	      add(m_toadd[i].p1(),m_toadd[i].p2(),m_toadd[i].p3());
  
  m_toadd.clear();
  m_marked.clear();
}

void sad::Scene::markForDeletion(BasicNode * what)
{
	m_marked<<what;
}
void sad::Scene::markForAddition(BasicNode * node, const hst::string & name,unsigned long lay)
{
	m_toadd<<hst::triplet<BasicNode*,hst::string,unsigned long>(node,name,lay);
}
sad::Camera & sad::Scene::camera()
{
	return m_camera;
}

sad::Camera::~Camera()
{
}
sad::Camera::Camera()
{
	this->m_curX=0.0f;
    this->m_curY=0.0f;
    this->m_curZ=-1.0f;
    this->m_rotX=0.0f;
    this->m_rotY=0.0f;
    this->m_rotZ=0.0f;
	this->m_angle=0.0f;
}

sad::Camera::Camera(float x, float y, float z,float angle, float rx, float ry, float rz)
{
	this->m_curX=x;
	this->m_curY=y;
	this->m_curZ=z;
	this->m_rotX=rx;
	this->m_rotY=ry;
	this->m_rotZ=rz;
	this->m_angle=angle;
}
void sad::Camera::apply()
{
	glTranslatef(m_curX,m_curY,m_curZ);
	glRotatef(m_angle,m_rotX,m_rotY,m_rotZ);
}

void sad::Scene::clear()
{
	m_layers.clear();
	m_nodehash.clear();
	m_marked.clear();
	m_toadd.clear();
}
