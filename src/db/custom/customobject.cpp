#include "db/custom/customobject.h"

#include "label.h"
#include "sprite2d.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

DECLARE_SOBJ_INHERITANCE(sad::db::custom::Object,sad::SceneNode);

sad::db::custom::Object::Object()
{
	m_sprite2d = new sad::Sprite2D();
	m_label = new sad::Label();
	m_my_schema = new sad::db::schema::Schema();
	m_custom_schema = new sad::db::schema::Schema();
	m_current_rendered_object = NULL;
	// Make schema renderer-dependent
	m_schema.setTree(sad::Renderer::ref());
	m_schema.add(this, &sad::db::custom::Object::updateConfiguration);
	this->initDefaultSchema();
}

sad::db::custom::Object::~Object()
{
	delete m_sprite2d;
	delete m_label;
	delete m_my_schema;
}

void sad::db::custom::Object::setScene(sad::Scene * scene)
{
	this->sad::SceneNode::setScene(scene);
	m_label->setScene(scene);
	m_sprite2d->setScene(scene);
}

bool sad::db::custom::Object::innerTypeIs(const sad::String &type)
{
	bool result = false;
	if (m_current_rendered_object) 
	{
		result = m_current_rendered_object->metaData()->canBeCastedTo(type);
	}
	return result;
}
void sad::db::custom::Object::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
	m_schema.setTree(r, tree_name);
	m_sprite2d->setTreeName(r, tree_name);
	m_label->setTreeName(r, tree_name);
}

sad::db::schema::Schema* sad::db::custom::Object::schema() const
{
	return m_custom_schema;
}

void sad::db::custom::Object::regions(sad::Vector<sad::Rect2D> & r)
{
	r << this->renderableArea();
}

sad::Rect2D sad::db::custom::Object::renderableArea() const
{
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
	if (m_current_rendered_object)
	{
		m_current_rendered_object->render();
	}
}

void sad::db::custom::Object::setSchemaName(const sad::String & s)
{
	m_schema.setPath(s);
	updateConfiguration(m_schema.get());
}

const sad::String& sad::db::custom::Object::schemaName() const
{
	return m_schema.path();
}


unsigned int sad::db::custom::Object::fontSize() const
{
	return m_label->size();
}

void sad::db::custom::Object::setFontSize(unsigned int size)
{
	m_label->setSize(size);
}

const sad::String& sad::db::custom::Object::fontName() const
{
	return m_label->fontName();
}

void sad::db::custom::Object::setFontName(const sad::String& name)
{
	m_label->setFontName(name);
}

float sad::db::custom::Object::lineSpacing() const
{
	return m_label->lineSpacing();
}

void sad::db::custom::Object::setLineSpacing(float s)
{
	m_label->setLineSpacing(s);	
}

double sad::db::custom::Object::angle() const
{
	return m_label->angle();	
}

void sad::db::custom::Object::setAngle(double a)
{
	m_label->setAngle(a);
	m_sprite2d->setAngle(a);
}


sad::Rect2D sad::db::custom::Object::area() const
{
	if (m_current_rendered_object == m_label)
	{
		return m_label->area();
	}
	return m_sprite2d->area();	
}


void sad::db::custom::Object::setArea(const sad::Rect2D & area)
{
	m_label->setArea(area);
	m_sprite2d->setArea(area);
}

const sad::String& sad::db::custom::Object::string() const
{
	return m_label->string();
}

void sad::db::custom::Object::setString(const sad::String & s)
{
	m_label->setString(s);
}

const sad::AColor& sad::db::custom::Object::color() const
{
	return m_label->color();
}

void sad::db::custom::Object::setColor(const sad::AColor & c)
{
	m_label->setColor(c);
	m_sprite2d->setColor(c);
}

bool sad::db::custom::Object::flipX() const
{
	return m_sprite2d->flipX();
}

void sad::db::custom::Object::setFlipX(bool f)
{
	m_sprite2d->setFlipX(f);
}

bool sad::db::custom::Object::flipY() const
{
	return m_sprite2d->flipY();
}

void sad::db::custom::Object::setFlipY(bool f)
{
	m_sprite2d->setFlipY(f);
}

bool sad::db::custom::Object::canBeRendered() const
{
	return m_current_rendered_object != NULL;
}

static sad::Hash<sad::String, sad::db::Property*>  SadDbCustomObjectEmptyProperties;

const sad::Hash<sad::String, sad::db::Property*>&  sad::db::custom::Object::schemaProperties() const
{
	if (!m_custom_schema)
	{
		return SadDbCustomObjectEmptyProperties;
	}
	return m_custom_schema->ownProperties();
}

void sad::db::custom::Object::setOptions(const sad::String& o)
{
	m_sprite2d->set(o);
}

const sad::String& sad::db::custom::Object::options() const
{
	return m_sprite2d->optionsName();
}

void sad::db::custom::Object::setTextureCoordinates(const sad::Rect2D & texturecoordinates)
{
	m_sprite2d->setTextureCoordinates(texturecoordinates);
}

const sad::Rect2D & sad::db::custom::Object::textureCoordinates() const
{
	return  m_sprite2d->textureCoordinates();
}


bool sad::db::custom::Object::load(const picojson::value& v)
{
	m_sprite2d->toggleLoadingMode();
	return this->sad::SceneNode::load(v);
}

void sad::db::custom::Object::initDefaultSchema()
{
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
			&sad::db::custom::Object::lineSpacing,
			&sad::db::custom::Object::setLineSpacing
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

	m_custom_schema->addParent(m_my_schema);
}

void sad::db::custom::Object::updateConfiguration(sad::db::custom::Schema * s)
{
	if (s)
	{
		sad::Renderer * renderer = sad::Renderer::ref();
		if (m_schema.renderer() != NULL)
		{
			renderer = m_schema.renderer();
		}
		sad::resource::Tree * tree = renderer->tree(m_schema.treeName());
		sad::resource::Resource* resource = NULL;
		if (tree)
		{
			resource = tree->root()->resource(s->treeItemName());
		}
		if (resource)
		{
			m_current_rendered_object = NULL;
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
			sad::Hash<sad::String, sad::db::Property*> propstoberemoved = m_custom_schema->ownProperties();
			
			s->getCustomProperties(hash);
			for(sad::Hash<sad::String, sad::db::Property*>::iterator it = hash.begin(); 
				it != hash.end(); 
				++it)
			{
				// DO NOT INSERT EXISTING PROPS
				if (m_my_schema->getProperty(it.key()) != NULL)
				{
					delete it.value();
				}
				else
				{
					sad::db::Property* myprop = m_custom_schema->getProperty(it.key());
					if (myprop != NULL)
					{
						propstoberemoved.remove(it.key());
						if (myprop->hasEqualTypeAs(it.value()))
						{
                            // A fix for https://github.com/mamontov-cpp/saddy-graphics-engine-2d/issues/64
                            // to resolve updating schema
                            if (myprop->hasDefaultValue() || it.value()->hasDefaultValue())
                            {
                                if (myprop->hasDefaultValue())
                                {                                    
                                    if (it.value()->hasDefaultValue()) // Replace default value
                                    {
                                        sad::db::Variant* v = it.value()->defaultValue();
                                        myprop->makeNonRequiredWithDefaultValue( new sad::db::Variant(*v));
                                    }
                                    else
                                    {
                                        myprop->makeRequired();
                                    }
                                }
                                else
                                {
                                    sad::db::Variant* v = it.value()->defaultValue();
                                    myprop->makeNonRequiredWithDefaultValue(new sad::db::Variant(*v));
                                }
                            }

							delete it.value();
						}
						else
						{
							m_custom_schema->remove(it.key());
							m_custom_schema->add(it.key(), it.value());
						}
					}
					else
					{
						m_custom_schema->add(it.key(), it.value());
					}
				}
			}

			for(sad::Hash<sad::String, sad::db::Property*>::iterator it = propstoberemoved.begin(); 
				it != propstoberemoved.end(); 
				++it)
			{
				m_custom_schema->remove(it.key());
			}
		}
		else
		{
			m_current_rendered_object = NULL;
		}
	}
	else
	{
		delete m_custom_schema;
		m_custom_schema = new sad::db::custom::Schema();
		m_custom_schema->addParent(m_my_schema);
		m_current_rendered_object = NULL;
	}
}
