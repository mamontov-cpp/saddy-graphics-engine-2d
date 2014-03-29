#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "orthographiccamera.h"

#include "os/glheaders.h"

#include <time.h>

sad::Scene::Scene()
: m_active(true), m_camera(new sad::OrthographicCamera()), m_renderer(NULL)
{
	m_camera->Scene = this;
}

sad::Scene::~Scene()
{
	for (unsigned long i = 0; i < this->m_layers.count(); i++)
		m_layers[i]->delRef();
	delete m_camera;
}

void sad::Scene::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
	for (unsigned long i = 0; i < this->m_layers.count(); i++)
		m_layers[i]->rendererChanged();
}


sad::Camera & sad::Scene::camera()
{
	return *m_camera;
}

void sad::Scene::setCamera(sad::Camera * camera) 
{ 
	delete m_camera; 
	m_camera=camera; 
}

int sad::Scene::findLayer(sad::SceneNode * node)
{
	for (unsigned int i = 0; i < m_layers.count();i++) 
	{
		if (m_layers[i] == node)
			return i;
	}
	return -1;
}

void sad::Scene::setLayer(sad::SceneNode * node, unsigned int layer)
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

void sad::Scene::swapLayers(sad::SceneNode * node1, sad::SceneNode * node2)
{
	int pos1 = findLayer(node1);
	int pos2 = findLayer(node2);
	if (pos1!=-1 && pos2!=-1)
	{
		m_layers[pos1] = node2;
		m_layers[pos2] = node1;
	}
}

void sad::Scene::render()
{  
  m_camera->apply();

  performQueuedActions();
  lockChanges();
  for (unsigned long i = 0;i < m_layers.count(); ++i)
  {
#ifdef LOG_RENDERING
	  SL_LOCAL_INTERNAL(
		fmt::Format("Before rendering object {0} error code is {1}")
	        << m_layers[i]->metaData()->name()
		<< glGetError(), 
		*m_renderer
	);
#endif
	  sad::SceneNode * node = m_layers[i];
	  if (node->active() && node->visible())
	  {
			node->render();
	  }
#ifdef LOG_RENDERING
	  SL_LOCAL_INTERNAL(
		fmt::Format("After rendering object {0} error code is {1}")
	        << m_layers[i]->metaData()->name()
		<< glGetError(), 
		*m_renderer
	);
#endif
  }
  unlockChanges();
  performQueuedActions();
}

void sad::Scene::addNow(sad::SceneNode * node)
{
	node->addRef();
	node->setScene(this);
	m_layers << node;
}

void sad::Scene::removeNow(sad::SceneNode * node)
{
	for(size_t i = 0; i < m_layers.count(); i++)
	{
		if (node == m_layers[i])
		{
			node->delRef();
			m_layers.removeAt(i);
			--i;
		}
	}
}

void sad::Scene::clearNow()
{
	for(size_t i = 0; i < m_layers.count(); i++)
	{
		m_layers[i]->delRef();
	}
	m_layers.clear();
}
