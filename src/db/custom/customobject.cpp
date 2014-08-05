#include "db/custom/customobject.h"

#include "label.h"
#include "sprite2d.h"

DECLARE_SOBJ_INHERITANCE(sad::db::custom::Object,sad::SceneNode);


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