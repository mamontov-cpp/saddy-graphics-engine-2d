/*! \file animationsresize.h
    

    Describes an animation for resizing an object
 */
#pragma once
#include "animationsanimation.h"

#include "../sadpoint.h"
#include "../sadsize.h"

namespace sad
{
    
namespace animations
{

/*! An animation of rotating the camera
 */
class Resize: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    Resize();
    /*! Can be inherited
     */
    virtual ~Resize() override;
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
    /*! Sets size of object on the start of animation
        \param[in] v starting size
     */
    void setStartSize(const sad::Point2D& v);
    /*! Sets size of object on the start of animation
        \param[in] v starting size
     */
    void setStartSize(const sad::Size2D& v);
    /*! Sets ending size of object on the end of animation
        \param[in] v ending size
     */
    void setEndSize(const sad::Point2D& v);
    /*! Sets ending size of object on the end of animation
        \param[in] v ending size
     */
    void setEndSize(const sad::Size2D& v);
    /*! Returns starting size for an animation
        \return size
     */
    const sad::Point2D& startSize() const;
    /*! Returns ending size for an animation
        \return size
     */
    const sad::Point2D& endSize() const;
    /*! An animations instance
        \param[in] i an instance for animation
     */
    virtual void start(sad::animations::Instance* i) override;
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
    /*! A starting size for resizing objects
     */
    sad::Point2D m_start_size;
    /*! An ending size for resizing objects
     */
    sad::Point2D m_end_size;
};

}

}

