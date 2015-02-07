#include "wayspointref.h"

#include <geometry2d.h>

#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/ways/wayswaypointchange.h"
#include "../../history/ways/wayswaypointswap.h"


scripting::ways::PointRef::PointRef() : m_way(NULL), m_pos(0)
{
	
}

scripting::ways::PointRef::PointRef(sad::p2d::app::Way* way, unsigned int pos)
: m_way(way), m_pos(pos)
{
	
}

scripting::ways::PointRef::~PointRef()
{
	
}

const sad::Point2D& scripting::ways::PointRef::toPoint() const
{
	return m_way->wayPoints()[m_pos];
}

double scripting::ways::PointRef::x() const
{
	return m_way->wayPoints()[m_pos].x();
}

double scripting::ways::PointRef::y() const
{
	return m_way->wayPoints()[m_pos].y();
}

void  scripting::ways::PointRef::setX(double x)
{
	scripting::Scripting* e = static_cast<scripting::Scripting*>(this->engine()->globalObject().property("E").toQObject());
	core::Editor* editor = e->panel()->editor();
	if (sad::is_fuzzy_equal(x, this->x()) == false)
	{
		sad::Point2D oldpoint = this->toPoint();
		sad::Point2D newpoint = oldpoint;
		newpoint.setX(x);
		history::Command* c = new history::ways::WayPointChange(m_way, m_pos, oldpoint, newpoint);
		c->commit();
		editor->currentBatchCommand()->add(c);
	}
}

void scripting::ways::PointRef::setY(double y)
{
	scripting::Scripting* e = static_cast<scripting::Scripting*>(this->engine()->globalObject().property("E").toQObject());
	core::Editor* editor = e->panel()->editor();
	if (sad::is_fuzzy_equal(y, this->y()) == false)
	{
		sad::Point2D oldpoint = this->toPoint();
		sad::Point2D newpoint = oldpoint;
		newpoint.setY(y);
		history::Command* c = new history::ways::WayPointChange(m_way, m_pos, oldpoint, newpoint);
		c->commit();
		editor->currentBatchCommand()->add(c);
	}
}

QString scripting::ways::PointRef::toString() const
{
	sad::Point2D p = this->toPoint();
	QString result = QString("PointRef(way : %1, pos: %2, x : %3, y : %4)")
					 .arg(m_way->MajorId)
					 .arg(m_pos)
					 .arg(p.x())
					 .arg(p.y());
	return result;
}

unsigned int scripting::ways::PointRef::position() const
{
	return m_pos;
}

void scripting::ways::PointRef::moveBack()
{
	if (m_pos > 0)
	{
		scripting::Scripting* e = static_cast<scripting::Scripting*>(this->engine()->globalObject().property("E").toQObject());
		core::Editor* editor = e->panel()->editor();

		history::Command* c = new history::ways::WayPointSwap(m_way, m_pos - 1, m_pos);
		c->commit();
		editor->currentBatchCommand()->add(c);
		
		m_pos--;
	}
}

void scripting::ways::PointRef::moveFront()
{
	if (m_pos >= 0 && m_pos < m_way->wayPoints().count() - 1)
	{
		scripting::Scripting* e = static_cast<scripting::Scripting*>(this->engine()->globalObject().property("E").toQObject());
		core::Editor* editor = e->panel()->editor();

		history::Command* c = new history::ways::WayPointSwap(m_way, m_pos, m_pos + 1);
		c->commit();
		editor->currentBatchCommand()->add(c);
		
		m_pos++;
	}
}
