#include "screensprite.h"
#include <QString>
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


#define PRECISION 0.001


static inline bool isPointsEqual(const hPointF & p1, const hPointF & p2)
{
	return fabs(p1.x() - p2.x()) < PRECISION && fabs(p1.y() - p2.y());
}

static inline bool isRectsEqual(const hRectF & p1, const hRectF & p2)
{
	bool ok = true;
	for(int i = 0; i < 4; i++)
	{
		ok = ok && isPointsEqual(p1[i], p2[i]);
	}
	return ok;
}

#undef PRECISION

void ScreenSprite::_render()
{
	if (m_observer)
	{	
		m_observer->sprite()->adapter()->render();
	}
}


hst::string ScreenSprite::description()
{
	QString a = QString("label(%1,%2,%3) at (%4,%5)");
	hPointF  p = this->region()[0];
	a = a.arg(m_config.data())
		 .arg(m_group.data())
		 .arg(QString::number(m_index))
		 .arg(QString::number(p.x()))
 		 .arg(QString::number(p.y()));
	return a.toStdString().c_str();
}

hRectF rotated(const hRectF & m_rect, float m_angle)
{
	hRectF result = m_rect;
	hPointF tmp, tmp2;
	hPointF middle = (m_rect[0] + m_rect[2])/2;
	for(int i = 0; i < 4; i++)
	{
		tmp = result[i] - middle;
		tmp2.setX(tmp.x()*cos(m_angle) - tmp.y() * sin(m_angle));
		tmp2.setY(tmp.x()*sin(m_angle) + tmp.y() * cos(m_angle));
		result[i] = middle + tmp2;
	}
	return result;
}

hRectF ScreenSprite::region()
{
	return rotated(m_rect, m_angle);	
}

void ScreenSprite::moveCenterTo(const hPointF & point)
{
	if (m_observer)
	{
		m_observer->sprite()->move(point - m_observer->sprite()->pos());
		m_rect = this->m_observer->sprite()->adapter()->rect();
	}
}




bool testIsWithin(const hPointF & p, const hRectF & r);

bool ScreenSprite::isWithin(const hPointF & p)
{
	hRectF r = this->region();
	return testIsWithin(p, r);
}


bool ScreenSprite::isValid(FontTemplateDatabase * db)
{

	SpriteDatabase & d = db->sprites();
	bool ok = true;
	if (d.configs().contains(m_config.data()))
	{
		if (d.configs()[m_config.data()].contains(m_group.data()))
		{
			ok = d.configs()[m_config.data()][m_group.data()].contains(m_index);
		}
	}
	return ok;
}


float dist2(const hPointF & p1, const hPointF & p2)
{
	float dx = p1.x() - p2.x();
	float dy = p1.y() - p2.y();
	return sqrtf(dx*dx + dy*dy);
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
		m_observer->sprite()->move(point - m_observer->sprite()->pos()); 
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
}

void ScreenSprite::setRotatedRectangle(const hRectF & rotatedrectangle, float angle)
{
	float mangle = -1 * angle;
	m_rect = rotated(rotatedrectangle, mangle);
	m_angle = angle;
	Sprite2DConfigObserver * o = this->observer();
	if (o)
	{
		this->observer()->sprite()->adapter()->setRect(rotatedrectangle);
	}
}

