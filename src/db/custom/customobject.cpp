#include "db/custom/customobject.h"

#include "label.h"
#include "sprite2d.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::db::custom::Object,sad::SceneNode);

sad::db::custom::Object::Object()
{
    PROFILER_EVENT;
    m_sprite2d = new sad::Sprite2D();
    m_label = new sad::Label();
    m_my_schema = new sad::db::schema::Schema();
    m_custom_schema = new sad::db::schema::Schema();
    m_current_rendered_object = nullptr;
    // Make schema renderer-dependent
    m_schema.setTree(sad::Renderer::ref());
    m_schema.add(this, &sad::db::custom::Object::updateConfiguration);
    this->initDefaultSchema();
}

sad::db::custom::Object::~Object()
{
    PROFILER_EVENT;
    delete m_sprite2d;
    delete m_label;
    delete m_my_schema;
    if (m_custom_schema)
    {
        delete m_custom_schema;
    }
}

void sad::db::custom::Object::setScene(sad::Scene * scene)
{
    PROFILER_EVENT;
    this->sad::SceneNode::setScene(scene);
    m_label->setScene(scene);
    m_sprite2d->setScene(scene);
}

bool sad::db::custom::Object::innerTypeIs(const sad::String &type)
{
    PROFILER_EVENT;
    bool result = false;
    if (m_current_rendered_object) 
    {
        result = m_current_rendered_object->metaData()->canBeCastedTo(type);
    }
    return result;
}
void sad::db::custom::Object::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
    PROFILER_EVENT;
    m_schema.setTree(r, tree_name);
    m_sprite2d->setTreeName(r, tree_name);
    m_label->setTreeName(r, tree_name);
}

sad::db::schema::Schema* sad::db::custom::Object::schema() const
{
    PROFILER_EVENT;
    return m_custom_schema;
}

void sad::db::custom::Object::regions(sad::Vector<sad::Rect2D> & r)
{
    PROFILER_EVENT;
    r << this->renderableArea();
}

sad::Rect2D sad::db::custom::Object::renderableArea() const
{
    PROFILER_EVENT;
    if (m_current_rendered_object == m_sprite2d)
    {
        return m_sprite2d->renderableArea();
    }
    if (m_current_rendered_object == m_label)
    {
        return m_label->region();
    }
    return sad::Rect2D();
}

void sad::db::custom::Object::render()
{
    PROFILER_EVENT;
    if (m_current_rendered_object)
    {
        m_current_rendered_object->setScene(m_scene);
        m_current_rendered_object->render();
    }
}

void sad::db::custom::Object::setSchemaName(const sad::String & s)
{
    PROFILER_EVENT;
    m_schema.setPath(s);
    updateConfiguration(m_schema.get());
}

const sad::String& sad::db::custom::Object::schemaName() const
{
    PROFILER_EVENT;
    return m_schema.path();
}


unsigned int sad::db::custom::Object::fontSize() const
{
    PROFILER_EVENT;
    return m_label->size();
}

void sad::db::custom::Object::setFontSize(unsigned int size)
{
    PROFILER_EVENT;
    m_label->setSize(size);
}

const sad::String& sad::db::custom::Object::fontName() const
{
    PROFILER_EVENT;
    return m_label->fontName();
}

void sad::db::custom::Object::setFontName(const sad::String& name)
{
    PROFILER_EVENT;
    m_label->setFontName(name);
}

float sad::db::custom::Object::lineSpacingRatio() const
{
    PROFILER_EVENT;
    return m_label->lineSpacingRatio();
}

void sad::db::custom::Object::setLineSpacingRatio(float s)
{
    PROFILER_EVENT;
    m_label->setLineSpacingRatio(s);	
}

void sad::db::custom::Object::setMaximalLineWidth(unsigned int width)
{
    PROFILER_EVENT;
    m_label->setMaximalLineWidth(width);
}

bool sad::db::custom::Object::canBeRotated() const
{
    PROFILER_EVENT;
   return true;
}

void sad::db::custom::Object::rotate(double angle)
{
    PROFILER_EVENT;
    this->setAngle(this->angle() + angle);
}

double sad::db::custom::Object::angle() const
{
    PROFILER_EVENT;
    return m_label->angle();
}

void sad::db::custom::Object::setAngle(double a)
{
    PROFILER_EVENT;
    m_label->setAngle(a);
    m_sprite2d->setAngle(a);
}


sad::Rect2D sad::db::custom::Object::area() const
{
    PROFILER_EVENT;
    if (m_current_rendered_object == m_label)
    {
        return m_label->area();
    }
    return m_sprite2d->area();	
}


void sad::db::custom::Object::setArea(const sad::Rect2D & area)
{
    PROFILER_EVENT;
    m_label->setArea(area);
    m_sprite2d->setArea(area);
}

const sad::String& sad::db::custom::Object::string() const
{
    PROFILER_EVENT;
    return m_label->string();
}

void sad::db::custom::Object::setString(const sad::String & s)
{
    PROFILER_EVENT;
    m_label->setString(s);
}

const sad::AColor& sad::db::custom::Object::color() const
{
    PROFILER_EVENT;
    return m_label->color();
}

void sad::db::custom::Object::setColor(const sad::AColor & c)
{
    PROFILER_EVENT;
    m_label->setColor(c);
    m_sprite2d->setColor(c);
}

bool sad::db::custom::Object::flipX() const
{
    PROFILER_EVENT;
    return m_sprite2d->flipX();
}

void sad::db::custom::Object::setFlipX(bool f)
{
    PROFILER_EVENT;
    m_sprite2d->setFlipX(f);
}

bool sad::db::custom::Object::flipY() const
{
    PROFILER_EVENT;
    return m_sprite2d->flipY();
}

void sad::db::custom::Object::setFlipY(bool f)
{
    PROFILER_EVENT;
    m_sprite2d->setFlipY(f);
}

unsigned int sad::db::custom::Object::maximalLineWidth() const
{
    PROFILER_EVENT;
    return m_label->maximalLineWidth();
}

bool sad::db::custom::Object::canBeRendered() const
{
    PROFILER_EVENT;
    return m_current_rendered_object != nullptr;
}

static sad::Hash<sad::String, sad::db::Property*>  SadDbCustomObjectEmptyProperties;

const sad::Hash<sad::String, sad::db::Property*>&  sad::db::custom::Object::schemaProperties() const
{
    PROFILER_EVENT;
    if (!m_custom_schema)
    {
        return SadDbCustomObjectEmptyProperties;
    }
    return m_custom_schema->ownProperties();
}

void sad::db::custom::Object::setOptions(const sad::String& o)
{
    PROFILER_EVENT;
    m_sprite2d->set(o);
}

const sad::String& sad::db::custom::Object::options() const
{
    PROFILER_EVENT;
    return m_sprite2d->optionsName();
}

void sad::db::custom::Object::setTextureCoordinates(const sad::Rect2D & texture_coordinates)
{
    PROFILER_EVENT;
    m_sprite2d->setTextureCoordinates(texture_coordinates);
}

const sad::Rect2D & sad::db::custom::Object::textureCoordinates() const
{
    PROFILER_EVENT;
    return  m_sprite2d->textureCoordinates();
}


void sad::db::custom::Object::setOverflowStrategy(sad::Label::OverflowStrategy s)
{
    PROFILER_EVENT;
    m_label->setOverflowStrategy(s);
}

void sad::db::custom::Object::setOverflowStrategyFromIndex(unsigned int s)
{
    PROFILER_EVENT;
    m_label->setOverflowStrategyFromIndex(s);
}

sad::Label::OverflowStrategy sad::db::custom::Object::overflowStrategy() const
{
    PROFILER_EVENT;
    return m_label->overflowStrategy();
}

unsigned int sad::db::custom::Object::overflowStrategyAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(overflowStrategy());
}

void sad::db::custom::Object::setBreakText(sad::Label::BreakText value)
{
    PROFILER_EVENT;
     m_label->setBreakText(value);
}


void sad::db::custom::Object::setBreakTextFromIndex(unsigned int value)
{
    PROFILER_EVENT;
    m_label->setBreakTextFromIndex(value);
}

sad::Label::BreakText sad::db::custom::Object::breakText() const
{
    PROFILER_EVENT;
    return m_label->breakText();
}

unsigned int sad::db::custom::Object::breakTextAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(breakText());
}

void sad::db::custom::Object::setTextEllipsisPosition(sad::Label::TextEllipsisPosition value)
{
    PROFILER_EVENT;
    m_label->setTextEllipsisPosition(value);
}

void sad::db::custom::Object::setTextEllipsisPositionAsIndex(unsigned int value)
{
    PROFILER_EVENT;
    m_label->setTextEllipsisPositionAsIndex(value);
}

sad::Label::TextEllipsisPosition sad::db::custom::Object::textEllipsis() const
{
    PROFILER_EVENT;
    return m_label->textEllipsis();
}

unsigned int sad::db::custom::Object::textEllipsisAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(textEllipsis());
}

void sad::db::custom::Object::setMaximalLinesCount(unsigned int line_count)
{
    PROFILER_EVENT;
    m_label->setMaximalLinesCount(line_count);
}

unsigned int sad::db::custom::Object::maximalLinesCount() const
{
    PROFILER_EVENT;
    return m_label->maximalLinesCount();
}

void sad::db::custom::Object::setOverflowStrategyForLines(sad::Label::OverflowStrategy s)
{
    PROFILER_EVENT;
    m_label->setOverflowStrategyForLines(s);    
}

void sad::db::custom::Object::setOverflowStrategyForLinesFromIndex(unsigned int s)
{
    PROFILER_EVENT;
    m_label->setOverflowStrategyForLinesFromIndex(s);
}

sad::Label::OverflowStrategy sad::db::custom::Object::overflowStrategyForLines() const
{
    PROFILER_EVENT;
    return m_label->overflowStrategyForLines();
}

unsigned int sad::db::custom::Object::overflowStrategyForLinesAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(overflowStrategyForLines());
}

void sad::db::custom::Object::setTextEllipsisPositionForLines(sad::Label::TextEllipsisPosition value)
{
    PROFILER_EVENT;
    m_label->setTextEllipsisPositionForLines(value);
}

void sad::db::custom::Object::setTextEllipsisPositionForLinesAsIndex(unsigned int value)
{
    PROFILER_EVENT;
    m_label->setTextEllipsisPositionForLinesAsIndex(value);
}

sad::Label::TextEllipsisPosition  sad::db::custom::Object::textEllipsisForLines() const
{
    PROFILER_EVENT;
    return m_label->textEllipsisForLines();
}

unsigned int sad::db::custom::Object::textEllipsisForLinesAsIndex() const
{
    PROFILER_EVENT;
    return static_cast<unsigned int>(textEllipsisForLines());    
}

bool sad::db::custom::Object::hasFormatting() const
{
    PROFILER_EVENT;
    return m_label->hasFormatting();
}

void sad::db::custom::Object::setHasFormatting(bool value)
{
    PROFILER_EVENT;
    m_label->setHasFormatting(value);
}

void sad::db::custom::Object::makeFormatted()
{
    PROFILER_EVENT;
    this->setHasFormatting(true);
}

void sad::db::custom::Object::disableFormatting()
{
    PROFILER_EVENT;
    this->setHasFormatting(false);
}

bool sad::db::custom::Object::copyCustomPropertyValuesFrom(sad::db::custom::Object* o)
{
    PROFILER_EVENT;
    bool can_copy = false;
    sad::Vector<sad::String> names;

    if (o)
    {
        if (this->schemaName() == o->schemaName())
        {
            const sad::Hash<sad::String, sad::db::Property*>& props = o->schemaProperties();
            for(sad::Hash<sad::String, sad::db::Property*>::const_iterator it = props.const_begin(); it != props.const_end(); ++it)
            {
                names << it.key();
            }
            can_copy = true;
            for(size_t i = 0; i < names.size(); i++)
            {
                bool can_copy_property = false;
                sad::db::Property* myprop = this->getObjectProperty(names[i]);
                sad::db::Property* oprop = o->getObjectProperty(names[i]);
                if (myprop != nullptr && oprop != nullptr)
                {
                    sad::db::Variant v;
                    oprop->get(o, v);
                    can_copy_property = myprop->couldBeSetFrom(v);
                }
                can_copy =  can_copy && can_copy_property;
            }
        }
    }

    if (can_copy)
    {
        for(size_t i = 0; i < names.size(); i++)
        {
            sad::db::Property* myprop = this->getObjectProperty(names[i]);
            sad::db::Property* oprop = o->getObjectProperty(names[i]);
            if (myprop != nullptr && oprop != nullptr)
            {
                sad::db::Variant v;
                oprop->get(o, v);
                myprop->set(this, v);
            }
        }
    }
    return can_copy;
}

void sad::db::custom::Object::makeSpanBetweenPoints(
    const sad::Point2D & p1, 
    const sad::Point2D & p2
)
{
    PROFILER_EVENT;
    if (m_label)
    {
        m_label->makeSpanBetweenPoints(p1, p2);
    }
    if (m_sprite2d)
    {
        m_sprite2d->makeSpanBetweenPoints(p1, p2);
    }
}

void sad::db::custom::Object::moveBy(const sad::Point2D & dist)
{
    PROFILER_EVENT;
   if (m_label)
   {
       m_label->moveBy(dist);
   }

   if (m_sprite2d)
   {
       m_sprite2d->moveBy(dist);
   }
}

unsigned int  sad::db::custom::Object::renderedStringLength() const
{
    PROFILER_EVENT;
    return m_label->renderedStringLength();
}

void sad::db::custom::Object::setRenderingStringLimit(unsigned int limit)
{
    PROFILER_EVENT;
    m_label->setRenderingStringLimit(limit);
}

void sad::db::custom::Object::clearRenderingStringLimit()
{
    PROFILER_EVENT;
    m_label->clearRenderingStringLimit();
}


void sad::db::custom::Object::setRenderingStringLimitAsRatioToLength(double limit)
{
    PROFILER_EVENT;
    m_label->setRenderingStringLimitAsRatioToLength(limit);
}

void sad::db::custom::Object::setShaderFunction(sad::ShaderFunction* fun)
{
    PROFILER_EVENT;
    m_sprite2d->setShaderFunction(fun);
    m_label ->setShaderFunction(fun);
    this->sad::SceneNode::setShaderFunction(fun);
}

void sad::db::custom::Object::rendererChanged()
{
    PROFILER_EVENT;
    m_sprite2d->rendererChanged();
    m_label->rendererChanged();
}

void sad::db::custom::Object::onAddedToScene()
{
    PROFILER_EVENT;
    m_sprite2d->onAddedToScene();
    m_label->onAddedToScene();
}

void sad::db::custom::Object::onRemovedFromScene()
{
    PROFILER_EVENT;
    m_sprite2d->onRemovedFromScene();
    m_label->onRemovedFromScene();
}

void sad::db::custom::Object::setTexture(sad::Texture* tex) const
{
    PROFILER_EVENT;
    if (m_sprite2d)
    {
        m_sprite2d->setTexture(tex);
    }
}

sad::Texture* sad::db::custom::Object::texture() const
{
    PROFILER_EVENT;
    if (m_sprite2d)
    {
        return m_sprite2d->texture();
    }
    return nullptr;
}

void sad::db::custom::Object::setTextureName(const sad::String& name) const
{
    PROFILER_EVENT;
    if (m_sprite2d)
    {
        m_sprite2d->setTexureName(name);
    }
}

sad::String sad::db::custom::Object::textureName() const
{
    PROFILER_EVENT;
    if (m_sprite2d)
    {
        return m_sprite2d->textureName();
    }
    return sad::String();
}


bool sad::db::custom::Object::load(const picojson::value& v)
{
    PROFILER_EVENT;
    m_sprite2d->toggleLoadingMode(true);
    bool result = this->sad::SceneNode::load(v);
    m_sprite2d->toggleLoadingMode(false);
    return result;
}

void sad::db::custom::Object::initDefaultSchema()
{
    PROFILER_EVENT;
    m_my_schema->addParent(sad::SceneNode::basicSchema());
    m_my_schema->add(
        "schema", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::String>(
            &sad::db::custom::Object::schemaName,
            &sad::db::custom::Object::setSchemaName
        )
    );
    m_my_schema->add(
        "font", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::String>(
            &sad::db::custom::Object::fontName,
            &sad::db::custom::Object::setFontName
        )
    );
    m_my_schema->add(
        "fontsize", 
        new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
            &sad::db::custom::Object::fontSize,
            &sad::db::custom::Object::setFontSize
        )
    );
    m_my_schema->add(
        "options", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::String>(
            &sad::db::custom::Object::options,
            &sad::db::custom::Object::setOptions
        )
    );
    m_my_schema->add(
        "linespacing", 
        new sad::db::MethodPair<sad::db::custom::Object, float>(
            &sad::db::custom::Object::lineSpacingRatio,
            &sad::db::custom::Object::setLineSpacingRatio
        )
    );
    m_my_schema->add(
        "texturecoordinates", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::Rect2D>(
            &sad::db::custom::Object::textureCoordinates,
            &sad::db::custom::Object::setTextureCoordinates
        )
    );
    m_my_schema->add(
        "angle", 
        new sad::db::MethodPair<sad::db::custom::Object, double>(
            &sad::db::custom::Object::angle,
            &sad::db::custom::Object::setAngle
        )
    );
    m_my_schema->add(
        "area", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::Rect2D>(
            &sad::db::custom::Object::area,
            &sad::db::custom::Object::setArea
        )
    );		
    m_my_schema->add(
        "text", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::String>(
            &sad::db::custom::Object::string,
            &sad::db::custom::Object::setString
        )
    );
    m_my_schema->add(
        "color", 
        new sad::db::MethodPair<sad::db::custom::Object, sad::AColor>(
            &sad::db::custom::Object::color,
            &sad::db::custom::Object::setColor
        )
    );
    m_my_schema->add(
        "flipx", 
        new sad::db::MethodPair<sad::db::custom::Object, bool>(
            &sad::db::custom::Object::flipX,
            &sad::db::custom::Object::setFlipX
        )
    );
    m_my_schema->add(
        "flipy", 
        new sad::db::MethodPair<sad::db::custom::Object, bool>(
            &sad::db::custom::Object::flipY,
            &sad::db::custom::Object::setFlipY
        )
    );

    sad::db::Property* mlw_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::maximalLineWidth,
        &sad::db::custom::Object::setMaximalLineWidth
    );
    mlw_property->makeNonRequiredWithDefaultValue(new sad::db::Variant( static_cast<unsigned int>(0) ));
    m_my_schema->add(
        "maximallinewidth", 
        mlw_property
    );

    sad::db::Property* os_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::overflowStrategyAsIndex,
        &sad::db::custom::Object::setOverflowStrategyFromIndex
    );
    os_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("overflowstrategy", os_property);

    sad::db::Property* bt_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::breakTextAsIndex,
        &sad::db::custom::Object::setBreakTextFromIndex
    );
    bt_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("breaktext", bt_property);

    sad::db::Property* tep_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::textEllipsisAsIndex,
        &sad::db::custom::Object::setTextEllipsisPositionAsIndex
    );
    tep_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("textellipsisposition", tep_property);


    sad::db::Property* mlines_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::maximalLinesCount,
        &sad::db::custom::Object::setMaximalLinesCount
    );
    mlines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("maximallinescount", mlines_property);

    sad::db::Property* oslines_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::overflowStrategyForLinesAsIndex,
        &sad::db::custom::Object::setOverflowStrategyForLinesFromIndex
    );
    oslines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("overflowstrategyforlines", oslines_property);

    sad::db::Property* teplines_property = new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
        &sad::db::custom::Object::textEllipsisForLinesAsIndex,
        &sad::db::custom::Object::setTextEllipsisPositionForLinesAsIndex
    );
    teplines_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(static_cast<unsigned int>(0)));
    m_my_schema->add("textellipsispositionforlines", teplines_property);

    sad::db::Property* hasformatting_property = new sad::db::MethodPair<sad::db::custom::Object, bool>(
        &sad::db::custom::Object::hasFormatting,
        &sad::db::custom::Object::setHasFormatting
        );
    hasformatting_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(false));
    m_my_schema->add("hasformatting", hasformatting_property);

    m_custom_schema->addParent(m_my_schema);
}

void sad::db::custom::Object::updateConfiguration(sad::db::custom::Schema * s)
{
    PROFILER_EVENT;
    if (s)
    {
        sad::Renderer * renderer = sad::Renderer::ref();
        if (m_schema.renderer() != nullptr)
        {
            renderer = m_schema.renderer();
        }
        sad::resource::Tree * tree = renderer->tree(m_schema.treeName());
        sad::resource::Resource* resource = nullptr;
        if (tree)
        {
            resource = tree->resource(s->treeItemName());
        }
        if (resource)
        {
            m_current_rendered_object = nullptr;
            if (resource->metaData()->canBeCastedTo("sad::Font"))
            {
                m_label->setTreeName(m_schema.treeName());
                m_label->setFontName(s->treeItemName());
                m_current_rendered_object = m_label;
                m_sprite2d->setArea(m_label->area());
            }
            if (resource->metaData()->name() == "sad::Sprite2D::Options")
            {
                m_sprite2d->setTreeName(m_schema.treeName());
                m_current_rendered_object = m_sprite2d;
                m_sprite2d->set(s->treeItemName());
            }

            // Update props
            sad::Hash<sad::String, sad::db::Property*> hash;
            sad::Hash<sad::String, sad::db::Property*> properties_to_be_removed = m_custom_schema->ownProperties();
            
            s->getCustomProperties(hash);
            for(sad::Hash<sad::String, sad::db::Property*>::iterator it = hash.begin(); 
                it != hash.end(); 
                ++it)
            {
                // DO NOT INSERT EXISTING PROPS
                if (m_my_schema->getProperty(it.key()) != nullptr)
                {
                    delete it.value();
                }
                else
                {
                    sad::db::Property* my_property = m_custom_schema->getProperty(it.key());
                    if (my_property != nullptr)
                    {
                        properties_to_be_removed.remove(it.key());
                        if (my_property->hasEqualTypeAs(it.value()))
                        {
                            // A fix for https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/64
                            // to resolve updating schema
                            if (my_property->hasDefaultValue() || it.value()->hasDefaultValue())
                            {
                                if (my_property->hasDefaultValue())
                                {                                    
                                    if (it.value()->hasDefaultValue()) // Replace default value
                                    {
                                        sad::db::Variant* v = it.value()->defaultValue();
                                        my_property->makeNonRequiredWithDefaultValue( new sad::db::Variant(*v));
                                    }
                                    else
                                    {
                                        my_property->makeRequired();
                                    }
                                }
                                else
                                {
                                    sad::db::Variant* v = it.value()->defaultValue();
                                    my_property->makeNonRequiredWithDefaultValue(new sad::db::Variant(*v));
                                }
                            }

                            delete it.value();
                        }
                        else
                        {
                            m_custom_schema->remove(it.key());
                            sad::db::Property*  p = it.value();
                            m_custom_schema->add(it.key(), p);
                        }
                    }
                    else
                    {
                        sad::db::Property*  p = it.value();
                        m_custom_schema->add(it.key(), p);
                    }
                }
            }

            for(sad::Hash<sad::String, sad::db::Property*>::iterator it = properties_to_be_removed.begin(); 
                it != properties_to_be_removed.end(); 
                ++it)
            {
                m_custom_schema->remove(it.key());
            }
        }
        else
        {
            m_current_rendered_object = nullptr;
        }
    }
    else
    {
        delete m_custom_schema;
        m_custom_schema = new sad::db::custom::Schema();
        m_custom_schema->addParent(m_my_schema);
        m_current_rendered_object = nullptr;
    }
}
