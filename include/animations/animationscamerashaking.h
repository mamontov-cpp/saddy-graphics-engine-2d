/*! \file animations/animationscamerashaking.h
    

    Declares animation of shaking the camera
 */
#pragma once
#include "animationsanimation.h"

#include "../sadpoint.h"
#include "../scene.h"

namespace sad
{
    
namespace animations
{

/*! An animation of shaking the camera
 */
class CameraShaking: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    CameraShaking();
    /*! Can be inherited
     */
    virtual ~CameraShaking();
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
    /*! Sets offset of how far camera should be translated
        \param[in] offset offset of how far camera should be translated
     */
    void setOffset(const sad::Point2D& offset);
    /*! Returns offset of how far camera should be translated
        \return offset
     */
    const sad::Point2D & offset() const;
    /*! Sets frequency of how many times, should be changed offset of camera of scene
        \param[in] freq frequency
     */
    void setFrequency(int freq);
    /*! Returns frequency of how many times, should be changed offset of camera of scene
        \return frequency
     */
    int frequency() const;
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
    /*! A vector of, how far should the camera be translated
     */
    sad::Point2D m_offset;
    /*! A frequency of how many translations should be performed, when camera shake is finished
     */
    int m_frequency;
};

}

}
