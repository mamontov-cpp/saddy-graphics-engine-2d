#include <sprite3d.h>
#include <geometry3d.h>
#include <renderer.h>
#include <sadmutex.h>
#include <glcontext.h>

#include <os/glheaders.h>
#include <os/gltexturedgeometry3d.h>
#include <os/gluntexturedgeometry3d.h>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"


DECLARE_SOBJ_INHERITANCE(sad::Sprite3D,sad::SceneNode);

sad::Sprite3D::Sprite3D() //-V730
: 
m_alpha(0),
m_theta(0),
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(0,0,0,0),
m_middle(0,0,0),
m_size(0,0),
m_renderable_area(sad::Point3D(0, 0), sad::Point3D(0, 0)),
m_color(sad::AColor(255,255,255,0)),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite3D::Sprite3D(		 //-V730
    sad::Texture * texture,
    const sad::Rect2D & texturecoordinates,
    const sad::Rect<sad::Point3D> & area,
    bool fast
)
: 
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_color(sad::AColor(255,255,255,0)),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    m_texture.attach(texture);
    normalizeTextureCoordinates();
    if (fast)
    {
        initFromRectangleFast(area);
    }
    else
    {
        initFromRectangle(area);
        buildRenderableArea();
    }
    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite3D::Sprite3D( //-V730
        const sad::String& texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect<sad::Point3D>& area,
        const sad::String& tree,
        bool fast
    )
: 
m_flipx(false),
m_flipy(false),
m_alpha(0),
m_theta(0),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_color(sad::AColor(255,255,255,0)),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    m_texture.setTree(nullptr, tree);
    m_texture.setPath(texture);
    normalizeTextureCoordinates();
    if (fast)
    {
        initFromRectangleFast(area);
    }
    else
    {
        initFromRectangle(area);
        buildRenderableArea();
    }
    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite3D::~Sprite3D()
{

}

static sad::db::schema::Schema* Sprite3DBasicSchema = nullptr;

static sad::Mutex Sprite3DBasicSchemaInit;

sad::db::schema::Schema* sad::Sprite3D::basicSchema()
{
    if (Sprite3DBasicSchema == nullptr)
    {
        Sprite3DBasicSchemaInit.lock();
        if (Sprite3DBasicSchema == nullptr)
        {
            Sprite3DBasicSchema = new sad::db::schema::Schema();
            Sprite3DBasicSchema->addParent(sad::SceneNode::basicSchema());
            Sprite3DBasicSchema->add(
                "texture", 
                new sad::db::MethodPair<sad::Sprite3D, sad::String>(
                    &sad::Sprite3D::textureName,
                    &sad::Sprite3D::setTextureName
                )
            );
            Sprite3DBasicSchema->add(
                "texturecoordinates", 
                new sad::db::MethodPair<sad::Sprite3D, sad::Rect2D>(
                    &sad::Sprite3D::textureCoordinates,
                    &sad::Sprite3D::setTextureCoordinates
                )
            );
            Sprite3DBasicSchema->add(
                "area", 
                new sad::db::MethodPair<sad::Sprite3D, sad::Rect<sad::Point3D> >(
                    &sad::Sprite3D::area,
                    &sad::Sprite3D::setRenderableArea
                )
            );
            Sprite3DBasicSchema->add(
                "angle", 
                new sad::db::MethodPair<sad::Sprite3D, double>(
                    &sad::Sprite3D::alpha,
                    &sad::Sprite3D::setAlpha
                )
            );
            Sprite3DBasicSchema->add(
                "theta", 
                new sad::db::MethodPair<sad::Sprite3D, double>(
                    &sad::Sprite3D::theta,
                    &sad::Sprite3D::setTheta
                )
            );
            Sprite3DBasicSchema->add(
                "color", 
                new sad::db::MethodPair<sad::Sprite3D, sad::AColor>(
                    &sad::Sprite3D::color,
                    &sad::Sprite3D::setColor
                )
            );
            Sprite3DBasicSchema->add(
                "flipx", 
                new sad::db::MethodPair<sad::Sprite3D, bool>(
                    &sad::Sprite3D::flipX,
                    &sad::Sprite3D::setFlipX
                )
            );
            Sprite3DBasicSchema->add(
                "flipy", 
                new sad::db::MethodPair<sad::Sprite3D, bool>(
                    &sad::Sprite3D::flipY,
                    &sad::Sprite3D::setFlipY
                )
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(Sprite3DBasicSchema);
        }
        Sprite3DBasicSchemaInit.unlock();
    }
    return Sprite3DBasicSchema;
}

sad::db::schema::Schema* sad::Sprite3D::schema() const
{
    return sad::Sprite3D::basicSchema();
}

void sad::Sprite3D::render()
{
    sad::Scene* scene = this->scene();
    if (!scene)
    {
        return;
    }
    sad::Renderer* r = scene->renderer();
    if (!r)
    {
        return;
    }
    sad::Texture * texture = m_texture.get();
    if (r->context()->isOpenGL3compatible())
    {
        sad::ShaderFunction* shader = this->shaderFunction();
        if (!shader)
        {
            shader = (texture) ? r->defaultShaderFunctionForTextures3d() : r->defaultShaderFunctionWithoutTextures3d();
        }
        if (texture)
        {
            m_was_textured = true;
            if (m_geometry_dirty)
            {
                m_g.m_textured_geometry = r->takeTextured3D();
                m_geometry_dirty = false;
            }
            sad::os::GLTexturedGeometry3D* g = m_g.m_textured_geometry;
            g->loadToGPU();
            if (m_tc_dirty)
            {
                g->setTextureCoordinates(m_normalized_texture_coordinates);
                m_tc_dirty = false;
            }
            if (m_vertexes_dirty)
            {
                g->setVertices(m_renderable_area);
                m_vertexes_dirty = false;
            }
            shader->apply(this, texture, &m_color);
            g->drawIndexedQuad();
        }
        else
        {
            m_was_textured = false;
            if (m_geometry_dirty)
            {
                m_g.m_untextured_geometry = r->takeUntextured3D();
                m_geometry_dirty = false;
            }
            sad::os::GLUntexturedGeometry3D* g = m_g.m_untextured_geometry;
            g->loadToGPU();
            if (m_tc_dirty)
            {
                m_tc_dirty = false;
            }
            if (m_vertexes_dirty)
            {
                g->setVertices(m_renderable_area);
                m_vertexes_dirty = false;
            }
            shader->apply(this, &m_color);
            g->drawIndexedQuad();
        }
        shader->disable();
    }
    else
    {
        glGetIntegerv(GL_CURRENT_COLOR, m_current_color_buffer);
        glColor4ub(m_color.r(), m_color.g(), m_color.b(), 255 - m_color.a());
        if (texture)
        {
            texture->bind();
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }
        glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            if (texture)
            {
                glTexCoord2f(
                    static_cast<GLfloat>(m_normalized_texture_coordinates[i].x()),
                    static_cast<GLfloat>(m_normalized_texture_coordinates[i].y())
                );
            }
            glVertex3f(
                static_cast<GLfloat>(m_renderable_area[i].x()),
                static_cast<GLfloat>(m_renderable_area[i].y()),
                static_cast<GLfloat>(m_renderable_area[i].z())
            );
        }
        glEnd();
        glColor4iv(m_current_color_buffer);
        if (!texture)
        {
            glEnable(GL_TEXTURE_2D);
        }
    }
}

void sad::Sprite3D::rendererChanged()
{
    if (m_texture.dependsOnRenderer())
    {
        m_texture.setRenderer(this->renderer());
    }
    tryReturnGeometry();
}

void sad::Sprite3D::setTextureCoordinates(const sad::Rect2D & texturecoordinates)
{
    m_texture_coordinates = texturecoordinates;
    normalizeTextureCoordinates();
    m_tc_dirty = true;
}

void sad::Sprite3D::setTextureCoordinate(int index, const sad::Point2D & point)
{
    m_texture_coordinates[index] = point;
    // Try  to immediately convert point to normalized if needed
    sad::Texture * tex = m_texture.get();
    if (tex)
    {
        sad::Point2D relativepoint(point.x() / tex->Width, point.y() / tex->Height );
        int newindex = 3 - index;

        // Take care of horizontal flip
        if (m_flipx)
        {
            switch(newindex) 
            {
                case 0 : { newindex = 1; break; }
                case 1 : { newindex = 0; break; }
                case 2 : { newindex = 3; break; }
                case 3 : { newindex = 2; break; }
            };
        }

        // Take care of vertical flip
        if (m_flipy)
        {
            switch(newindex) 
            {
                case 0 : { newindex = 3; break; }
                case 1 : { newindex = 2; break; }
                case 2 : { newindex = 1; break; }
                case 3 : { newindex = 0; break; }
            };
        }

        m_tc_dirty = true;
        m_normalized_texture_coordinates[newindex] = relativepoint;
    }
}

void sad::Sprite3D::setTextureCoordinate(int index, double x ,double y)
{
    this->setTextureCoordinate(index, sad::Point2D(x, y));
}


const sad::Rect2D & sad::Sprite3D::textureCoordinates() const
{
    return m_texture_coordinates;
}

void sad::Sprite3D::setRenderableArea(const sad::Rect<sad::Point3D> & rect)
{
    initFromRectangle(rect);
    buildRenderableArea();
    m_vertexes_dirty = true;
}

sad::Rect<sad::Point3D> sad::Sprite3D::area() const
{
    sad::Rect<sad::Point3D> result(
        m_middle + sad::Point3D(m_size.Width / -2,  m_size.Height / -2 , 0),
        m_middle + sad::Point3D(m_size.Width /  2,  m_size.Height / -2 , 0),
        m_middle + sad::Point3D(m_size.Width /  2,  m_size.Height / 2 , 0),
        m_middle + sad::Point3D(m_size.Width / -2,  m_size.Height / 2 , 0)
    );
    return result;
}

const sad::Rect<sad::Point3D> & sad::Sprite3D::renderableArea() const
{
    return m_renderable_area;
}

const sad::Point3D & sad::Sprite3D::point(int n) const
{
    return m_renderable_area[n];
}

const sad::Point3D & sad::Sprite3D::middle() const
{
    return m_middle;
}

void sad::Sprite3D::setMiddle(const sad::Point3D & p)
{
    m_middle = p;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

const sad::Size2D &  sad::Sprite3D::size() const
{
    return m_size;	
}

void sad::Sprite3D::setSize(const sad::Size2D & size)
{
    m_size = size;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

void sad::Sprite3D::moveBy(const sad::Point3D & dist)
{
    m_middle += dist;
    for (int i=0;i<4;i++)
    {
        m_renderable_area[i] += dist;
    }
    m_vertexes_dirty = true;
}

void sad::Sprite3D::moveBy(const sad::Point2D& p)
{
    sad::Point3D tmp(p.x(), p.y(), 0);
    m_vertexes_dirty = true;
    moveBy(tmp);
}

void sad::Sprite3D::moveTo(const sad::Point3D & p)
{
    sad::Point3D  dist = p;
    dist -= middle();
    m_vertexes_dirty = true;
    moveBy(dist);
}

bool sad::Sprite3D::canBeRotated() const
{
    return true;
}

void sad::Sprite3D::rotate(double angle)
{
    rotate(angle, 0);
}

void sad::Sprite3D::rotate(double alpha, double theta)
{
    m_alpha += alpha;
    m_theta += theta;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

void sad::Sprite3D::setAlpha(double alpha)
{
    m_alpha = alpha;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

void sad::Sprite3D::setTheta(double theta)
{
    m_theta = theta;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

double sad::Sprite3D::alpha() const
{
    return m_alpha;
}

double sad::Sprite3D::theta() const
{
    return m_theta;
}

void sad::Sprite3D::setColor(const sad::AColor & clr) 
{
    m_color = clr;
}

const sad::AColor & sad::Sprite3D::color() const
{
    return m_color;
}

void sad::Sprite3D::setFlipX(bool flipx)
{
    m_tc_dirty = true;
    bool changed = m_flipx != flipx;
    m_flipx = flipx;
    if (changed)
    {
        normalizeTextureCoordinates();
    }
}

void sad::Sprite3D::setFlipY(bool flipy)
{
    m_tc_dirty = true;
    bool changed = m_flipy != flipy;
    m_flipy = flipy;
    if (changed)
    {
        normalizeTextureCoordinates();
    }
}

bool sad::Sprite3D::flipX() const
{
    return m_flipx;
}

bool sad::Sprite3D::flipY() const
{
    return m_flipy;
}

void sad::Sprite3D::setTexture(sad::Texture * texture)
{
    m_texture.attach(texture);
    normalizeTextureCoordinates();

    tryReturnGeometry();
}

sad::Texture * sad::Sprite3D::texture() const
{
    return m_texture.get();
}

void sad::Sprite3D::setTextureName(const sad::String & name)
{
    m_texture.setPath(name);

    tryReturnGeometry();
    reloadTexture();
}

const sad::String& sad::Sprite3D::textureName()
{
    return m_texture.path();
}

void sad::Sprite3D::setScene(sad::Scene * scene)
{
    this->sad::SceneNode::setScene(scene);
    if (m_texture.dependsOnRenderer() && scene)
    {
        m_texture.setRenderer(scene->renderer());
        reloadTexture();
    }
    tryReturnGeometry();
}

void sad::Sprite3D::setTreeName(const sad::String & tree_name)
{
    m_texture.setTree(m_texture.renderer(), tree_name);
    reloadTexture();
    tryReturnGeometry();
}

void sad::Sprite3D::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    m_texture.setTree(r, tree_name);
    reloadTexture();
    tryReturnGeometry();
}

void sad::Sprite3D::onRemovedFromScene()
{
    tryReturnGeometry();
}

void sad::Sprite3D::initFromRectangleFast(const sad::Rect<sad::Point3D> & rect)
{
    m_alpha = 0;
    m_theta = 0;

    m_middle = rect[0];
    m_middle += rect[2];
    m_middle /= 2.0;

    m_size.Width = rect[0].distance(rect[1]);
    m_size.Height = rect[0].distance(rect[3]);

    m_renderable_area = rect;
}

void sad::Sprite3D::initFromRectangle(const sad::Rect<sad::Point3D> & rect)
{
    bool error = false;
    sad::Rect<sad::Point3D> baserect;
    sad::getBaseRect(rect, baserect, m_alpha, m_theta, &error);
    assert(!error);
    
    m_middle = baserect[0];
    m_middle += baserect[2];
    m_middle /= 2.0;

    m_size.Width = baserect[0].distance(baserect[1]);
    m_size.Height = baserect[0].distance(baserect[3]);
}

void sad::Sprite3D::buildRenderableArea()
{
    sad::Rect<sad::Point3D> baserect = this->area();
    sad::rotate(baserect, m_renderable_area, m_alpha, m_theta);
}

void sad::Sprite3D::reloadTexture()
{
    normalizeTextureCoordinates();
}

void sad::Sprite3D::normalizeTextureCoordinates()
{
    sad::Texture * texture = m_texture.get();
    if (texture)
    {
        for(int i = 0; i < 4; i++)
        {
            const sad::Point2D & point = m_texture_coordinates[i];
            sad::Point2D relativepoint(point.x() / texture->Width, point.y() / texture->Height );
            m_normalized_texture_coordinates[3 - i] = relativepoint;
        }
        if (m_flipx)
        {
            std::swap(m_normalized_texture_coordinates[0], m_normalized_texture_coordinates[1]);
            std::swap(m_normalized_texture_coordinates[2], m_normalized_texture_coordinates[3]);
        }
        if (m_flipy)
        {
            std::swap(m_normalized_texture_coordinates[0], m_normalized_texture_coordinates[3]);
            std::swap(m_normalized_texture_coordinates[1], m_normalized_texture_coordinates[2]);
        }
    }
}

void sad::Sprite3D::tryReturnGeometry()
{
    if (m_old_renderer && !m_geometry_dirty)
    {
        if (m_was_textured)
        {
            if (m_g.m_textured_geometry)
            {
                m_old_renderer->storeGeometry(m_g.m_textured_geometry);
            }
        }
        else
        {
            if (m_g.m_untextured_geometry)
            {
                m_old_renderer->storeGeometry(m_g.m_untextured_geometry);
            }
        }
    }
    m_geometry_dirty = true;
    m_tc_dirty = true;
    m_vertexes_dirty = true;
}
