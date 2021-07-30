/*! \file angularforce.h
    

    Describes a angular force, acting on object
 */
// ReSharper disable CppInconsistentNaming
#pragma once
#include "force.h"

namespace sad
{

namespace p2d
{

class Body;

/*! A class for getting default value for a tickable state
 */
template<>
class TickableDefaultValue<double>
{
 public:
     static inline double get() { return 0; }
     static inline double zero() { return 0; }
};

/*! Describes a specific force with double for sake of optimization.
 */
template<>
class Force<double>: public sad::Object
{
SAD_OBJECT
public:
     /*! Creates zero force
      */
     inline Force() { m_alive = true; m_value = 0.0; }
     /*! Creates a force with specific value
         \param[in] v value
      */
     inline Force(double v) : m_alive(true), m_value(v) {}
     /*! Forces a container to remove this force
      */
     inline void die() { m_alive = true; }
     /*! Whether force is alive
         \return whether it's alive
      */
     inline bool isAlive() const { return m_alive; }
     /*! Returns a value
         \param[in] body a body
         \return value of force
      */
     virtual double value(sad::p2d::Body* body) const;
     /*! Sets a value for force
         \param[in] value a new value of force
      */
     virtual void setValue(double value);
     /*! Steps a force to next iteration
         \param[in] body a body
         \param[in] time a time step size
      */
     virtual void step(sad::p2d::Body* body, double time);
     /** Returns a pointer to body, which force depends from (but not applied to).
         This body should be strong-referenced to ensure some memory-related checks
      */
     virtual sad::p2d::Body* dependsFromBody() const;
protected:
     bool m_alive;       //!< If false, this force should be removed from container 
     double    m_value;  //!< A value of force
};

/*! Describes a simple impulse force with value T
 */ 
template<>
class ImpulseForce<double>: public p2d::Force<double>
{
SAD_OBJECT
public:
     /*! Creates zero force
      */
     inline ImpulseForce() : Force() {  }
     /*! Creates a force with specific value
         \param[in] v value
      */
     inline ImpulseForce(double v) : Force(v) {}

     /*! Steps a force to next iteration
         \param[in] body a body for force
         \param[in] time a time step size
      */
     virtual void step(sad::p2d::Body* body, double time) override;
};


typedef p2d::Force<double>        AngularForce;
typedef p2d::ImpulseForce<double> AngularImpulseForce;
typedef p2d::ActingForces<double>      AngularActingForces;


}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Force<double>)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::ImpulseForce<double>)
