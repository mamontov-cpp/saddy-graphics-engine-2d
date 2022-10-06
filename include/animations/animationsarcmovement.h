/*! \file animationsarcmovement.h
 *  
 *  Declares a simple animation for movement on an arc
 */
#pragma once
#include "animationsanimation.h"
#include "../geometry2d.h"

#include "../sadpoint.h"


namespace sad
{

namespace animations
{

/*! An animation for simple arc movement of object
 */
class ArcMovement : public sad::animations::Animation
{
    SAD_OBJECT
public:
    /*! Creates new empty animation
     */
    ArcMovement();
    /*! Deleted
     *  \param[in] o other
     */
    ArcMovement(const sad::animations::ArcMovement& o) = delete;
    /*! Deleted
     *  \param[in] o other
     *  \return self-reference
     */
    sad::animations::ArcMovement& operator=(const sad::animations::ArcMovement& o) = delete;
    /*! Can be inherited
     */
    virtual ~ArcMovement() override;
    /*! Sets center point
        \param[in] p point
     */
    void setCenterPoint(const sad::Point2D& p);
    /*! Sets starting point
        \param[in] p point
     */
    void setStartingPoint(const sad::Point2D& p);
    /*! Sets ending point
        \param[in] p point
     */
    void setEndingPoint(const sad::Point2D& p);
    /*! Returns center point
        \return center point
     */
    const sad::Point2D& centerPoint() const;
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
    /*! Starts an animation. Called on start of instance.
        \param[in] i animation instance
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
    /*! Sets whether clamping floating points should be performed
     *  \param[in] clamp whether clamping points should be performed
     */
    void setClampFloatingPoints(bool clamp);
    /*! Returns true if clamping floating points should be performed
     *  \return value
     */
    bool clampFloatingPoints() const;
protected:
    /*! Recomputes movements properties
     */
    void recomputeProperties();
    /*! A center point
     */
    sad::Point2D m_center_point;
    /*! A starting point for animation
     */
    sad::Point2D m_start_point;
    /*! An ending point for animation
     */
    sad::Point2D m_end_point;
    /*! Whether we should clamp floating points
     */
    bool m_clamp_floating_points;
    /*! Movement properties
     */
    sad::EllipticMovementProperties m_properties;
};

}

}
