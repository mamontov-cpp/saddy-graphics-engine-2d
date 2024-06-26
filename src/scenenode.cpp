#include <stdexcept>

#include "scenenode.h"
#include "scene.h"
#include "sadmutex.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "opticksupport.h"

DECLARE_SOBJ(sad::SceneNode);

sad::SceneNode::SceneNode() : m_visible(true), m_scene(nullptr), m_cached_layer(0), m_cached_scene_id(0), m_shader_function(nullptr)
{
    PROFILER_EVENT;

}

void sad::SceneNode::regions(sad::Vector<sad::Rect2D> & r)
{
    PROFILER_EVENT;
    
}

sad::SceneNode::~SceneNode()
{
    PROFILER_EVENT;
    if (m_shader_function)
    {
        m_shader_function->delRef();
    }
}

static sad::db::schema::Schema* SceneNodeBasicSchema = nullptr;
static sad::Mutex SceneNodeBasicSchemaInit;

void sad::SceneNode::reset()
{
    PROFILER_EVENT;
    m_scene = nullptr;
    this->sad::db::Object::reset();
}

sad::db::schema::Schema* sad::SceneNode::basicSchema()
{
    PROFILER_EVENT;
    if (SceneNodeBasicSchema == nullptr)
    {
        SceneNodeBasicSchemaInit.lock();
        if (SceneNodeBasicSchema == nullptr)
        {
            SceneNodeBasicSchema = new sad::db::schema::Schema();
            SceneNodeBasicSchema->addParent(sad::db::Object::basicSchema());
            SceneNodeBasicSchema->add(
                "visible", 
                new sad::db::MethodPair<sad::SceneNode, bool>(
                    &sad::SceneNode::visible,
                    &sad::SceneNode::setVisible
                )
            );
            SceneNodeBasicSchema->add(
                "scene", 
                new sad::db::MethodPair<sad::SceneNode, unsigned long long>(
                    &sad::SceneNode::sceneId,
                    &sad::SceneNode::setCachedSceneId
                )
            );
            SceneNodeBasicSchema->add(
                "layer", 
                new sad::db::MethodPair<sad::SceneNode, unsigned int>(
                    &sad::SceneNode::cachedLayer,
                    &sad::SceneNode::setCachedLayer
                )
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(SceneNodeBasicSchema);
        }
        SceneNodeBasicSchemaInit.unlock();
    }
    return SceneNodeBasicSchema;
}

sad::db::schema::Schema* sad::SceneNode::schema() const
{
    PROFILER_EVENT;
    return sad::SceneNode::basicSchema();
}

void sad::SceneNode::rendererChanged()
{
    PROFILER_EVENT;
    
}

void sad::SceneNode::setScene(sad::Scene * scene)
{
    PROFILER_EVENT;
    m_scene = scene;
}

sad::Scene * sad::SceneNode::scene() const
{
    PROFILER_EVENT;
    return m_scene;
}

sad::Renderer * sad::SceneNode::renderer() const
{
    PROFILER_EVENT;
    if (scene())
    {
        return scene()->renderer();
    }
    return nullptr;
}

void sad::SceneNode::setCachedLayer(unsigned int layer)
{
    PROFILER_EVENT;
    m_cached_layer = layer;
    if (scene())
    {
        scene()->setLayer(this, layer);
    }
}

unsigned int sad::SceneNode::cachedLayer() const
{
    PROFILER_EVENT;
    if (scene())
    {
        sad::SceneNode * me = const_cast<sad::SceneNode *>(this);
        me->m_cached_layer = (unsigned int)(scene()->findLayer(me));
    }
    return m_cached_layer;
}

void sad::SceneNode::setCachedSceneId(unsigned long long scene_id)
{
    PROFILER_EVENT;
    m_cached_scene_id = scene_id;
}

unsigned long long  sad::SceneNode::sceneId() const
{
    PROFILER_EVENT;
    if (scene())
    {
        return scene()->MinorId;
    }
    return m_cached_scene_id;
}

void sad::SceneNode::moveBy(const sad::Point2D& p)
{
    PROFILER_EVENT;
    
}

bool sad::SceneNode::canBeRotated() const
{
    PROFILER_EVENT;
    return false;
}

void sad::SceneNode::rotate(double delta)
{
    PROFILER_EVENT;
    throw std::logic_error("this node cannot be rotated");
}

void sad::SceneNode::setShaderFunction(sad::ShaderFunction* fun)
{
    PROFILER_EVENT;
    if (m_shader_function)
    {
        m_shader_function->delRef();
    }
    m_shader_function = fun;
    if (m_shader_function)
    {
        m_shader_function->addRef();
    }
}

sad::ShaderFunction* sad::SceneNode::shaderFunction() const
{
    PROFILER_EVENT;
    return m_shader_function;
}

void sad::SceneNode::onAddedToScene()
{
    PROFILER_EVENT;
    
}


void sad::SceneNode::onRemovedFromScene()
{
    PROFILER_EVENT;
    
}
