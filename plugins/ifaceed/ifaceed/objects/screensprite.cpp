#include "screensprite.h"
#include <QString>


ScreenSprite::ScreenSprite()
{
	m_observer = NULL;
}

hst::string ScreenSprite::type()
{
	return "ScreenSprite";
}


void ScreenSprite::_render()
{
	if (m_observer)
	{

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