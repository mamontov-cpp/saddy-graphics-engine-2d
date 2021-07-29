/*! \file animationscamerarotation.h
    

    Describes an animation for editing a camera rotation
 */
#pragma once
#include "animationsanimation.h"
#include "animationssavedcamerarotation.h"

#include "setstate/setcamerarotation.h"

#include "../sadpoint.h"
#include "../scene.h"



namespace sad
{
    
namespace animations
{

/*! An animation of rotating the camera
 */
class CameraRotation: public sad::animations::Animation
{
SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    CameraRotation();
    /*! Can be inherited
     */
    virtual ~CameraRotation() override;
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
    /*! Set minimal angle for camera rotation
        \param[in] angle an angle for rotation
     */
    void setMinAngle(double angle);
    /*! Returns minimal angle
        \return minimal angle for rotation
     */
    double minAngle() const;
    /*! Set maximal angle for camera rotation
        \param[in] angle an angle for rotation
     */
    void setMaxAngle(double angle);
    /*! Returns maximal angle
        \return maximal angle for rotation
     */
    double maxAngle() const;
    /*! Sets a pivot point for rotation
        \param[in] p pivot
     */
    void setPivot(const sad::Point3D& p);
    /*! Returns a pivot point for rotation
        \return pivot point
     */
    const sad::Point3D& pivot() const;
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
    /*! A minimal angle for editing
     */
    double m_min_angle;
    /*! A maximal angle for editing
     */
    double m_max_angle;
    /*! A pivot point for rotation
     */
    sad::Point3D m_pivot;
};

}

}

