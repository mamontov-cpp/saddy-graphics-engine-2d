#include <sprite2d.h>
#include <geometry2d.h>
#include <renderer.h>
#include <sadmutex.h>
#include <glcontext.h>

#include <os/glheaders.h>
#include <os/gltexturedgeometry2d.h>
#include <os/gluntexturedgeometry2d.h>

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "opticksupport.h"


DECLARE_SOBJ_INHERITANCE(sad::Sprite2D::Options, sad::resource::Resource);

bool sad::Sprite2D::Options::load(
    const sad::resource::ResourceFile & file,
    sad::Renderer * r,
    const picojson::value& options
)
{
    PROFILER_EVENT;
    std::ifstream stream(file.name().c_str());
    if (stream.bad())
    {
        sad::String newpath = util::concatPaths(r->executablePath(), file.name());
        stream.open(newpath.c_str());
    }	
    bool result = false;
    if (stream.good())
    {
        picojson::value v;
        stream >> v;
        if (picojson::get_last_error().empty())
        {
            result  = this->load(v);
        }
    }
    return result;
}

bool sad::Sprite2D::Options::load(const picojson::value& v)
{
    PROFILER_EVENT;
    sad::Maybe<sad::String> texture = picojson::to_type<sad::String>(
                picojson::get_property(v, "texture")
    );
    sad::Maybe<sad::Rect2D> texrect = picojson::to_type<sad::Rect2D>(
                picojson::get_property(v, "coordinates")
    );
    sad::Maybe<sad::Rect2D> rect = picojson::to_type<sad::Rect2D>(
                picojson::get_property(v, "rect")
    );
    bool result = texture.exists() && texrect.exists() && rect.exists();
    if (result)
    {
        Texture = texture.value();
        TextureRectangle = texrect.value();
        Rectangle = rect.value();
    }
    return result;
}

DECLARE_SOBJ_INHERITANCE(sad::Sprite2D,sad::SceneNode);

sad::Sprite2D::Sprite2D()
: 
m_angle(0),
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(0,0,0,0),
m_middle(0,0),
m_size(0,0),
m_renderable_area(sad::Point2D(0, 0), sad::Point2D(0, 0)),
m_color(sad::AColor(255,255,255,0)),
m_changesizeifoptionssizechanged(true),
m_explicit_set(false),
m_loading(false),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    PROFILER_EVENT;
    m_options.add(this, &sad::Sprite2D::onOptionsChange);
    m_texture.add(this, &sad::Sprite2D::onTextureChange);

    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite2D::Sprite2D(
    sad::Texture * texture,
    const sad::Rect2D & texturecoordinates,
    const sad::Rect2D & area,
    bool fast
)
: 
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_color(sad::AColor(255,255,255,0)),
m_changesizeifoptionssizechanged(true),
m_explicit_set(false),
m_loading(false),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    PROFILER_EVENT;
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
    m_options.add(this, &sad::Sprite2D::onOptionsChange);
    m_texture.add(this, &sad::Sprite2D::onTextureChange);

    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite2D::Sprite2D(
        const sad::String& texture,
        const sad::Rect2D& texturecoordinates,
        const sad::Rect2D& area,
        bool fast,
        const sad::String & treename
    )
: 
m_angle(0),
m_flipx(false),
m_flipy(false),
m_normalized_texture_coordinates(0, 0, 0, 0),
m_texture_coordinates(texturecoordinates),
m_color(sad::AColor(255,255,255,0)),
m_changesizeifoptionssizechanged(true),
m_explicit_set(false),
m_old_renderer(nullptr),
m_was_textured(false),
m_geometry_dirty(true),
m_tc_dirty(true),
m_vertexes_dirty(true)
{
    PROFILER_EVENT;
    m_texture.setTree(nullptr, treename);
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
    m_options.add(this, &sad::Sprite2D::onOptionsChange);
    m_texture.add(this, &sad::Sprite2D::onTextureChange);

    m_g.m_textured_geometry = nullptr;
    m_g.m_untextured_geometry = nullptr;
}

sad::Sprite2D::~Sprite2D()
{
    PROFILER_EVENT;

}

void sad::Sprite2D::regions(sad::Vector<sad::Rect2D> & r)
{
    PROFILER_EVENT;
    r << this->renderableArea();
}

static sad::db::schema::Schema* Sprite2DBasicSchema = nullptr;

static sad::Mutex Sprite2DBasicSchemaInit;

sad::db::schema::Schema* sad::Sprite2D::basicSchema()
{
    PROFILER_EVENT;
    if (Sprite2DBasicSchema == nullptr)
    {
        Sprite2DBasicSchemaInit.lock();
        if (Sprite2DBasicSchema == nullptr)
        {
            Sprite2DBasicSchema = new sad::db::schema::Schema();
            Sprite2DBasicSchema->addParent(sad::SceneNode::basicSchema());

            void (sad::Sprite2D::*p1)(const sad::String&) = &sad::Sprite2D::set;
            Sprite2DBasicSchema->add(
                "options", 
                new sad::db::MethodPair<sad::Sprite2D, sad::String>(
                    &sad::Sprite2D::optionsName,
                    p1
                )
            );
            Sprite2DBasicSchema->add(
                "area", 
                new sad::db::MethodPair<sad::Sprite2D, sad::Rect2D>(
                    &sad::Sprite2D::area,
                    &sad::Sprite2D::setArea
                )
            );
            Sprite2DBasicSchema->add(
                "texturecoordinates", 
                new sad::db::MethodPair<sad::Sprite2D, sad::Rect2D>(
                    &sad::Sprite2D::textureCoordinates,
                    &sad::Sprite2D::setTextureCoordinates
                )
            );
            Sprite2DBasicSchema->add(
                "angle", 
                new sad::db::MethodPair<sad::Sprite2D, double>(
                    &sad::Sprite2D::angle,
                    &sad::Sprite2D::setAngle
                )
            );
            Sprite2DBasicSchema->add(
                "color", 
                new sad::db::MethodPair<sad::Sprite2D, sad::AColor>(
                    &sad::Sprite2D::color,
                    &sad::Sprite2D::setColor
                )
            );
            Sprite2DBasicSchema->add(
                "flipx", 
                new sad::db::MethodPair<sad::Sprite2D, bool>(
                    &sad::Sprite2D::flipX,
                    &sad::Sprite2D::setFlipX
                )
            );
            Sprite2DBasicSchema->add(
                "flipy", 
                new sad::db::MethodPair<sad::Sprite2D, bool>(
                    &sad::Sprite2D::flipY,
                    &sad::Sprite2D::setFlipY
                )
            );

            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(Sprite2DBasicSchema);
        }
        Sprite2DBasicSchemaInit.unlock();
    }
    return Sprite2DBasicSchema;
}

sad::db::schema::Schema* sad::Sprite2D::schema() const
{
    PROFILER_EVENT;
    return sad::Sprite2D::basicSchema();
}

void sad::Sprite2D::render()
{
    PROFILER_EVENT;
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
    m_old_renderer = r;
    sad::Texture* tex = m_texture.get();
    if (r->context()->isOpenGL3compatible())
    {
        sad::ShaderFunction* shader = this->shaderFunction();
        if (!shader)
        {
            shader = (tex) ? r->defaultShaderFunctionForTextures2d() : r->defaultShaderFunctionWithoutTextures2d();
        }
        if (tex)
        {
            m_was_textured = true;
            if (m_geometry_dirty)
            {
                m_g.m_textured_geometry = r->takeTextured2D();
                m_geometry_dirty = false;
            }
            sad::os::GLTexturedGeometry2D* g = m_g.m_textured_geometry;
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
            shader->apply(this, tex, &m_color);
            g->drawIndexedQuad();
        } 
        else
        {
            m_was_textured = false;
            if (m_geometry_dirty)
            {
                m_g.m_untextured_geometry = r->takeUntextured2D();
                m_geometry_dirty = false;
            }
            sad::os::GLUntexturedGeometry2D* g = m_g.m_untextured_geometry;
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
        if (tex)
        {
            tex->bind();
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }
        glBegin(GL_QUADS);
        for (int i = 0; i < 4; i++)
        {
            if (tex)
            {
                glTexCoord2f(
                    static_cast<GLfloat>(m_normalized_texture_coordinates[i].x()),
                    static_cast<GLfloat>(m_normalized_texture_coordinates[i].y())
                );
            }
            glVertex2f(
                static_cast<GLfloat>(m_renderable_area[i].x()),
                static_cast<GLfloat>(m_renderable_area[i].y())
            );
        }
        glEnd();
        glColor4iv(m_current_color_buffer);
        if (!tex)
        {
            glEnable(GL_TEXTURE_2D);
        }
    }
}

void sad::Sprite2D::resetTextureAndOptions()
{
    PROFILER_EVENT;
    m_texture.detach();
    m_options.detach();
    m_tc_dirty = true;
    m_vertexes_dirty = true;
}

void sad::Sprite2D::rendererChanged()
{
    PROFILER_EVENT;
    sad::Renderer* new_renderer = this->renderer();
    if (m_options.dependsOnRenderer())
    {
        m_options.setRenderer(new_renderer);
    }
    m_options.get();
    if (m_texture.dependsOnRenderer())
    {
        m_texture.setRenderer(new_renderer);
    }
    tryReturnGeometry();
}


void sad::Sprite2D::setTextureCoordinates(const sad::Rect2D & texturecoordinates)
{
    PROFILER_EVENT;
    m_texture_coordinates = texturecoordinates;
    normalizeTextureCoordinates();
    m_tc_dirty = true;
}

void sad::Sprite2D::setTextureCoordinate(int index, const sad::Point2D & point)
{
    PROFILER_EVENT;
    sad::Texture * tex = m_texture.get();
    m_texture_coordinates[index] = point;
    // Try  to immediately convert point to normalized if needed
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

        m_normalized_texture_coordinates[newindex] = relativepoint;
        m_tc_dirty = true;
    }
}

void sad::Sprite2D::setTextureCoordinate(int index, double x ,double y)
{
    PROFILER_EVENT;
    this->setTextureCoordinate(index, sad::Point2D(x, y));
    m_tc_dirty = true;
}

const sad::Rect2D & sad::Sprite2D::textureCoordinates() const
{
    PROFILER_EVENT;
    return m_texture_coordinates;
}

void sad::Sprite2D::setRenderableArea(const sad::Rect2D & rect)
{
    PROFILER_EVENT;
    m_vertexes_dirty = true;
    initFromRectangle(rect);
    buildRenderableArea();
}

sad::Rect2D sad::Sprite2D::area() const
{
    PROFILER_EVENT;
    sad::Rect2D result(
        m_middle + sad::Point2D(m_size.Width / -2,  m_size.Height / -2),
        m_middle + sad::Point2D(m_size.Width /  2,  m_size.Height / -2),
        m_middle + sad::Point2D(m_size.Width /  2,  m_size.Height / 2),
        m_middle + sad::Point2D(m_size.Width / -2,  m_size.Height / 2)
    );
    return result;
}

void sad::Sprite2D::setArea(const sad::Rect2D & a)
{
    PROFILER_EVENT;
    m_middle = a[0];
    m_middle += a[2];
    m_middle /= 2.0;

    m_size.Width = a[0].distance(a[1]);
    m_size.Height = a[0].distance(a[3]);
    m_vertexes_dirty = true;

    buildRenderableArea();
}

void sad::Sprite2D::forceInvalidGeometry(const sad::Rect2D& quad)
{
    PROFILER_EVENT;
    m_middle = quad.p0();
    m_middle += quad.p1();
    m_middle += quad.p2();
    m_middle += quad.p3();

    m_middle /= 4.0;

    m_size.Width = std::max({quad.p0().x(), quad.p1().x(), quad.p2().x(), quad.p3().x()}) - std::min({ quad.p0().x(), quad.p1().x(), quad.p2().x(), quad.p3().x() });
    m_size.Height = std::max({ quad.p0().y(), quad.p1().y(), quad.p2().y(), quad.p3().y() }) - std::min({ quad.p0().y(), quad.p1().y(), quad.p2().y(), quad.p3().y() });

    m_vertexes_dirty = true;

    m_renderable_area = quad;
}

const sad::Rect2D & sad::Sprite2D::renderableArea() const
{
    PROFILER_EVENT;
    return m_renderable_area;
}

const sad::Point2D & sad::Sprite2D::point(int n) const
{
    PROFILER_EVENT;
    return m_renderable_area[n];
}

const sad::Point2D & sad::Sprite2D::middle() const
{
    PROFILER_EVENT;
    return m_middle;
}

void sad::Sprite2D::setMiddle(const sad::Point2D & p)
{
    PROFILER_EVENT;
    m_middle = p;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

const sad::Size2D &  sad::Sprite2D::size() const
{
    PROFILER_EVENT;
    return m_size;	
}

void sad::Sprite2D::setSize(const sad::Size2D & size, bool reg)
{
    PROFILER_EVENT;
    m_size = size;
    m_vertexes_dirty = true;
    buildRenderableArea();
}

void sad::Sprite2D::moveBy(const sad::Point2D & dist)
{
    PROFILER_EVENT;
    m_middle += dist;
    m_vertexes_dirty = true;
    for (int i=0;i<4;i++)
    {
        m_renderable_area[i] += dist;
    }
}

void sad::Sprite2D::moveTo(const sad::Point2D & p)
{
    PROFILER_EVENT;
    m_vertexes_dirty = true;
    sad::Point2D  dist = p;
    dist -= middle();
    moveBy(dist);
}

bool  sad::Sprite2D::canBeRotated() const
{
    PROFILER_EVENT;
    return true;
}

void sad::Sprite2D::rotate(double angle)
{
    PROFILER_EVENT;
    m_vertexes_dirty = true;
    m_angle += angle;
    buildRenderableArea();
}

void sad::Sprite2D::setAngle(double angle)
{
    PROFILER_EVENT;
    m_vertexes_dirty = true;
    m_angle = angle;
    buildRenderableArea();
}

double sad::Sprite2D::angle() const
{
    PROFILER_EVENT;
    return m_angle;
}

void sad::Sprite2D::setColor(const sad::AColor & clr) 
{
    PROFILER_EVENT;
    m_color = clr;
}

const sad::AColor & sad::Sprite2D::color() const
{
    PROFILER_EVENT;
    return m_color;
}

void sad::Sprite2D::setFlipX(bool flipx)
{
    PROFILER_EVENT;
    m_tc_dirty = true;
    bool changed = m_flipx != flipx;
    m_flipx = flipx;
    if (changed)
    {
        normalizeTextureCoordinates();
    }
}

void sad::Sprite2D::setFlipY(bool flipy)
{
    PROFILER_EVENT;
    m_tc_dirty = true;
    bool changed = m_flipy != flipy;
    m_flipy = flipy;
    if (changed)
    {
        normalizeTextureCoordinates();
    }
}

bool sad::Sprite2D::flipX() const
{
    PROFILER_EVENT;
    return m_flipx;
}

bool sad::Sprite2D::flipY() const
{
    PROFILER_EVENT;
    return m_flipy;
}

void sad::Sprite2D::setTexture(sad::Texture * texture)
{
    PROFILER_EVENT;
    m_texture.attach(texture);

    tryReturnGeometry();
}

sad::Texture * sad::Sprite2D::texture() const
{
    PROFILER_EVENT;
    return m_texture.get();
}

void sad::Sprite2D::setTexureName(const sad::String & name)
{
    PROFILER_EVENT;
    m_texture.setPath(name);
    reloadTexture();
    tryReturnGeometry();
}

const sad::String& sad::Sprite2D::textureName()
{
    PROFILER_EVENT;
    return m_texture.path();
}



void sad::Sprite2D::set(const sad::Sprite2D::Options & o)
{
    PROFILER_EVENT;
    if (!m_loading)
    {
        m_explicit_set = true;
    }
    // Make texture render dependent
    if (this->renderer() != nullptr)
    {
        m_texture.setRenderer(this->renderer());
    }
    m_options.attach(const_cast<sad::Sprite2D::Options *>(&o));
    tryReturnGeometry();
}

void sad::Sprite2D::set(const sad::String & optionsname)
{
    PROFILER_EVENT;
    const bool was_textured = m_was_textured;

    m_explicit_set = true;
    m_options.setPath(optionsname);

    sad::Renderer* my_renderer = this->renderer();
    // Make texture render dependent
    if (my_renderer != nullptr)
    {
        m_options.setRenderer(my_renderer);
        m_texture.setRenderer(my_renderer);
    }

    sad::Sprite2D::Options* opts = m_options.get();

    if (opts)
    {
        this->onOptionsChange(opts);
    }
    bool should_return = true;
    if ((m_old_renderer == my_renderer) && (m_old_renderer != nullptr))
    {
        sad::Texture* tex = m_texture.get();
        const bool has_texture = tex != nullptr;
        if (has_texture == was_textured)
        {
            should_return = false;
        }
    }
    if (should_return)
    {
        tryReturnGeometry();
    }
    else
    {
        m_tc_dirty = true;
        m_vertexes_dirty = true;
    }
}

sad::Sprite2D::Options* sad::Sprite2D::getOptions() const
{
    PROFILER_EVENT;
    return m_options.get();
}

const sad::String& sad::Sprite2D::optionsName() const
{
    PROFILER_EVENT;
    return m_options.path();
}

void sad::Sprite2D::setTreeName(const sad::String & treename)
{
    PROFILER_EVENT;
    m_options.setTree(m_options.renderer(), treename);
    m_texture.setTree(m_texture.renderer(), treename);
    tryReturnGeometry();
}

const sad::String & sad::Sprite2D::treeName() const
{
    PROFILER_EVENT;
    return m_texture.treeName();
}

void sad::Sprite2D::makeSpanBetweenPoints(
    const sad::Point2D & p1, 
    const sad::Point2D & p2
)
{
    PROFILER_EVENT;
    sad::Rect2D area = this->area();
    return makeSpanBetweenPoints(area, p1, p2);
}

void sad::Sprite2D::makeSpanBetweenPoints(const sad::Rect2D & r, const sad::Point2D & p1, const sad::Point2D & p2)
{
    PROFILER_EVENT;
    sad::Rect2D kr(r);
    for(int i = 0; i < 4; i++)
        kr[i].setX(0);

    double distance = p1.distance(p2) / 2;
    kr[0].setX(kr[0].x() - distance );
    kr[2].setX(kr[2].x() + distance );
    kr[1].setX(kr[1].x() + distance );
    kr[3].setX(kr[3].x() - distance );

    this->rotate(this->angle() * -1);
    this->setRenderableArea(kr);
    this->moveTo( (p1 + p2) / 2);
    double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
    if (angle < 0)
    {
        angle += 2 * M_PI;
    }
    this->rotate(angle);
}

void sad::Sprite2D::setScene(sad::Scene * scene)
{
    PROFILER_EVENT;
    this->sad::SceneNode::setScene(scene);
    if (scene)
    {
        if (m_texture.dependsOnRenderer())
        {
            m_texture.setRenderer(scene->renderer());
        }
        if (m_texture.dependsOnRenderer())
        {
            m_options.setRenderer(scene->renderer());
        }
    }
    tryReturnGeometry();
}

void sad::Sprite2D::setChangeSizeWhenOptionsAreChanged(bool flag)
{
    PROFILER_EVENT;
    m_changesizeifoptionssizechanged = flag;
}

bool sad::Sprite2D::changeSizeWhenOptionsAreChanged() const
{
    PROFILER_EVENT;
    return m_changesizeifoptionssizechanged;
}

void sad::Sprite2D::toggleLoadingMode(bool on)
{
    PROFILER_EVENT;
    m_loading = on;
}

bool sad::Sprite2D::load(const picojson::value& v)
{
    PROFILER_EVENT;
    toggleLoadingMode(true);
    bool result =  this->sad::SceneNode::load(v);
    toggleLoadingMode(false);
    return result;
}

void sad::Sprite2D::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    PROFILER_EVENT;
    m_options.setTree(r, tree_name);
    m_texture.setTree(r, tree_name);
    tryReturnGeometry();
}

void sad::Sprite2D::onRemovedFromScene()
{
    PROFILER_EVENT;
    tryReturnGeometry();
}

void sad::Sprite2D::initFromRectangleFast(const sad::Rect2D& rect)
{
    PROFILER_EVENT;
    m_angle = 0;

    m_middle = rect[0];
    m_middle += rect[2];
    m_middle /= 2.0;

    m_size.Width = rect[0].distance(rect[1]);
    m_size.Height = rect[0].distance(rect[3]);

    m_renderable_area = rect;
}

void sad::Sprite2D::initFromRectangle(const sad::Rect2D& rect)
{
    PROFILER_EVENT;
    bool error = false;
    sad::Rect2D baserect;
    sad::getBaseRect(rect, baserect, m_angle, &error);
    assert(!error);
    
    m_middle = baserect[0];
    m_middle += baserect[2];
    m_middle /= 2.0;

    m_size.Width = baserect[0].distance(baserect[1]);
    m_size.Height = baserect[0].distance(baserect[3]);
}

void sad::Sprite2D::buildRenderableArea()
{
    PROFILER_EVENT;
    m_renderable_area = this->area();
    sad::rotate(m_renderable_area, (float)m_angle);
}

void sad::Sprite2D::onTextureChange(sad::Texture * tex)
{
    PROFILER_EVENT;
    normalizeTextureCoordinates(tex);
}

void sad::Sprite2D::reloadTexture()
{
    PROFILER_EVENT;
    normalizeTextureCoordinates();
}

void sad::Sprite2D::normalizeTextureCoordinates()
{
    PROFILER_EVENT;
    sad::Texture * tex = m_texture.get();
    normalizeTextureCoordinates(tex);
}

void sad::Sprite2D::normalizeTextureCoordinates(sad::Texture * tex)
{
    PROFILER_EVENT;
    if (tex)
    {
        for(int i = 0; i < 4; i++)
        {
            const sad::Point2D & point = m_texture_coordinates[i];
            sad::Point2D relativepoint(point.x() / tex->Width, point.y() / tex->Height );
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

void sad::Sprite2D::tryReturnGeometry()
{
    PROFILER_EVENT;
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

void sad::Sprite2D::onOptionsChange(sad::Sprite2D::Options * opts)
{
    PROFILER_EVENT;
    sad::Renderer* r = this->renderer();
    if (r == nullptr)
    {
        r = m_options.renderer();
    }
    m_texture.setTree(r, m_options.treeName());
    m_texture.setPath(opts->Texture);
    if (m_loading == false)
    {
        m_texture_coordinates = opts->TextureRectangle;
    }
    reloadTexture();
    if (m_loading)
    {
        this->buildRenderableArea();
        m_explicit_set = false;
    }
    else 
    {
        if (m_changesizeifoptionssizechanged || m_explicit_set)
        {
            m_size = sad::Size2D(opts->Rectangle.width(), opts->Rectangle.height());
            this->buildRenderableArea();
            m_explicit_set = false;
        }
    }
}
