/*! \file way.h
    

    Describes a way as a set of points, on which body will evenly move
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../vector.h"
#include "../point.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../axle.h"

#include "../../sadvector.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace p2d
{

namespace app
{

/*! A way point is a point in 2D space
 */
typedef p2d::Point WayPoint;


/*! Describes a way as a set of points, determining how way is performed.
    Building a way consists of two phases - construction, toggled, when calling
    p2d:Way::startConstruction(), and work - when called p2d::Way::construct.

    You can reset state by calling p2d::Way::startConstruction.
 */
class Way: public sad::db::Object
{
public:
    /*! Creates a default closed unconstructed way
     */
    Way();
    /*! Destructor for a way
     */
    virtual ~Way() override;
    /*! Steps a link on way
        \param[in] current_time a current time 
        \param[in] step a time step
        \return a point from way
     */
    sad::p2d::Point getPointInTime(double current_time, double step);
    /*! Sets i-th point to another point
        \param[in] i an index for point
        \param[in] p point
     */
    void setPoint(int i,  const sad::p2d::app::WayPoint& p);
    /*! Add new point
        \param[in] p point
     */
    void addPoint(const sad::p2d::app::WayPoint& p);
    /*! Inserts point in specified position
        \param[in] i a position
        \param[in] p point
     */
    void insertPoint(int i, const sad::p2d::app::WayPoint& p);
    /*! Removes a point
        \param[in] i an index for point
     */
    void removePoint(int i);
    /*! Returns, true, whether way is closed
     */
    bool closed() const;
    /*! Sets, whether way open or closed
     */
    void setClosed(bool b);
    /*! Makes a way closed, like circle
     */
    void makeClosed();
    /*! Makes a way simple, as open
     */
    void makeOpen();
    /*! Sets a total time for a way
        \param[in] time a time for data
     */ 
    void setTotalTime(double time);
    /*! Returns a time
        \return time
     */
    double totalTime() const;
    /*! Returns a way points
        \return way points
     */
    inline const sad::Vector<p2d::app::WayPoint> & wayPoints() const { return m_way_points; }
    /*! Constructs a way
     */
    void construct();
    /*! Starts anew construction of way
     */
    void startConstruction();
    /*! Returns string "sad::p2d::app::Way"
        \return name, under which object will be serialized
     */
    virtual const sad::String& serializableName() const override;
    /*! Returns schema for all kinds of way
        \return schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const override;
    /*! Loads sprite from picojson object
        \param[in] v a picojson object
        \return  whether it as successfull
     */
    virtual bool load(const picojson::value& v) override;
protected:
    bool m_constructed;                           //!< Whether way is constructed
    bool m_closed;                                //!< Whether way is closed
    double m_total_time;                           //!< Amount of time,  which is needed to pass all way
    sad::Vector<p2d::app::WayPoint> m_way_points;  //!< A set of way points
    sad::Vector<double>        m_times;           //!< A time, when the point should be reached
};

}

}

}
