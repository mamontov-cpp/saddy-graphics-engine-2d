/*! \file animationswaymoving.h
    

    Describes an animation for moving an object
 */
#pragma once
#include "animationsanimation.h"

#include "../p2d/app/way.h"

#include "../db/dbstronglink.h"


namespace sad
{
    
namespace animations
{

/*! An animation of moving for way
 */
class WayMoving: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    WayMoving();
    /*! Can be inherited
     */
    virtual ~WayMoving();
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Sets database for working with way, when loading
        \param[in] table a table
     */
    virtual void setTable(sad::db::Table * table);
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
    /*! Sets way for animation
        \param[in] way a way
     */ 
    void setWay(sad::p2d::app::Way * way);
    /*! Returns way
        \return way
     */
    sad::p2d::app::Way* way() const;
    /*! Sets way object id
        \param[in] id
     */
    void setWayObjectId(unsigned long long id);
    /*! Returns way object id
        \return way object id
     */
    unsigned long long wayObjectId() const;
    /*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o);
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o);
protected:
    /*! A link for a way
     */
    sad::db::StrongLink<sad::p2d::app::Way> m_way;
};

}

}

