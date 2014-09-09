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

void sad::db::custom::Object::setTreeName(sad::Renderer* r, const sad::String & tree_name)
{
	m_schema.setTree(r, tree_name);
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
		"fontsize", 
		new sad::db::MethodPair<sad::db::custom::Object, unsigned int>(
			&sad::db::custom::Object::fontSize,
			&sad::db::custom::Object::setFontSize
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
