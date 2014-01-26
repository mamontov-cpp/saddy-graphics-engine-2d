#include "screensprite.h"

#include "../core/fonttemplatesdatabase.h"
#include "../core/spritedatabase.h"

#include <geometry2d.h>
#include <3rdparty/format/format.h>

#include <QString>

DECLARE_SOBJ_INHERITANCE(ScreenSprite, AbstractScreenObject);


class SpritePropertyListener: public PropertyListener<sad::Rect2D>,  public PropertyListener<float>
{
private:
	ScreenSprite * m_sprite;
public:
	inline SpritePropertyListener() {}
	inline void setSprite(ScreenSprite * sprite) { m_sprite = sprite; }

	virtual void notify(const sad::Rect2D & data)
	{
		sad::Sprite2DConfigObserver * o = m_sprite->observer();
		if (o)
		{
			m_sprite->observer()->sprite()->setRenderableArea(data);
		}
	}
	virtual void notify(const float & data)
	{
		sad::Sprite2DConfigObserver * o = m_sprite->observer();
		if (o)
		{
			sad::Sprite2D * c = o->sprite();
			c->rotate(data - c->angle()); 
		}
	}
};

ScreenSprite::ScreenSprite()
{
	m_observer = NULL;
	
	m_rect_listener = new SpritePropertyListener();
	m_rect_listener->setSprite(this);
	
	m_angle_listener = new SpritePropertyListener();
	m_angle_listener->setSprite(this);

	this->addProperty("color",new MappedField<sad::Color>(&m_color, sad::Color(255,255,255)));
	this->addProperty("alpha"  ,new MappedField<int>(&m_alpha, 0));

	this->addProperty("config" ,new MappedField<sad::String>(&m_config, ""));
	this->addProperty("group" ,new MappedField<sad::String>(&m_group, ""));
	this->addProperty("index" ,new MappedField<int>(&m_index, 0));
	
	MappedField<sad::Rect2D> * hr = new MappedField<sad::Rect2D>(&m_rect, sad::Rect2D(sad::Point2D(0,0), sad::Point2D(128,128)));
	hr->setListener(m_rect_listener);
	this->addProperty("rect" , hr);
	MappedField<float> * fr =  new MappedField<float>(&m_angle, 0.0f);
	fr->setListener(m_angle_listener);
	this->addProperty("angle", fr);
}


ScreenSprite::~ScreenSprite()
{
	delete  m_rect_listener;
	delete  m_angle_listener;
	delete m_observer;
}

sad::String ScreenSprite::typeName()
{
	return "ScreenSprite";
}



void ScreenSprite::_render()
{
	if (m_observer)
	{	
		m_observer->sprite()->setColor(
			sad::AColor(m_color.r(), m_color.g(), m_color.b(), (sad::uchar)m_alpha)
		);
		m_observer->sprite()->render();
	}
}


sad::String ScreenSprite::_description()
{
	QString a = QString("sprite(%1,%2,%3) at (%4,%5)");
	sad::Point2D  p = this->region()[0];
	a = a.arg(m_config.data())
		 .arg(m_group.data())
		 .arg(QString::number(m_index))
		 .arg(QString::number(p.x()))
 		 .arg(QString::number(p.y()));
	return a.toStdString().c_str();
}


sad::Rect2D ScreenSprite::region()
{
	sad::Rect2D rd = m_rect;
	sad::rotate(rd, m_angle);
	return  rd;	
}

void ScreenSprite::moveCenterTo(const sad::Point2D & point)
{
	if (m_observer)
	{
		sad::Rect2D  hregion = this->region();
 		sad::Point2D middle = (hregion[0] + hregion[2]) / 2; 
		sad::Point2D delta = point - middle;
		m_observer->sprite()->moveBy(delta);
		sad::moveBy(delta, m_rect);
	}
}





bool ScreenSprite::isWithin(const sad::Point2D & p)
{
	sad::Rect2D r = this->region();
	return sad::isWithin(p, r);
}


bool ScreenSprite::isValid(FontTemplateDatabase * db, sad::Vector<sad::String> * errors)
{

	SpriteDatabase & d = db->sprites();
	bool ok = false;
	if (d.configs().contains(m_config.data()))
	{
		if (d.configs()[m_config.data()].contains(m_group.data()))
		{
			ok = d.configs()[m_config.data()][m_group.data()].contains(m_index);
		}
	}
	if (ok == false && errors != NULL)
	{
		(*errors) << str(fmt::Print("Sprite template (\"{0},{1},{2}\") is absent in database\n") 
					  << m_config.data()
					  << m_group.data()
					  << m_index
				     ).c_str();
	}
	return ok;
}



bool ScreenSprite::tryReload(FontTemplateDatabase * db)
{
	sad::Point2D  point(0,0);
	sad::Point2D  size(0,0);
    //bool changesize = false;
	if (m_observer)
	{
		point = m_observer->sprite()->middle();
	}

	delete m_observer;
	m_observer = NULL;
	if (isValid(db))
	{
		this->reloadNoSize(db);
		this->moveCenterTo(point);
		m_observer->sprite()->rotate(m_angle);
		return true;
	}
	return false;
}

void ScreenSprite::reloadNoSize(FontTemplateDatabase * db)
{
	sad::Sprite2DConfig * cfg = db->sprites().hconfigs()[m_config];
	sad::Sprite2DConfigObserver * obs = new sad::Sprite2DConfigObserver(m_group, m_index, cfg);
	obs->createSprite(sad::Point2D(0,0));
	m_observer = obs;
	m_rect = this->m_observer->sprite()->renderableArea();
}

void ScreenSprite::setRotatedRectangle(const sad::Rect2D & rotatedrectangle, float angle)
{
	float mangle = -1 * angle;
	m_rect = rotatedrectangle;
	sad::rotate(m_rect, mangle);	
	m_angle = angle;
	sad::Sprite2DConfigObserver * o = this->observer();
	if (o)
	{
		this->observer()->sprite()->setRenderableArea(rotatedrectangle);
	}
}

void ScreenSprite::initializeGraphicAfterLoad(FontTemplateDatabase * db)
{
	sad::Sprite2DConfig * cfg = db->sprites().hconfigs()[m_config];
	sad::Sprite2DConfigObserver * obs = new sad::Sprite2DConfigObserver(m_group, m_index, cfg);
	obs->createSprite(sad::Point2D(0,0));
	m_observer = obs;
	obs->sprite()->setRenderableArea(m_rect);
	obs->sprite()->rotate(m_angle);
}
