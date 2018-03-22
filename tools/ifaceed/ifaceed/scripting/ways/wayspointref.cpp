#include "wayspointref.h"

#include <geometry2d.h>

#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/ways/wayswaypointchange.h"
#include "../../history/ways/wayswaypointswap.h"


scripting::ways::PointRef::PointRef() : m_way(NULL), m_pos(0)
{
    
}

scripting::ways::PointRef::PointRef(scripting::Scripting* s, sad::p2d::app::Way* way, unsigned int pos)
: m_scripting(s), m_way(way), m_pos(pos)
{
    
}

scripting::ways::PointRef::~PointRef()
{
    
}

bool scripting::ways::PointRef::valid() const
{
    if (!m_way)
    {
        m_scripting->context()->throwError("Invalid way for point reference");
        return false;
    }
    if (!m_way->Active)
    {
        m_scripting->context()->throwError("Invalid way for point reference");
        return false;
    }
    if (m_pos >= m_way->wayPoints().count())
    {
        m_scripting->context()->throwError("Invalid position of point");
        return false;
    }

    return true;
}

static sad::Point2D defaultpoint;

const sad::Point2D& scripting::ways::PointRef::toPoint() const
{
    if (!valid())
    {
        return defaultpoint;
    }
    return m_way->wayPoints()[m_pos];
}

double scripting::ways::PointRef::x() const
{
    if (!valid())
    {
        return 0;
    }
    return m_way->wayPoints()[m_pos].x();
}

double scripting::ways::PointRef::y() const
{
    if (!valid())
    {
        return 0;
    }
    return m_way->wayPoints()[m_pos].y();
}

void  scripting::ways::PointRef::setX(double x)
{
    if (!valid())
    {
        return ;
    }
    scripting::Scripting* e = m_scripting;
    core::Editor* editor = e->editor();
    if (sad::is_fuzzy_equal(x, this->x()) == false)
    {
        sad::Point2D oldpoint = this->toPoint();
        sad::Point2D newpoint = oldpoint;
        newpoint.setX(x);
        history::Command* c = new history::ways::WayPointChange(m_way, m_pos, oldpoint, newpoint);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
    }
}

void scripting::ways::PointRef::setY(double y)
{
    if (!valid())
    {
        return ;
    }
    scripting::Scripting* e = m_scripting;
    core::Editor* editor = e->editor();
    if (sad::is_fuzzy_equal(y, this->y()) == false)
    {
        sad::Point2D oldpoint = this->toPoint();
        sad::Point2D newpoint = oldpoint;
        newpoint.setY(y);
        history::Command* c = new history::ways::WayPointChange(m_way, m_pos, oldpoint, newpoint);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
    }
}


unsigned int scripting::ways::PointRef::position() const
{
    return m_pos;
}

QString scripting::ways::PointRef::toString() const
{
    if (!valid())
    {
        return "PointRef(<invalid>)";
    }
    QString result = QString("PointRef(way : %1, pos: %2, x : %3, y : %4)")
        .arg(m_way->MajorId)
        .arg(m_pos)
        .arg(this->x())
        .arg(this->y());
    return result;
}

void scripting::ways::PointRef::moveBack()
{
    if (!valid())
    {
        return ;
    }
    if (m_pos > 0)
    {
        scripting::Scripting* e = m_scripting;
        core::Editor* editor = e->editor();

        history::Command* c = new history::ways::WayPointSwap(m_way, m_pos - 1, m_pos);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
        
        m_pos--;
    }
}

void scripting::ways::PointRef::moveFront()
{
    if (!valid())
    {
        return ;
    }
    if (m_pos < m_way->wayPoints().count() - 1)
    {
        scripting::Scripting* e = m_scripting;
        core::Editor* editor = e->editor();

        history::Command* c = new history::ways::WayPointSwap(m_way, m_pos, m_pos + 1);
        c->commit(editor);
        editor->currentBatchCommand()->add(c);
        
        m_pos++;
    }
}
