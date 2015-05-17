#include "rect2i.h"

#include <QScriptContext>
#include <QScriptEngine>


scripting::Rect2I::Rect2I()
{
	
}

scripting::Rect2I::Rect2I(const sad::Rect2I& p) : m_rect(p)
{
	
}

scripting::Rect2I::~Rect2I()
{
	
}

const sad::Rect2I& scripting::Rect2I::toRect() const
{
	return m_rect;	
}

QString scripting::Rect2I::toString() const
{
	return QString("sad::Rect2I(sad::Point2I(%1, %2), sad::Point2I(%3, %4), sad::Point2I(%5, %6), sad::Point2I(%7, %8))")
		   .arg(m_rect[0].x())
		   .arg(m_rect[0].y())
		   .arg(m_rect[1].x())
		   .arg(m_rect[1].y())
		   .arg(m_rect[2].x())
		   .arg(m_rect[2].y())
		   .arg(m_rect[3].x())
		   .arg(m_rect[3].y());
}

void scripting::Rect2I::setPoint(int i, scripting::Point2I* p)
{
	if (i >= 0 && i < 4)
	{
		m_rect[i] = p->toPoint();
	}
}

QScriptValue scripting::Rect2I::point(int i) const
{
	if (i >= 0 && i < 4)
	{
		return context()->engine()->newQObject(new scripting::Point2I(m_rect[i]), QScriptEngine::AutoOwnership);
	}
	return QScriptValue();
}
