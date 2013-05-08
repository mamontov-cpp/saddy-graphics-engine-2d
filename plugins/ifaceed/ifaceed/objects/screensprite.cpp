#include "screensprite.h"
#include <QString>
#include <extra/geometry2d.h>
#include <3rdparty/format/format.h>

#include "../core/fonttemplatesdatabase.h"
#include "../core/spritedatabase.h"

class SpritePropertyListener: public PropertyListener<hRectF>,  public PropertyListener<float>
{
private:
	ScreenSprite * m_sprite;
public:
	inline SpritePropertyListener() {}
	inline void setSprite(ScreenSprite * sprite) { m_sprite = sprite; }

	virtual void notify(const hRectF & data)
	{
		Sprite2DConfigObserver * o = m_sprite->observer();
		if (o)
		{
			m_sprite->observer()->sprite()->adapter()->setRect(data);
		}
	}
	virtual void notify(const float & data)
	{
		Sprite2DConfigObserver * o = m_sprite->observer();
		if (o)
		{
			Sprite2DController * c = o->sprite();
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

	this->addProperty("color",new MappedField<hst::color>(&m_color, hst::color(255,255,255)));
	this->addProperty("alpha"  ,new MappedField<int>(&m_alpha, 0));

	this->addProperty("config" ,new MappedField<hst::string>(&m_config, ""));
	this->addProperty("group" ,new MappedField<hst::string>(&m_group, ""));
	this->addProperty("index" ,new MappedField<int>(&m_index, 0));
	
	MappedField<hRectF> * hr = new MappedField<hRectF>(&m_rect, hRectF(hPointF(0,0), hPointF(128,128)));
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

hst::string ScreenSprite::typeName()
{
	return "ScreenSprite";
}



void ScreenSprite::_render()
{
	if (m_observer)
	{	
		m_observer->sprite()->setColor(
			hst::acolor(m_color.r(), m_color.g(), m_color.b(), (Uint8)m_alpha)
		);
		m_observer->sprite()->adapter()->render();
	}
}


hst::string ScreenSprite::_description()
{
	QString a = QString("sprite(%1,%2,%3) at (%4,%5)");
	hPointF  p = this->region()[0];
	a = a.arg(m_config.data())
		 .arg(m_group.data())
		 .arg(QString::number(m_index))
		 .arg(QString::number(p.x()))
 		 .arg(QString::number(p.y()));
	return a.toStdString().c_str();
}


hRectF ScreenSprite::region()
{
	hRectF rd = m_rect;
	rotate(m_angle, rd);
	return  rd;	
}

void ScreenSprite::moveCenterTo(const hPointF & point)
{
	if (m_observer)
	{
		hRectF  hregion = this->region();
 		hPointF middle = (hregion[0] + hregion[2]) / 2; 
		hPointF delta = point - middle;
		m_observer->sprite()->move(delta);
		moveBy(delta, m_rect);
	}
}





bool ScreenSprite::isWithin(const hPointF & p)
{
	hRectF r = this->region();
	return ::isWithin(p, r);
}


bool ScreenSprite::isValid(FontTemplateDatabase * db, hst::vector<hst::string> * errors)
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
	hPointF  point(0,0);
	hPointF  size(0,0);
	bool changesize = false;
	if (m_observer)
	{
		point = m_observer->sprite()->pos();
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
	Sprite2DConfig * cfg = db->sprites().hconfigs()[m_config];
	Sprite2DConfigObserver * obs = new Sprite2DConfigObserver(m_group, m_index, cfg);
	obs->createSprite(hPointF(0,0));
	m_observer = obs;
	m_rect = this->m_observer->sprite()->adapter()->rect();
}

void ScreenSprite::setRotatedRectangle(const hRectF & rotatedrectangle, float angle)
{
	float mangle = -1 * angle;
	m_rect = rotatedrectangle;
	rotate(mangle, m_rect);	
	m_angle = angle;
	Sprite2DConfigObserver * o = this->observer();
	if (o)
	{
		this->observer()->sprite()->adapter()->setRect(rotatedrectangle);
	}
}

