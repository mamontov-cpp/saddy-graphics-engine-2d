/*! \file phy52d/object.h

    Describes main application object, that is basic for all objects in application
 */
#pragma once
#include <p2d/body.h>

#include <sprite2d.h>
#include <object.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <sadvector.h>

namespace sad
{

namespace phy52d
{

class App;
/*! Describes a basic in-game object, which provides primitives, needed to 
    describe all in-game objects
 */
class Object: public sad::SceneNode
{
    /* Declare metadata, needed to describe inheritance tree, name of class
       This metadata can be used where real type is needed - for most part,
       for collision detection
     */
    SAD_OBJECT
public:
     /*! Creates an empty object
      */
     Object();
     /*! Moves an object sprite by specified direction.
         Note, that this function should be called only from body's listener,
         to preserve synchronization between sprite and physical body
         \param[in] dist distance 
      */
     void notifyMove(const p2d::Vector & dist);
     /*! Rotates an object sprite nby specified direction
         Note, that this function should be called only from body's listener
         to preserve synchronization between sprite and physical body
         \param[in] angle rotated angle
      */
     void notifyRotate(const double & angle);
     /*! Sets an angle of game object to specified
         \param[in] angle an angle
      */
     void setAngle(double angle) const;
     /*! Makes game object look to specified point
         \param[in] p point
      */
     void lookAt(const sad::Point2D & p) const;
     /*! Sets angular velocity, making object rotating clockwise
         \param[in] v velocity
      */
     void setAngularVelocity(double v) const;
     /*! Sets only horizontal speed
         \param[in] v speed
      */
     void setHorizontalSpeed(double v) const;
     /*! Sets only vertical speed
         \param[in] v speed
      */
     void setVerticalSpeed(double v) const;
     /*! Stops horizontal movement
      */
     void stopHorizontal() const;
     /*! Stops vertical movement
      */
     void stopVertical() const;
     /*! Stops a game object tangential movement
      */
     void stop() const;
     /*! Sets a world, which object belongs to
         \param[in] g world
      */
     void setApp(sad::phy52d::App * g);
     /*! Sets scene for a sprite
         \param[in] s scene
      */
     virtual void setScene(sad::Scene * s) override;
     /*! Called, when scene renderer's changed
      */
     virtual void rendererChanged() override;
     /*! The object does not own anything, if game is not null
      */
     virtual ~Object() override;
     /*! Returns body of object	
         \return body of object
      */ 
     p2d::Body * body() const;
     /*! Called, when object is rendered. Object can easily re-implement
         it to work with AI, or do something other (like shoot). 
      */
     virtual void render() override;
     /*! Returns a world for object
         \return object
      */
     sad::phy52d::App * app() const;
     /*! Returns a position of  object
         \return a positions
      */
     p2d::Point position() const;
     /*! Sets a position for  object
         \param[in] p point
      */
     void setPosition(const p2d::Point & p) const;
     /*! A basic method for moving object
         \param[in] p point
      */
    // ReSharper disable once CppInconsistentNaming
    virtual void moveBy(sad::Point2D& p);  // NOLINT(clang-diagnostic-overloaded-virtual)
     /*! Returns an angle of  object
         \return an angle
      */
     double angle() const;
     /*! Adds new tangential force
         \param[in] f force
      */
     void addTangentialForce(p2d::TangentialForce * f) const;
     /*! Adds new angular force
         \param[in] f force
      */
     void addAngularForce(p2d::AngularForce * f) const;
     /*! Sets a tangential velocity
         \param[in] v velocity
      */
     void setTangentialVelocity(const p2d::Vector & v) const;
     /*! Sets object' sprite options
      *  \param[in] options options to set
      */
     void setOptions(sad::Sprite2D::Options* options) const;
     /*! Sets collision shape
      *  \param[in] shape a collision shape
      */
     void setShape(sad::p2d::CollisionShape* shape) const;
     /*! Sets shader function
      *  \param[in] fun a function
      */
     virtual void setShaderFunction(sad::ShaderFunction* fun) override;
     /*! Called, when object is added to scene
      */
     void onAddedToScene() override;
     /*! Called, when object is removed from scene
      */
     void onRemovedFromScene() override;
protected:
     /*! A body is representation for a game object in physics engine, needed
      */
     p2d::Body * m_body;
     /*! A sprite is a graphical representation for a game object
      */
     sad::Sprite2D * m_sprite;
     /*! An object is linked to game to forward the in-game to a game object 
      */
     sad::phy52d::App * m_app;
     /*! A first listener for movement of body
      */
     p2d::MovementDeltaListener<sad::phy52d::Object, p2d::Vector> * m_listener1;
     /*! A second listener for movement of body
      */
     p2d::MovementDeltaListener<sad::phy52d::Object, double> * m_listener2;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::phy52d::Object)
