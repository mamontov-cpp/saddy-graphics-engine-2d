/*! \file animationssimplemovement.h
    

    Describes an animation for simple movement of object on line
 */
#pragma once
#include "animationsanimation.h"

#include "../sadpoint.h"


namespace sad
{
    
namespace animations
{

/*! An animation for simple movement of object on line
 */
class SimpleMovement: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    SimpleMovement();
    /*! Can be inherited
     */
    virtual ~SimpleMovement() override;
    /*! Sets starting point
        \param[in] p point
     */
    void setStartingPoint(const sad::Point2D& p);
    /*! Sets ending point
        \param[in] p point
     */
    void setEndingPoint(const sad::Point2D& p);
    /*! Returns starting point
        \return starting point
     */
    const sad::Point2D& startingPoint() const;
    /*! Returns ending point
        \return ending point
     */
    const sad::Point2D& endingPoint() const;
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const override;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successful
     */
    virtual bool loadFromValue(const picojson::value& v) override;
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
    /*! A starting point for animation
     */
    sad::Point2D m_start_point;
    /*! An ending point for animation
     */
    sad::Point2D m_end_point;
};

}

}
