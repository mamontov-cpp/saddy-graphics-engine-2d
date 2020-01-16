#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "orthographiccamera.h"
#include "sadmutex.h"

// ReSharper disable once CppUnusedIncludeDirective
#include "os/glheaders.h"

#include "db/schema/schema.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/dbproperty.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/save.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/load.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <ctime>

sad::Scene::Scene()
: m_active(true), m_cached_layer(0), m_camera(new sad::OrthographicCamera()), m_renderer(NULL)
{
    m_camera->addRef();
    m_camera->setScene(this);
}

sad::Scene::~Scene()
{
    for (unsigned long i = 0; i < this->m_layers.count(); i++)
        m_layers[i]->delRef();
    m_camera->delRef();
}

void sad::Scene::reset()
{
    this->clear();
}

static sad::db::schema::Schema* SadSceneSchema;

static sad::Mutex SadSceneSchemaInit;

sad::db::schema::Schema* sad::Scene::basicSchema()
{
    if (SadSceneSchema == NULL)
    {
        SadSceneSchemaInit.lock();
        if (SadSceneSchema == NULL)
        {
            SadSceneSchema = new sad::db::schema::Schema();
            SadSceneSchema->addParent(sad::db::Object::basicSchema());  
            SadSceneSchema->add(
                "layer", 
                new sad::db::MethodPair<sad::Scene, unsigned int>(
                    &sad::Scene::sceneLayer,
                    &sad::Scene::setSceneLayer
                )
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(SadSceneSchema);
        }
        SadSceneSchemaInit.unlock();
    }
    return SadSceneSchema;
}


sad::db::schema::Schema* sad::Scene::schema() const
{
    return sad::Scene::basicSchema();
}


void sad::Scene::setRenderer(sad::Renderer * renderer)
{
    m_renderer = renderer;
    for (unsigned long i = 0; i < this->m_layers.count(); i++)
        m_layers[i]->rendererChanged();
}

void sad::Scene::clearRenderer()
{
    m_renderer = NULL;
}

sad::Camera* sad::Scene::getCamera() const
{
    return m_camera;
}

sad::Camera & sad::Scene::camera() const
{
    return *m_camera;
}

void sad::Scene::setCamera(sad::Camera * camera) 
{ 
    if (m_camera)
    {
        m_camera->delRef(); 
    }
    m_camera = camera;
    if (m_camera)
    {
        m_camera->setScene(this);
        m_camera->addRef();
    }
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
    if (pos1 != -1 && pos2 != -1)
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

  m_camera->restore();  
}

unsigned int sad::Scene::cachedSceneLayer() const
{   
    return m_cached_layer;
}

unsigned int sad::Scene::sceneLayer() const
{
    if (m_renderer)
    {
        return m_renderer->layer(const_cast<sad::Scene*>(this));
    }
    return m_cached_layer;
}

void sad::Scene::setSceneLayer(unsigned int layer)
{
    if (m_renderer)
    {
        m_renderer->setLayer(this, layer);
    }
    m_cached_layer = layer;
}

static sad::String SceneSerializableName = "sad::Scene";

const sad::String& sad::Scene::serializableName() const
{
    return SceneSerializableName;   
}

void sad::Scene::addNow(sad::SceneNode * node)
{
    node->addRef();
    bool mustchangerenderer = (node->renderer() == NULL) || (m_renderer != node->renderer());
    node->setScene(this);
    if (mustchangerenderer)
    {
        node->rendererChanged();
    }
    m_layers << node;
    if (node)
    {
        node->onAddedToScene();
    }
}

void sad::Scene::removeNow(sad::SceneNode * node)
{
    for(size_t i = 0; i < m_layers.count(); i++)
    {
        if (node == m_layers[i])
        {
            if (node)
            {
                node->onRemovedFromScene();
            }
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
