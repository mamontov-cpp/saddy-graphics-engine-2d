/*! \file animationswaymoving.h
    

    Describes an animation for moving an object
 */
#pragma once
#include "animationsanimation.h"

#include "../way.h"

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
    virtual ~WayMoving() override;
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const override;
    /*! Sets database for working with way, when loading
        \param[in] table a table
     */
    virtual void setTable(sad::db::Table * table) override;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successful
     */
    virtual bool loadFromValue(const picojson::value& v) override;
    /*! Sets way for animation
        \param[in] way a way
     */ 
    void setWay(sad::Way * way);
    /*! Returns way
        \return way
     */
    sad::Way* way() const;
    /*! Sets way object id
        \param[in] id id of way object
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
    virtual void setState(sad::animations::Instance* i, double time) override;
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o) override;
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o) override;
protected:
    /*! A link for a way
     */
    sad::db::StrongLink<sad::Way> m_way;
};

}

}

