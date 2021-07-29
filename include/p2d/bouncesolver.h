/*! \file bouncesolver.h
    A solver for bouncing bodies, when they are colliding
 */
#pragma once
#include "body.h"
#include "findcontactpoints.h"

#include "../maybe.h"
#include "../object.h"

namespace sad
{

namespace p2d
{

class CollisionTest;
/*! A bounce solver allows to easily solve problems with 
    bouncing of various objects. 
    
    Due to hard problems with fastly changed forces it is HIGHLY recommended, that
    objects must be checked for collision with p2d::CollisionTest, before calling a
    bounce. 
 */
class BounceSolver: public sad::Object  // NOLINT(cppcoreguidelines-special-member-functions)
{
SAD_OBJECT
public:
    /*! A type of inelastic collisions for solver
     */
    enum InelasticCollisionType
    {
       ICT_NO_INELASTIC_COLLISION = 0,   //!< Resolve all collisions as elastic, do not enable special routine for inelastic collisions
       ICT_FIRST = 1,                    //!< First body in collision will stick to other body and move as one with it, other won't change it's speed
       ICT_SECOND = 2                    //!< Second body in collision
    };
    /*! A solver task for solving collision data
     */
    struct SolverTask
    {
        sad::p2d::CollisionShape* First{nullptr};          //!<  A first moving shape
        sad::p2d::Vector          FirstVelocity;        //!<  A first velocity for solving
        sad::p2d::CollisionShape* Second{nullptr};         //!<  A second moving shape
        sad::p2d::Vector          SecondVelocity;       //!<  A second velocity for solving
        double PivotTime{0};                            //!< A pivot time for solving, 0 is for start
        /*! Kills task
         */
        void destroy() const;
    };
    /*! Constructs new solver
     */
    BounceSolver();
    /*! Removes allocated algorithm for finding data
     */
    ~BounceSolver();
    /*! Pushes restitution coefficient for two objects. A coefficient will be  restored to 1,
        after performing bounce. If you set coefficient to 1, bounce solver will work with
        bouncing as absolutely elastic, 0 - absolutely inelastic
        \param[in] value a new value for recovery coefficient
     */
    inline void pushRestitutionCoefficient(double value)
    {
        m_restitution_coefficient = value;
    }
    /*! Returns restitution coefficien for two objects. A coefficient will be  restored to 1,
     *  after performing bounce. If you set coefficient to 1, bounce solver will work with
     *  bouncing as absolutely elastic, 0 - absolutely inelastic
     *  \return flag value
     */
    inline double getRestitutionCoefficient() const
    {
        return m_restitution_coefficient;
    }
    /*! Pushes resilience coefficient for two objects. A coefficients are restored to normal,
        after performing bounce.
        \param[in] r coefficient
        \param[in] index an index
     */
    inline void pushRotationFriction(double r, int index)
    {
        assert(index == 1 || index == 2 );
        m_rotationfriction[index - 1] = r;
    }
    /*! Pushes resilience coefficient for two objects. A coefficients are restored to normal,
        after performing bounce.
        \param[in] r coefficient
     */
    inline void pushRotationFriction(double r)
    {
        m_rotationfriction[0] = r;
        m_rotationfriction[1] = r;
    }
    /*! Returns current method for finding contact points
        \return current method
     */
    inline p2d::FindContactPoints * find() const { return m_find; }
    /*! Sets method for finding contact points
        \param[in] find new method for finding contact points
     */
    inline void setFind(p2d::FindContactPoints * find ) 
    { 
        delete m_find; 
        m_find = find;
    }
    /*! Returns  time of impact
     *  \return time of impact
     */
    inline double toi() const { return m_toi; }
    /*! Returns  corrected time of impact
     *  \return time of impact
     */
    inline double correctedTOI() const { return (m_toi < 0.0) ? 0.0 : m_toi; }
    /*! A solver, for bodies
        \param[in] b1 first body
        \param[in] b2 second body
        \return true if bouncing was successfull and there were no errors
     */
    bool bounce(p2d::Body * b1, p2d::Body * b2);
    /*! Logs error, when unable to find contact points
        \param[in] m message
     */
    virtual void logFCPError(const char * m);
    /*! Enables debug for solver
     */
    inline void enableDebug() { m_debug = true; }
    /*! Dumps all collision data to string
        \return dump of all collected data
     */
    std::string dump() const;
    /*! Toggles inelastic collisions
        \param[in] b inelastic collisions
     */
    void toggleInelasticCollisions(bool b);
    /*! Returns whether inelastic collisions are enabled
        \return whether they are enabled
     */
    bool isEnabledInelasticCollisions() const;
    /*! Sets whether inelastic collision types are enabled
        \param[in] type type of collisions
     */
    void setInelasticCollisionType(sad::p2d::BounceSolver::InelasticCollisionType type);
    /*! Sets whether inelastic collision types are enabled. Used for scripting
        \param[in] type type of collisions
     */
    void setInelasticCollisionTypeAsUnsignedInt(unsigned int type);
    /*! Returns inelastic collision types
        \return inelastic collision types
     */
    sad::p2d::BounceSolver::InelasticCollisionType inelasticCollisionType() const;
    /*! Returns inelastic collision types as unsigned int
        \return inelastic collision types
     */
    unsigned int inelasticCollisionTypeAsUnsignedInt() const;
    /*! Sets recursion limit for solving collisions
        \param[in] limit a limit for recursion
     */
    void setRecursionLimit(size_t limit);
    /*! Returns recursion limit fo solving collisions
        \return[in] recursion limit
     */
    size_t recursionLimit() const;
    /*! Sets collision precision step for solver
        \param[in] step a step value
     */
    void setCollisionPrecisionStep(double step);
    /*! Returns a collision precision step
        \return a precision step
     */
    double collisionPrecisionStep() const;
    /*! Sets max solver iterations for solver for inelastic collisions
        \param[in] value a value
     */
    void setMaxSolverIterations(size_t value);
    /*! Returns max solver iterations for solver for inelastic collisions
        \return a value
     */
    size_t maxSolverIterations() const;
    /*! Toggles flag value for ignoring contact points. Please, note, that this only
        works for inelastic collision mode and could work only for it. Elastic
        collisions ignore this flag and won't do work, if cannot determine contact points,
        since solver needs them to determine normal and tangential part of speeds.
        \param[in] value new flag value
     */
    void toggleIgnoreContactPoints(bool value);
    /*! Returns flag value for ignoring contact points. Please, note, that this only
        works for inelastic collision mode and could work only for it. Elastic
        collisions ignore this flag and won't do work, if cannot determine contact points,
        since solver needs them to determine normal and tangential part of speeds.
        \return a flag value
     */
    bool getIgnoreContactPointsFlag() const;
    /*! Toggles flag value for ignoring negative time of impact. Please, note, that enabling this
        can and will cause some strange artifacts, in case, when we went too deep into
        other body, so use it wisely. It could sometimes solve issues, when spawned body
        is already in other body
        \param value new flag value
     */
    void toggleIgnoreNegativeTOI(bool value);
    /*! Returns flag value for ignoring negative time of impact. Please, note, that enabling this
        can and will cause some strange artifacts, in case, when we went too deep into
        other body, so use it wisely. It could sometimes solve issues, when spawned body
        is already in other body
        \return a flag velu
     */
    bool getIgnoreNegativeTOIFlag() const;
protected:
    p2d::FindContactPoints * m_find;  //!< Current algorithm for finding a contact poinnts
    p2d::Body * m_first;   //!< First body to test against
    p2d::Body * m_second;  //!< Second body to test against
    
    p2d::Vector m_av1;   //!< An approximated speed for first body
    p2d::Vector m_av2;   //!< An approximated speed for second body
    
    sad::Maybe<sad::p2d::PointsPair> m_contact; //!< A contact point if set

    p2d::Vector m_force_moment[2]; //!< A force moment to data 

    double      m_toi;  //!< Current time of impact

    double      m_restitution_coefficient;  //!< A restitution coefficient
    double      m_rotationfriction[2];   //!< A tangential friction, which is applied to rotation
        
    bool        m_debug; //!< Whether debug logging is enabled
    /*! A special flag, which is set to false if bodies has two points of collision
        If it's set to false - we should not count rotation
     */
    bool        m_shouldperformrotationfriction;
    /*! A special flag, that makes all collision inelastic, thus disabling changing velocity
        for solver
     */
    bool        m_inelastic_collisions;
    /*! An inelastic collision type, that turns those types on or off
     */
    sad::p2d::BounceSolver::InelasticCollisionType m_inelastic_collision_type;
    /*! A recursion limit for solving recursive dependencies
     */
    size_t    m_recursion_limit;
    /*! An inner recursion counter for avoiding stack overflow
     */
    size_t    m_recursion_counter;
    /*! A collision precision step
     */
    double    m_collision_precision_step;
    /*! A maximal solver iterations for inelastic collisions
     */
    size_t    m_max_solver_iterations;
    /*! Flag, that indicates, that solver should ignore contact points.
     */
    bool m_ignore_no_contact_points;
    /*! Flag, that indicates, that solver should ignore negative time of impact
     */
    bool m_ignore_negative_toi;
    /*! Inelastic bounce with fixed second body, where first body sticks to second
        \param[in] b1 first body
        \param[in] b2 second body
        \return true if it was successfully computed
     */
    bool inelasticBounceWithFixedSecondBody(sad::p2d::Body* b1, sad::p2d::Body* b2);
    /*! Returns list of bodies to recompute collisions with
        \param[in] position a list of positions
        \return
     */
    sad::Vector<sad::p2d::Body*> getBodiesToRecomputeCollisionsWith(size_t position) const;
    /*! Reshedules first bodies position after inelastic bounce
        \param[in] insert_position a position for inserted position
     */
    void resheduleFistBodyPositionAfterInElasticBounce(size_t insert_position) const;
    /*! Inserts collision data into first body's collision list
        \param[in] data a data for first body
        \return a position where it was inserted
     */
    size_t insertDataIntoCollisionList(const sad::p2d::Body::CollisionData& data) const;
    /*! Fixes local inelastic collision for first body
        \param[in] task solver task
        \return a collision data for solver
     */
    sad::p2d::Body::CollisionData fixLocalInelasticCollision(const sad::p2d::BounceSolver::SolverTask& task) const;
    /*! Tries to find basic task for inelastic bounce solving
        \return basic task if can be found
     */
    sad::Maybe<sad::p2d::BounceSolver::SolverTask> findBasicTaskForInelasticBounce() const;
    /*! Tries to find basic task for collision data
        \param[in] data a local data
        \param[in] first_shape shape for first object for collision
        \param[out] solver_task solver task a solver task
     */
    void tryFindBasicTaskForInelasticBounceForData(
        sad::p2d::Body::CollisionData& data,
        sad::p2d::CollisionShape*  first_shape,
        sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
    ) const;
    /*! Tries to find basic task for collision data
        \param[in] data a local data
        \param[in] first_shape shape for first object for collision
        \param[in] second_data a data for second solver
        \param[out] solver_task solver task a solver task
     */
    void tryFindBasicTaskForInelasticBounceForData(
        sad::p2d::Body::CollisionData& data,
        sad::p2d::CollisionShape*  first_shape,

        sad::p2d::Body::CollisionData& second_data,
        sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
    ) const;
    /*! Tries to find basic task for inelastic bounce with collisions
        \param[in] data a local data
        \param[in] first_shape a shape for first object
        \param[out] solver_task solver task a solver task
     */
    void tryFindBasicTaskForInelasticBounceWithCollisions(
        sad::p2d::Body::CollisionData& data,
        sad::p2d::CollisionShape*  first_shape,
        sad::Maybe<sad::p2d::BounceSolver::SolverTask>& solver_task
    ) const;
    /*! Returns true if already handled inelastic collision between two bodies
        \param[in] first first body
        \param[in] second second body
        \return true if second body is in list of collisions of first body
     */
    static bool isAlreadyHandledInelasticCollisionBetween(sad::p2d::Body* first, sad::p2d::Body* second);
    /*! Bounces with normal data
        \param[in] b1 first body
        \param[in] b2 second body
        \return true
     */
    bool bounceNormal(sad::p2d::Body* b1, sad::p2d::Body* b2);
    /*! Performs bouncing off for an object with a solver
        \param[in] pairs a set of pairs of collision points for time of impact
     */ 
    void performBouncing(const p2d::SetOfPointsPair & pairs);

    /*! Approximately solves time of impact and finds contact points for two object
        \param[in] first a first body
        \param[in] av1 an average velocity for first body
        \param[in] second a second body
        \param[in] av2 an average velocity for second body
        \param[out] pairs  a set of pairs of collision points for time of impact
     */
    void solveTOIFCP(
        sad::p2d::CollisionShape* first,
        const sad::p2d::Vector& av1,
        sad::p2d::CollisionShape* second,
        const sad::p2d::Vector& av2,
        sad::p2d::SetOfPointsPair& pairs
    );
    /*! Approximately solves time of impact and finds contact points for two object
        \param[out] pairs a set of pairs of collision points for time of impact
     */
    void solveTOIFCP(p2d::SetOfPointsPair & pairs);
    /*! Resolves bouncing for first body
        \param[in] b1 first body
        \param[in,out] n1 normal part of first velocity
        \param[in] b2 second body
        \param[in,out] n2 normal part of second velocity
        \param[in] pairs a normal pairs
     */
    void resolveNormalSpeed(sad::p2d::Body* b1, sad::p2d::Vector& n1, sad::p2d::Body* b2, sad::p2d::Vector & n2, const sad::p2d::SetOfPointsPair& pairs) const;
    /*! Resets coefficients for bouncing
     */
    void resetCoefficients();
    /*! Tries to resolve friction of rotation for a body, changing it's rotation speed
        \param[in] b this body
        \param[in] t a tangential speed for body
        \param[in] ni a normal speed for body
        \param[in] index for a body
        \param[in] pivot of force
     */
    inline void tryResolveFriction(p2d::Body * b, const p2d::Vector & t, const p2d::Vector & ni, int index, double pivot);

};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::BounceSolver)
