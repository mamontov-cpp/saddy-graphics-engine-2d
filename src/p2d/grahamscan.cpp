#include <p2d/grahamscan.h>

#include <geometry2d.h>
#include <sadpair.h>

#include <algorithm>
#include "opticksupport.h"

namespace sad
{

namespace p2d
{

void erase_equal_points(sad::Vector<sad::p2d::Point> & set)
{
    PROFILER_EVENT;
    for(unsigned int i = 1; i < set.size(); i++)
    {
        if (sad::equal(set[i], set[i-1]))
        {
            set.removeAt(i);
            --i;
        }
    }
}

int find_min_point_on_y_axis(const sad::Vector<sad::p2d::Point> & set)
{
    PROFILER_EVENT;
    if (set.size() == 0) return -1;
    
    double miny = set[0].y();
    int result = 0;
    for(unsigned int i = 0; i < set.size(); i++)
    {
        if (set[i].y() < miny) 
        {
            result = i;
            miny = set[i].y();
        }
    }
    return result;
}

typedef sad::Pair<p2d::Point, double> SetSortingEntry;

bool compare(const sad::p2d::SetSortingEntry & o1, const sad::p2d::SetSortingEntry & o2)
{
    PROFILER_EVENT;
    return o1.p2() < o2.p2();
}

double angle(const sad::p2d::Point & p1, const sad::p2d::Point & p2)
{
    PROFILER_EVENT;
    double  result = 0;
    if (!sad::equal(p1, p2))
    {
        double dy = p2.y() - p1.y();
        double dx = p2.x() - p1.x();
        double angle = atan2(dy , dx);
        result = angle;
        if (result < 0) result += 2 * M_PI;
    }
    return result;
}

sad::Vector<p2d::Point> build_sorted_set(const sad::Vector<sad::p2d::Point> & set, 
                                         int min_index)
{
    PROFILER_EVENT;
    assert( min_index > -1 && min_index < (int)(set.size()) );

    sad::Vector<sad::p2d::Point> result;
    sad::p2d::Point min_point = set[min_index]; 
    result << min_point;

    // Build heap
    sad::Vector<sad::p2d::SetSortingEntry> sortedset;
    for(unsigned int i = 0; i < set.size(); i++)
    {
        if (i != min_index) 
        {
            double angle= sad::p2d::angle(min_point, set[i]);
            sortedset << sad::p2d::SetSortingEntry(set[i], angle);
        }
    }

    // Sort heap
    std::sort(sortedset.begin(), sortedset.end(), sad::p2d::compare);

    // Append it to an end
    for(unsigned int i = 0; i < sortedset.size(); i++)
    {
        result << sortedset[i].p1();
    }

    return result;
}

bool is_convex(const sad::p2d::Point & prev, 
               const sad::p2d::Point & cur,
               const sad::p2d::Point & next)
{
    PROFILER_EVENT;
    double clockwiseangle = sad::p2d::angle(cur, prev);
    double counterclockwiseangle = sad::p2d::angle(cur, next);
    
    double max = clockwiseangle;
    double min = counterclockwiseangle;
    bool  cwisgreaterccw = (clockwiseangle > counterclockwiseangle);
    if (cwisgreaterccw == false)
    {
        std::swap(min,max);
    }
    double diff = (max - min);
    bool diffisgreaterthanpi = (diff > M_PI);
    bool result  = false;
    if (!sad::is_fuzzy_equal(diff, M_PI)) 
    {
        bool f1 = !cwisgreaterccw && diffisgreaterthanpi;
        bool f2 = cwisgreaterccw && !diffisgreaterthanpi;
        result = f1 || f2;
    }
    return result;
}

bool erase_concave_points(sad::Vector<sad::p2d::Point> & result)
{
    PROFILER_EVENT;
    bool erased = false;
    for(unsigned int i = 1; (i < result.size()) && result.size() > 2; i++)
    {
        sad::p2d::Point prev = result[i-1];
        sad::p2d::Point next = result[0];
        sad::p2d::Point cur = result[i];
        if (i !=  result.size() - 1) next = result[i+1];
        if (sad::p2d::is_convex(prev, cur, next) == false) 
        {
            result.removeAt(i);
            --i;
            erased = true;
        }
    }
    return erased;
}

}

}

sad::Vector<sad::p2d::Point> sad::p2d::graham_scan(const sad::Vector<sad::p2d::Point> & set)
{
    PROFILER_EVENT;
    sad::Vector<p2d::Point> result = set;
    sad::p2d::erase_equal_points(result);
    if (result.size() > 2)
    {
        // Run graham scan implementation

        // Find index of point with minimal y axis value
        int min_point_index = sad::p2d::find_min_point_on_y_axis(result);

        // Builds sorted set by angle to point with minimal point
        result = sad::p2d::build_sorted_set(result, min_point_index);

        // Optimize selected set, removing the concave points
        bool changed = true;
        do
        {
            changed = sad::p2d::erase_concave_points(result);
        } while (changed);
        
    }
    return result;
}
