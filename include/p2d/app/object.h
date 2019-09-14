/*! \file p2d/app/object.h

    Describes main application object, that is basic for all objects in application
 */
#pragma once
#include "constants.h"

#include "../body.h"

#include "../../sprite2d.h"
#include "../../object.h"
#include "../../sadvector.h"

namespace sad
{

namespace p2d
{

namespace app
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
     void setAngle(double angle);
     /*! Makes game object look to specified point
         \param[in] p point
      */
     void lookAt(const sad::Point2D & p);
     /*! Sets angular velocity, making object rotating clockwise
         \param[in] v velocity
      */
     void setAngularVelocity(double v);
     /*! Sets only horizontal speed
         \param[in] v speed
      */
     void setHorizontalSpeed(double v);
     /*! Sets only vertical speed
         \param[in] v speed
      */
     void setVerticalSpeed(double v);
     /*! Stops horizontal movement
      */
     void stopHorizontal();
     /*! Stops vertical movement
      */
     void stopVertical();
     /*! Stops a game object tangential movement
      */
     void stop();
     /*! Sets a world, which object belongs to
         \param[in] g world
      */
     void setApp(p2d::app::App * g);
     /*! Sets scene for a sprite
         \param[in] s scene
      */
     virtual void setScene(sad::Scene * s);
     /*! Called, when scene renderer's changed
      */
     virtual void rendererChanged();
     /*! The object does not own anything, if game is not null
      */
     virtual ~Object();
     /*! Returns body of object	
         \return body of object
      */ 
     p2d::Body * body();
     /*! Called, when object is rendered. Object can easily reimplement
         it to work with AI, or do something other (like shoot). 
      */
     virtual void render();
     /*! Returns a world for object
         \return object
      */
     p2d::app::App * app();
     /*! Returns a position of  object
         \return a positions
      */
     p2d::Point position() const;
     /*! Sets a position for  object
         \param[in] p point
      */
     void setPosition(const p2d::Point & p);
     /*! A basic method for moving object
         \param[in] p point
      */
     virtual void moveBy(sad::Point2D& p);
     /*! Returns an angle of  object
         \return an angle
      */
     double angle() const;
     /*! Adds new tangential force
         \param[in] f force
      */
     void addTangentialForce(p2d::TangentialForce * f);
     /*! Adds new angular force
         \param[in] f force
      */
     void addAngularForce(p2d::AngularForce * f);
     /*! Sets a tangential velocity
         \param[in] v velocity
      */
     void setTangentialVelocity(const p2d::Vector & v);
     /*! Sets shader function
      *  \param[in] fun a function
      */
     virtual void setShaderFunction(sad::ShaderFunction* fun);
protected:
     /*! A body is representation for a game object in physics engine, needed
      */
     p2d::Body * m_body;
     /*! A sprite is a graphical representation for a game object
      */
     sad::Sprite2D * m_sprite;
     /*! An object is linked to game to forward the in-game to a game object 
      */
     p2d::app::App * m_app;
     /*! A first listener for movement of body
      */
     p2d::MovementDeltaListener<p2d::app::Object, p2d::Vector> * m_listener1;
     /*! A second listener for movement of body
      */
     p2d::MovementDeltaListener<p2d::app::Object, double> * m_listener2;
protected:
     /*! Inits object parameters from constants of specified type
      */
     template<typename T> void initFromConstants()
     {
         Sprite2D::Options * o = p2d::app::Constants<T>::sprite();
         this->m_sprite->set(*o);
         delete o;
         this->m_body->setShape(p2d::app::Constants<T>::shape());
     }
};

}

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::app::Object)
