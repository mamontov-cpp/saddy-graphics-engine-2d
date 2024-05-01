#include "way.h"

#include <geometry2d.h>

#include "db/schema/schema.h"
#include "opticksupport.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"

// ========================================= sad::Way METHODS =========================================

sad::Way::Way() : m_constructed(true), m_closed(false), m_total_time(100)
{
    PROFILER_EVENT;

}

sad::Way::~Way() = default;

sad::Point2D sad::Way::getPointInTime(double current_time, double step)
{
    PROFILER_EVENT;
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
            index = static_cast<int>(i);
        }
    }
    const double time_span = m_times[index+1] - m_times[index];
    const double relative_time = time - m_times[index];
    sad::WayPoint p1 = m_way_points[index]; 
    sad::WayPoint p2;
    if (static_cast<unsigned int>(index + 1) == m_way_points.size())
    {
        p2 = m_way_points[0];
    }
    else
    {
        p2 = m_way_points[index + 1];
    }
    p1 += (p2 - p1) * (relative_time / time_span);
    return p1;
}


void sad::Way::setPoint(int i,  const sad::WayPoint & p)
{
    PROFILER_EVENT;
    m_way_points[i] = p;
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::addPoint(const sad::WayPoint & p)
{
    PROFILER_EVENT;
    m_way_points << p;
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::insertPoint(int i, const sad::WayPoint& p)
{
    PROFILER_EVENT;
    m_way_points.insert(p, i);
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::removePoint(int i)
{
    PROFILER_EVENT;
    m_way_points.removeAt(i);
    if (m_constructed)
    {
        construct();
    }
}

bool sad::Way::closed() const
{
    PROFILER_EVENT;
    return m_closed;
}

void sad::Way::setClosed(bool b)
{
    PROFILER_EVENT;
    m_closed = b;
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::makeClosed()
{
    PROFILER_EVENT;
    m_closed = true;
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::makeOpen()
{
    PROFILER_EVENT;
    m_closed = false;
    if (m_constructed)
    {
        construct();
    }
}

void sad::Way::setTotalTime(double time)
{
    PROFILER_EVENT;
    m_total_time = time;
    if (m_constructed)
    {
        construct();
    }
}

double sad::Way::totalTime() const
{
    PROFILER_EVENT;
    return m_total_time;
}

void sad::Way::startConstruction()
{
    PROFILER_EVENT;
    m_constructed = false;
}


void sad::Way::construct()
{
    PROFILER_EVENT;
    m_constructed = true;
    if (m_way_points.size() <= 1 || sad::is_fuzzy_zero(m_total_time))
    {
        return;
    }
    double current_time = 0;
    double total_distance = 0;
    for(unsigned int i = 0; i < m_way_points.size() - 1; i++)
    {
        total_distance += m_way_points[i].distance(m_way_points[i+1]);
    }
    if (m_closed)
    {
        total_distance +=  m_way_points[0].distance(m_way_points[m_way_points.size()-1]);
    }

    const double average_speed = total_distance / m_total_time;
    m_times.clear();
    m_times << 0;
    for(unsigned int i = 1; i < m_way_points.size(); i++)
    {
        const double d = m_way_points[i-1].distance(m_way_points[i]);
        current_time += d / average_speed;
        m_times << current_time;
    }
    if (m_closed)
    {
        m_times << m_total_time;
    }
}

static sad::String SadWayName = "sad::Way";  // NOLINT(clang-diagnostic-exit-time-destructors)

const sad::String&  sad::Way::serializableName() const
{
    PROFILER_EVENT;
    return SadWayName;
}

static sad::db::schema::Schema* SadWaySchema = nullptr;

sad::db::schema::Schema* sad::Way::basicSchema()
{
    PROFILER_EVENT;
    if (SadWaySchema == nullptr)
    {
        SadWaySchema = new sad::db::schema::Schema();
        SadWaySchema->addParent(sad::db::Object::basicSchema());

        SadWaySchema->add(
            "totaltime", 
            new sad::db::MethodPair<sad::Way, double>(
                &sad::Way::totalTime,
                &sad::Way::setTotalTime
            )
        );
        SadWaySchema->add(
            "closed", 
            new sad::db::MethodPair<sad::Way, bool>(
                &sad::Way::closed,
                &sad::Way::setClosed
            )
        );
        SadWaySchema->add(
            "waypoints", 
            define_field(&sad::Way::m_way_points)
        );

        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(SadWaySchema);
    }
    return SadWaySchema;
}

sad::db::schema::Schema* sad::Way::schema() const
{
    PROFILER_EVENT;
    return sad::Way::basicSchema();
}

bool sad::Way::load(const picojson::value& v)
{
    PROFILER_EVENT;
    const bool result = this->sad::db::Object::load(v);
    if (result)
    {
        construct();
    }
    return result;
}
