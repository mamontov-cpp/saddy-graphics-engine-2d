/*! \file body.h
    

    Describes a body object
 */
#pragma once

#include "collisionshape.h"
#include "force.h"
#include "angularforce.h"
#include "movement.h"

#include "../object.h"
#include "../sadstring.h"


/*! A special point, which can be added to time or other values to make objects
    not collide
 */
#define COLLISION_PRECISION 1.0E-4

namespace sad
{

namespace p2d
{
class World;
/*! Describes a body in physics engine
 */
class Body: public sad::Object
{
SAD_OBJECT
public:
    /*! Construct new body line with zero width at (0,0) with zero speed
     */
    Body();
    /*! Returns current shape for a body
     */
    p2d::CollisionShape * currentShape();

    /*! Sets linked user objects
        \param[in] objects list of objects
     */
    template<
        typename _ObjectType
    >
    void setUserObjects(const sad::Vector<_ObjectType*>& objects)
    {
        if (m_user_objects.size())
        {
            for(size_t i = 0; i < m_user_objects.size(); i++)
            {
                m_user_objects[i]->delRef();
            }
        }
        m_user_objects.clear();
        for(size_t i = 0; i < objects.size(); i++)
        {
            if (objects[i])
            {
                objects[i]->addRef();
                m_user_objects << objects[i];
            }
        }
    }
    /*! Returns user objects
        \return objects
     */
    const sad::Vector<sad::Object*>& userObjects() const;
    /*! Sets user object for a body
     */
    void setUserObject(sad::Object * o);
    /*! Returns an inner user-defined object
        \return inner user-defined object
     */
    sad::Object * userObject() const;
    /*! Returns a type of user-defined object
        \return type of object
     */
    virtual const sad::String & userType() const;
    /*! Returns a position of body at specified time.
        \param[in] time time, when position is needed
        \param[in] index index, where shape should be stored 
        \return what shape, body had at specified time
     */
    virtual p2d::CollisionShape & at(double time, int index = 0) const;
    /*! Notifies body, that item is rotated
        \param[in] delta difference between angles 
     */
    void notifyRotate(const double & delta);
    /*! Notifies body, that item is moved
        \param[in] delta difference between point
     */
    void notifyMove(const p2d::Vector & delta);
    /*! Steps values, that must be changed at end of time step, like
        a ghost options and force
        \param[in] time specified time
     */
    void stepDiscreteChangingValues(double time);
    /*! Steps positions and values at specified time
        \param[in] time specified time 
     */
    void stepPositionsAndVelocities(double time);
    /*! Adds listener for body movement
        \param[in] listener a common listener 
     */
    inline void addMoveListener(p2d::TangentialMovement::listener_t listener)
    {
        m_tangential->addListener(listener);
    }
    /*! Adds listener for body movement
        \param[in] listener a common listener 
     */
    inline void addMoveListener(const std::function<void(const sad::p2d::Vector&)>& fn)
    {
        m_tangential->addListener(new  sad::p2d::LambdaMovementDeltaListener<sad::p2d::Vector>(fn));
    }
    /*! Adds listener for body movement
        \param[in] listener a common listener 
     */
    inline void addMoveListener(const std::function<void(sad::p2d::Vector)>& fn)
    {
        m_tangential->addListener(new  sad::p2d::LambdaMovementDeltaListener<sad::p2d::Vector>(fn));
    }
    /*! Removes listener for body movement
        \param[in] listener a common listener 
     */
    inline void removeMoveListener(p2d::TangentialMovement::listener_t listener)
    {
        m_tangential->removeListener(listener);
    }
    /*! Adds listener for body rotation
        \param[in] listener a common listener
     */
    inline void addRotateListener(p2d::AngularMovement::listener_t listener)
    {
        m_angular->addListener(listener);
    }
    /*! Adds listener for body rotation
        \param[in] listener a common listener 
     */
    inline void addRotateListener(const std::function<void(const double&)>& fn)
    {
        m_angular->addListener(new  sad::p2d::LambdaMovementDeltaListener<double>(fn));
    }
    /*! Adds listener for body rotation
        \param[in] listener a common listener 
     */
    inline void addRotateListener(const std::function<void(double)>& fn)
    {
        m_angular->addListener(new  sad::p2d::LambdaMovementDeltaListener<double>(fn));
    }

    /*! Attached list of objects fully, adding listeners for them, that will move them according to body position
        \param[in] objects an objects, that will be set as inner
     */
    template<
        typename _Object
    >
    inline void attachObjects(const sad::Vector<_Object*>& objects)
    {
        this->addMoveListener(new sad::p2d::ObjectGroupTangentialDeltaListener<_Object>(objects));
        this->addRotateListener(new sad::p2d::ObjectGroupAngularDeltaListener<_Object>(objects));
        this->setUserObjects(objects);
    }
    /*! Attached object fully, adding listeners for them, that will move it according to body position
        \param[in] object an object
     */
    template<
        typename _Object
    >
    inline void attachObject(_Object* object)
    {
        this->addMoveListener(new sad::p2d::ObjectGroupTangentialDeltaListener<_Object>(object));
        this->addRotateListener(new sad::p2d::ObjectGroupAngularDeltaListener<_Object>(object));
        this->setUserObject(object);
    }
    /*! Removes listener for body rotation
        \param[in] listener a common listener
     */
    inline void removeRotateListener(p2d::AngularMovement::listener_t listener)
    {
        m_angular->removeListener(listener);
    }
    /*! Tries to set transformer for current shape of body
     */
    void trySetTransformer();
    /*! Sets new weight for body
        \param[in] weight new weight
     */
    void setWeight(const p2d::Weight & weight);
    /*! Returns current weight for body
        \return  weight
     */
    const p2d::Weight & weight() const;
    /*! Returns current weight for body
        \return  weight
     */
    p2d::Weight & weight();
    /*! Sets flag, which points whether current body is ghost
        \param[in] value if true body is ghost, otherwise it is not
     */
    void setIsGhost(bool value);
    /*! Tests whether body is ghost
        \return whether body is ghost
     */
    bool isGhost() const;
    /*! Sets new world for body
        \param[in] world world
     */
    void setWorld(p2d::World * world);
    /*! Returns a world for body
        \return world
     */
    p2d::World * world();
    /*! Sets new shape for a body. Shape must have center at (0,0)
        and rotated by zero angle. It will move automatically to current 
        points and rotate by specified angle
        \param[in] shape new shape
     */
    void setShape(p2d::CollisionShape * shape);
    virtual ~Body();

    typedef p2d::MovementDeltaListener<p2d::Body, p2d::Vector> move_t;
    typedef p2d::MovementDeltaListener<p2d::Body, double> rotate_t;
    /*! Inits angle for body
        \param[in] angle an angle
     */
    void initAngle(double angle) const;
    /*! Inits position for body
        \param[in] p point
     */
    void initPosition(const p2d::Point& p) const;
    /*! Sets current position for object
        \param[in] p point
     */
    void setCurrentPosition(const p2d::Point & p);
    /*! Sets next position for object
        \param[in] p point
     */
    void shedulePosition(const p2d::Point & p);
    /*! Shedules specific position at specified time
        \param[in] p point
        \param[in] time time when position should change
     */
    void shedulePositionAt(const p2d::Point & p, double time);
    /*! Returns current position of body
        \return current position of body
     */
    const p2d::Vector & position() const;
    /*! Determines, whether body will teleport at end of current time step
        \return whether position will change
     */
    bool willPositionChange() const;
    /*! Returns next position, where body will be teleported
        \return next position
     */
    p2d::Vector nextPosition() const;
    /*! Sets current tangential velocity
        \param[in] v velocity
     */
    void setCurrentTangentialVelocity(const p2d::Vector & v);
    /*! Shedules new velocity
        \param[in] v velocity
     */
    void sheduleTangentialVelocity(const p2d::Vector & v);
    /*! Shedules new velocity at specified time
        \param[in] v velocity
        \param[in] time time when velocity should be applied
     */
    void sheduleTangentialVelocityAt(const p2d::Vector & v, double time);
    /*! A tangential velocity
        \return tangential velocity
     */
    const p2d::Vector & tangentialVelocity() const;
    /*! Determines, whether body will leap to other velocity at end of current time step
        \return whether position will change
     */
    bool willTangentialVelocityChange() const;
    /*! Returns next velocity, where body will change speed due to user call
        \return next position
     */
    p2d::Vector nextTangentialVelocity() const;
    /*! Sets current angle for object
        \param[in] angle new angle
     */
    void setCurrentAngle(double angle);
    /*! Sets next angle for object
        \param[in] angle specified angle
     */
    void sheduleAngle(double angle);
    /*! Shedules angle for object at specified time
        \param[in] angle specified angle
        \param[in] time  a specified time
     */
    void sheduleAngleAt(double angle, double time);
    /*! Returns current angle of body
        \return current angle of body
     */
    double angle() const;
    /*! Determines, whether body will change angle at end of current time step
        \return whether position will change
     */
    bool willAngleChange() const;
    /*! Returns next angle, which body will be set , due to user call of shedule
        \return next angle
     */
    double nextAngle() const;
    /*! Sets current tangential velocity
        \param[in] v velocity
     */
    void setCurrentAngularVelocity(double v);
    /*! Shedules new angular velocity
        \param[in] v velocity
     */
    void sheduleAngularVelocity(double v);
    /*! Shedules new angular velocity at specified time
        \param[in] v velocity
        \param[in] time time of specified velocity
     */
    void sheduleAngularVelocityAt(double v, double time);
    /*! A angular velocity
        \return tangential velocity
     */
    double angularVelocity() const;
    /*! A angular velocity
        \return tangential velocity
     */
    double angularVelocityAt(double time) const;
    /*! Determines, whether body will leap to other velocity at end of current time step
        \return whether position will change
     */
    bool willAngularVelocityChange() const;
    /*! Returns next velocity, where body will change speed due to user call
        \return next position
     */
    double nextAngularVelocity() const;
    /*! Moves body by specified vector
        \param[in] v vector
     */
    void move(const p2d::Vector & v);
    /*! Rotates body by specified angle
     */
    void rotate(double delta);
    /*! Clears all move listeners for body
     */
    void clearMoveListeners();
    /*! Clears all rotate listeners for body
     */
    void clearRotateListeners();
    /*! Clears all of listeners for body
     */
    void clearListeners();
    /*! Adds new force to tangential forces list
        \param[in] force a force
     */
    void addForce(sad::p2d::Force<sad::p2d::Vector>* force);
    /*! Adds new force to angular forces list
        \param[in] force a force
     */
    void addForce(sad::p2d::Force<double>* force);
    /*! Removes force from tangential forces list
        \param[in] force a force
     */
    void removeForce(sad::p2d::Force<sad::p2d::Vector>* force);
    /*! Removes force from  to angular forces list
        \param[in] force a force
     */
    void removeForce(sad::p2d::Force<double>* force);
    /*! Clears tangential forces
     */
    void clearTangentialForces();
    /*! Clears angular forces
     */
    void clearAngularForces();
    /*! Clear forces
     */
    void clearForces();
    /*! Returns list of tangential forces
        \return list of tangential forces
     */
    const sad::Vector<sad::p2d::Force<sad::p2d::Vector>* >& tangentialForcesList();
    /*! Returns list of angular forces
        \return list of angular forces
    */
    const sad::Vector<sad::p2d::Force<double>* >& angularForcesList();
    /*! Adds new force to tangential forces list
        \param[in] force a force
        \param[in] time a time for adding a force
    */
    void sheduleAddForce(sad::p2d::Force<sad::p2d::Vector>* force);
    /*! Adds new force to angular forces list
        \param[in] force a force
        \param[in] time a time for adding a force
    */
    void sheduleAddForce(sad::p2d::Force<double>* force);
    /*! Adds new force to tangential forces list
        \param[in] force a force
        \param[in] time a time for adding a force
     */
    void sheduleAddForce(sad::p2d::Force<sad::p2d::Vector>* force, double time);
    /*! Adds new force to angular forces list
        \param[in] force a force
        \param[in] time a time for adding a force
     */
    void sheduleAddForce(sad::p2d::Force<double>* force, double time);


    /*! Returns a tangential forces, acting on body
     */
    p2d::TangentialActingForces & tangentialForces();
    /*! Returns an angular forces, acting on body
     */
    p2d::AngularActingForces  & angularForces();
    /*! Returns an average velocity
        \return average velocity
     */
    p2d::Vector averageChangeIndependentTangentialVelocity(); 
    /*! Returns a tangential velocity at specified time
        \param[in] time a time
        \return velocity
     */
    p2d::Vector tangentialVelocityAt(double time);
    /*! Builds an acceleration cache for any of bodies
     */
    void buildCaches();
    /*! Returns  time step for body
        \return time step
     */
    double timeStep() const;
    /*! If next position is scheduled, places object between two positions,
        otherwise schedules new position. Note, that instead a position, a distance
        between current position and new is passed
        \param[in] distance
     */
    void correctPosition(const p2d::Vector & distance);
    /*! If next velocity is scheduled, computes middle of two velocities, otherwise
        schedules new velocity. 
        \param[in] v new velocity
     */
    void correctTangentialVelocity(const p2d::Vector & v);
    /*! Sets object as fixed. A fixed objects does not change their impulse on collision
        \param[in] fixed new value for fixed
     */
    inline void setFixed(bool fixed) { m_fixed = fixed;}
    /*! Returns whether bodies are fixed. Fixed bodies can't change their speeds if moving.
        They can change it, only if bounced from another fixed body.
        \return whether body is fixed
     */
    inline bool fixed() const { return m_fixed; }
    /*! Set amount of sampling, needed to cache positions for collision detection
     */
    void setSamplingCount(int samples);
    /*! Builds inner bodu caches with time step
        \param time_step a current time step
     */
    void buildCaches(double time_step);
public:
    /*! A special constant, needed to be set by world in order to set step for body
     */
    double TimeStep;
    /*! A temporary shapes for returning shape at specific time.
        Selected and freed here. It's a temporary shapes 
     */
    CollisionShape * Temporary;
private:
    /*! A weight of specific body
     */
    p2d::Weight m_weight;
    /*! Whether body is ghost
     */
    bool m_is_ghost;
    /*! A world simulation
     */
    p2d::World* m_world;
    /*! Returns a user object
        \return user object for a body
     */
    sad::Vector<sad::Object*> m_user_objects;
    /*! A tangential movement for body
     */
    p2d::TangentialMovement* m_tangential;
    /*! A angular movement for body
     */
    p2d::AngularMovement* m_angular;
    /*! A current shape of data
     */
    CollisionShape* m_current;
    /* An index for last sample
     */
    int  m_lastsampleindex;
    /*! Whether position is cached by valid
     */
    bool m_samples_are_cached;
    /*! A type size of shape in body
     */
    size_t  m_shapesize;
    /*! Describes, whether this body should not be changed
     */
    bool m_fixed;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Body)
