/*! \file animationswayinstance.h
    

    A way instance for working with way path moving
 */
#pragma once
#include "animationsinstance.h"

#include "../db/dbstronglink.h"

#include "../p2d/app/way.h"

namespace sad
{
    
namespace animations
{

/*! Defines a instance, linked to way
 */
class WayInstance: public sad::animations::Instance
{
SAD_OBJECT
public:
   /*! Creates new  empty instance
     */
    WayInstance();
    /*! Copies instance state (except timer)
        \param[in] o instance
     */
    WayInstance(const sad::animations::WayInstance& o);
    /*! Copies instance
        \param[in] o instance
        \return reference to self
     */
    sad::animations::WayInstance& operator=(const sad::animations::WayInstance& o);
    /*! Could be inherited
     */
    virtual ~WayInstance();
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Sets a table
        \param[in] t a table
     */
    virtual void setTable(sad::db::Table* t);
    /*! Returns serializable name for an instance
        \return a serializable name
     */
    virtual const sad::String& serializableName() const;
    /*! Sets major id for instance
        \param[in] majorid
     */
    void setWayMajorId(unsigned long long majorid);
    /*! A major id for way
        \return major id
     */
    unsigned long long wayMajorId() const;
    /*! Sets way instance
        \param[in] i way
     */
    void setWay(sad::p2d::app::Way* i);
    /*! Returns current way
        \return way
     */
    sad::p2d::app::Way* way() const;
protected:
    /*! Starts an animation instance
        \param[in] animations an animations
     */
    virtual void start(sad::animations::Animations* animations);
    /*!  Computes time, which should be used to animate. Finishes
         animations to be finished
         \param[in] animations an animations
         \param[in] restoreOnFinish whether we should restore state on finish
     */
    virtual double computeTime(sad::animations::Animations* animations, bool restoreOnFinish = true);
    /*! Processes animation instance with specified time
        \param[in] animations an animation part
        \param[in] time a time
     */
    virtual void processTime(sad::animations::Animations* animations, double time);
    /*! Checks, whether instance is valid
        \param[in] animations an animation list
     */
    virtual void checkIfValid(sad::animations::Animations* animations);
    /*! Compiles fast call and saves an animtions
        \param[in] animations an animations
     */
    virtual void saveStateAndCompile(sad::animations::Animations* animations);
    /*! Tries to restore object state from cache
        \param[in] animations animations, which is calling process for this instance
     */
    virtual void restoreObjectState(sad::animations::Animations* animations);
    /*! Returns a way link
     */
    sad::db::StrongLink<sad::p2d::app::Way> m_way_link;
};
    
}

}


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::animations::WayInstance)
