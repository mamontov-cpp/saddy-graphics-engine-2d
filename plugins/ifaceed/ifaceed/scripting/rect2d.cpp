#include "rect2d.h"

#include <QScriptContext>
#include <QScriptEngine>


scripting::Rect2D::Rect2D()
{
	
}

scripting::Rect2D::Rect2D(const sad::Rect2D& p) : m_rect(p)
{
	
}

scripting::Rect2D::~Rect2D()
{
	
}

const sad::Rect2D& scripting::Rect2D::toRect() const
{
	return m_rect;	
}

QString scripting::Rect2D::toString() const
{
	return QString("sad::Rect2D(sad::Point2D(%1, %2), sad::Point2D(%3, %4), sad::Point2D(%5, %6), sad::Point2D(%7, %8))")
		   .arg(m_rect[0].x())
		   .arg(m_rect[0].y())
		   .arg(m_rect[1].x())
		   .arg(m_rect[1].y())
		   .arg(m_rect[2].x())
		   .arg(m_rect[2].y())
		   .arg(m_rect[3].x())
		   .arg(m_rect[3].y());
}

void scripting::Rect2D::setPoint(int i, scripting::Point2D* p)
{
	if (i >= 0 && i < 4)
	{
		m_rect[i] = p->toPoint();
	}
}

QScriptValue scripting::Rect2D::point(int i) const
{
	if (i >= 0 && i < 4)
	{
		return context()->engine()->newQObject(new scripting::Point2D(m_rect[i]), QScriptEngine::AutoOwnership);
	}
	return QScriptValue();
}
