#include "screensprite.h"
#include <QString>
#include "../core/fonttemplatesdatabase.h"
#include "../core/spritedatabase.h"

ScreenSprite::ScreenSprite()
{
	m_observer = NULL;

	this->addProperty("config" ,new MappedField<hst::string>(&m_config, ""));
	this->addProperty("group" ,new MappedField<hst::string>(&m_group, ""));
	this->addProperty("index" ,new MappedField<int>(&m_index, 0));

	this->addProperty("rect" ,new MappedField<hRectF>(&m_rect, hRectF(hPointF(0,0), hPointF(128,128))));
	this->addProperty("angle",new MappedField<float>(&m_angle, 0.0f));
}

hst::string ScreenSprite::type()
{
	return "ScreenSprite";
}


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

hRectF ScreenSprite::region()
{
	return m_rect;
}

void ScreenSprite::moveCenterTo(const hPointF & point)
{
	if (m_observer)
	{
		m_observer->sprite()->move(point - m_observer->sprite()->pos());
		m_rect = this->m_observer->sprite()->adapter()->rect();
	}
}


ScreenSprite::~ScreenSprite()
{
	delete m_observer;
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


inline float dist2(const hPointF & p1, const hPointF & p2)
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
	else
	{
		changesize = true;
		point = (m_rect[0] + m_rect[1])/2;
		size.setX(dist2(m_rect[0], m_rect[1]));
		size.setY(dist2(m_rect[0], m_rect[3]));
	}

	delete m_observer;
	m_observer = NULL;
	if (isValid(db))
	{
		this->reloadNoSize(db);
		m_observer->sprite()->move(point - m_observer->sprite()->pos()); 
		if (changesize)
		{
			m_observer->sprite()->setSize(size, false);
		}
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