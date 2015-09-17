/*! \file animations/animationstexturecoordinatescontinuous.h
    

    Here is defined animation, which interpolates texture coordinates from one rectangle
    to another
 */
#pragma once

#include "../fuzzyequal.h"
#include "../sadvector.h"
#include "../sadstring.h"
#include "../sadhash.h"

#include "animationsanimation.h"


namespace sad
{
    
namespace animations
{

/*! An animation, which changes texture coordinates of sprite over time starting with one
    rectangle to another, ending.
 */
class TextureCoordinatesContinuous: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    TextureCoordinatesContinuous();
    /*! Can be inherited
     */
    virtual ~TextureCoordinatesContinuous();
    /*! A basic schema for object
        \return a schema
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
    /*! Sets starting rectangle
        \param[in] rect rectangle
     */
    void setStartRect(const sad::Rect2D& rect);
    /*! Returns starting rectangle
        \return rectangle
     */
    const sad::Rect2D& startRect() const;
    /*! Sets ending rectangle
        \param[in] rect rectangle
     */
    void setEndRect(const sad::Rect2D& rect);
    /*! Returns ending rectangle
        \return rectangle
     */
    const sad::Rect2D& endRect() const;
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
    /*! A starting rectangle
     */
    sad::Rect2D m_start_rect;
    /*! An ending rectangle 
     */
    sad::Rect2D m_end_rect;
};

}

}
