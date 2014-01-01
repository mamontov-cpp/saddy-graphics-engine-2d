#include "scene.h"
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "orthographiccamera.h"

#include <time.h>

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
: m_camera(new sad::OrthographicCamera()), m_renderer(NULL)
{
	m_camera->Scene = this;
}

sad::Scene::~Scene()
{
	for (unsigned long i=0;i<this->m_layers.count();i++)
		this->onNodeRemoval(m_layers[i]);
	delete m_camera;
}


void sad::Scene::setCamera(sad::Camera * camera) 
{ 
	delete m_camera; 
	m_camera=camera; 
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

  performQueuedActions();
  lockChanges();
  for (unsigned long i=0;i<m_layers.count();++i)
  {
	  m_layers[i]->render();
  }
  unlockChanges();
  performQueuedActions();
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
