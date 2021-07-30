#include "p2d/app/way.h"

#include <geometry2d.h>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

// ========================================= sad::p2d::app::Way METHODS =========================================

sad::p2d::app::Way::Way() : m_constructed(true), m_closed(false), m_total_time(100)
{

}

sad::p2d::app::Way::~Way()
{

}


sad::p2d::Point sad::p2d::app::Way::getPointInTime(double current_time, double step)
{
    if (!m_constructed)
    {
        construct();
    }

    if (m_way_points.size() == 1)
    {
        return m_way_points[0];
    }

    double time = current_time + step;
    if (time > m_total_time  && !m_closed)
    {
        return m_way_points[m_way_points.size() - 1];
    }

    while(time > m_total_time)
        time -= m_total_time;

    int index = -1;
    for(unsigned int i = 0; (i < m_times.size() - 1) && index == -1; i++)
    {
        if (time >= m_times[i] && time <= m_times[i+1])
        {
            index = i;
        }
    }
    double timespan = m_times[index+1] - m_times[index];
    double relativetime = time - m_times[index];
    sad::p2d::Point p1 = m_way_points[index]; 
    sad::p2d::Point p2;
    if (index + 1 == m_way_points.size())
    {
        p2 = m_way_points[0];
    }
    else
    {
        p2 = m_way_points[index + 1];
    }
    p1 += (p2 - p1) * (relativetime / timespan);
    return p1;
}


void sad::p2d::app::Way::setPoint(int i,  const sad::p2d::app::WayPoint & p)
{
    m_way_points[i] = p;
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::addPoint(const sad::p2d::app::WayPoint & p)
{
    m_way_points << p;
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::insertPoint(int i, const sad::p2d::app::WayPoint& p)
{
    m_way_points.insert(p, i);
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::removePoint(int i)
{
    m_way_points.removeAt(i);
    if (m_constructed)
    {
        construct();
    }
}

bool sad::p2d::app::Way::closed() const
{
    return m_closed;
}

void sad::p2d::app::Way::setClosed(bool b)
{
    m_closed = b;
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::makeClosed()
{
    m_closed = true;
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::makeOpen()
{
    m_closed = false;
    if (m_constructed)
    {
        construct();
    }
}

void sad::p2d::app::Way::setTotalTime(double time)
{
    m_total_time = time;
    if (m_constructed)
    {
        construct();
    }
}

double sad::p2d::app::Way::totalTime() const
{
    return m_total_time;
}

void sad::p2d::app::Way::startConstruction()
{
    m_constructed = false;
}


void sad::p2d::app::Way::construct()
{
    m_constructed = true;
    if (m_way_points.size() <= 1 || sad::is_fuzzy_zero(m_total_time))
    {
        return;
    }
    double curtime = 0;
    double totaldistance = 0;
    for(unsigned int i = 0; i < m_way_points.size() - 1; i++)
    {
        totaldistance += m_way_points[i].distance(m_way_points[i+1]);
    }
    if (m_closed)
    {
        totaldistance +=  m_way_points[0].distance(m_way_points[m_way_points.size()-1]);
    }

    double avspeed = totaldistance / m_total_time;
    m_times.clear();
    m_times << 0;
    for(unsigned int i = 1; i < m_way_points.size(); i++)
    {
        double d = m_way_points[i-1].distance(m_way_points[i]);
        curtime += d / avspeed;
        m_times << curtime;
    }
    if (m_closed)
    {
        m_times << m_total_time;
    }
}

static sad::String SadP2DAppWayName = "sad::p2d::app::Way";

const sad::String&  sad::p2d::app::Way::serializableName() const
{
    return SadP2DAppWayName;
}

static sad::db::schema::Schema* SadP2DAppWaySchema = nullptr;

sad::db::schema::Schema* sad::p2d::app::Way::basicSchema()
{
    if (SadP2DAppWaySchema == nullptr)
    {
        SadP2DAppWaySchema = new sad::db::schema::Schema();
        SadP2DAppWaySchema->addParent(sad::db::Object::basicSchema());

        SadP2DAppWaySchema->add(
            "totaltime", 
            new sad::db::MethodPair<sad::p2d::app::Way, double>(
                &sad::p2d::app::Way::totalTime,
                &sad::p2d::app::Way::setTotalTime
            )
        );
        SadP2DAppWaySchema->add(
            "closed", 
            new sad::db::MethodPair<sad::p2d::app::Way, bool>(
                &sad::p2d::app::Way::closed,
                &sad::p2d::app::Way::setClosed
            )
        );
        SadP2DAppWaySchema->add(
            "waypoints", 
            define_field(&sad::p2d::app::Way::m_way_points)
        );

        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(SadP2DAppWaySchema);
    }
    return SadP2DAppWaySchema;
}

sad::db::schema::Schema* sad::p2d::app::Way::schema() const
{
    return sad::p2d::app::Way::basicSchema();
}

bool sad::p2d::app::Way::load(const picojson::value& v)
{
    bool result = this->sad::db::Object::load(v);
    if (result)
    {
        construct();
    }
    return result;
}
