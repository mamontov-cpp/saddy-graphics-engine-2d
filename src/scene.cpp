#include "scene.h"
#include <time.h>
#include <input.h>
#include <renderer.h>

#ifndef LINUX
	#ifndef NOMINMAX
    #define NOMINMAX 
    #endif
	#include <windows.h>
	#include <gl/gl.h>														
	#include <gl/glu.h>
#else
	#include <GL/gl.h>														
	#include <GL/glu.h>
#endif

DECLARE_SOBJ(sad::BasicNode);

sad::BasicNode::BasicNode()
{
}
sad::BasicNode::~BasicNode()
{
}

sad::Scene::Scene()
{

 m_camera   = new Camera();
 m_renderer = NULL;
}
sad::Scene::~Scene()
{
	for (unsigned long i=0;i<this->m_layers.count();i++)
		this->onNodeRemoval(m_layers[i]);
	delete m_camera;
}
void sad::Scene::addNow(sad::BasicNode * node)
{
	m_layers << node;
}

void sad::Scene::removeNow(sad::BasicNode * node)
{
	for(size_t i = 0; i < m_layers.count(); i++)
	{
		if (node == m_layers[i])
		{
			this->onNodeRemoval(node);
			m_layers.removeAt(i);
			--i;
		}
	}
}

void sad::Scene::clearNow()
{
	for(size_t i = 0; i < m_layers.count(); i++)
	{
		this->onNodeRemoval(m_layers[i]);
	}
	m_layers.clear();
}


void sad::Scene::render()
{
  
  m_camera->apply();

  this->m_renderer->controls()->preRender();

  performQueuedActions();
  lockChanges();
  for (unsigned long i=0;i<m_layers.count();++i)
  {
	  m_layers[i]->render();
  }
  unlockChanges();
  performQueuedActions();

  this->m_renderer->controls()->postRender();
}


void sad::Scene::onNodeRemoval(sad::BasicNode * node)
{
	delete node;
}

int sad::Scene::findLayer(sad::BasicNode * node)
{
	for (unsigned int i = 0;i<m_layers.count();i++) 
	{
		if (m_layers[i] == node)
			return i;
	}
	return -1;
}

void sad::Scene::swapLayers(sad::BasicNode * node1, sad::BasicNode * node2)
{
	int pos1 = findLayer(node1);
	int pos2 = findLayer(node2);
	if (pos1!=-1 && pos2!=-1)
	{
		m_layers[pos1] = node2;
		m_layers[pos2] = node1;
	}
}

void sad::Scene::setLayer(sad::BasicNode * node, unsigned int layer)
{
	int oldlayer = findLayer(node); 
	if (oldlayer!=-1)
	{
		m_layers.removeAt(oldlayer);
		if (layer >= m_layers.count())
		{
			m_layers << node;
		}
		else
		{
			m_layers.insert(node,layer);
		}
	}
}

sad::Camera & sad::Scene::camera()
{
	return *m_camera;
}

sad::Camera::~Camera()
{
}
sad::Camera::Camera()
{
	this->m_curX=0.0f;
    this->m_curY=0.0f;
    this->m_curZ=0.0f;
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


