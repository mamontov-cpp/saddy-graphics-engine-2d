/*! \file bouncesolver.h
    A solver for bouncing bodies, when they are colliding
 */
#pragma once
#include "body.h"
#include "findcontactpoints.h"

#include "../object.h"

namespace sad
{

namespace p2d
{
/*! A bounce solver allows to easily solve problems with 
    bouncing of various objects. 
    
    Due to hard problems with fastly changed forces it is HIGHLY recommended, that
    objects must be checked for collision with p2d::CollisionTest, before calling a
    bounce. 
 */
class BounceSolver: public sad::Object
{
SAD_OBJECT
public:
    /*! Constructs new solver
     */
    BounceSolver();
    /*! Removes allocated algorithm for finding data
     */
    ~BounceSolver();
    /*! Pushes resilience coefficient for two objects. A coefficients are restored to normal,
        after performing bounce.
        \param[in] r coefficient
        \param[in] index an index
     */
    inline void pushResilienceCoefficient(double r, int index)
    {
        assert(index == 1 || index == 2 );
        m_resilience[index - 1] = r;
    }
    /*! Pushes resilience coefficient for two objects. A coefficients are restored to normal,
        after performing bounce.
        \param[in] r coefficient
     */
    inline void pushResilienceCoefficient(double r)
    {
        m_resilience[0] = r;
        m_resilience[1] = r;
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
    inline p2d::FindContactPoints * find() { return m_find; }
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
     */
    void bounce(p2d::Body * b1, p2d::Body * b2);
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
    std::string dump();
protected:
    p2d::FindContactPoints * m_find;  //!< Current algorithm for finding a contact poinnts
    p2d::Body * m_first;   //!< First body to test against
    p2d::Body * m_second;  //!< Second body to test against
    
    p2d::Vector m_av1;   //!< An approximated speed for first body
    p2d::Vector m_av2;   //!< An approximated speed for second body
    
    sad::Maybe<sad::p2d::PointsPair> m_contact; //!< A contact point if set

    p2d::Vector m_force_moment[2]; //!< A force moment to data 

    double      m_toi;  //!< Current time of impact

    double      m_resilience[2]; //!< A resilience coefficients for bodies
    double      m_rotationfriction[2];   //!< A tangential friction, which is applied to rotation
        
    bool        m_debug; //!< Whether debug logging is enabled
    /*! A special flag, which is set to false if bodies has two points of collision
        If it's set to false - we should not count rotation
     */
    bool        m_shouldperformrotationfriction;
    /*! Performs bouncing off for an object with a solver
        \param[in] pairs a set of pairs of collision points for time of impact
     */ 
    void performBouncing(const p2d::SetOfPointsPair & pairs);
    /*! Approximately solves time of impact and finds contact points for two object
        \param[out] pairs a set of pairs of collision points for time of impact
     */
    void solveTOIFCP(p2d::SetOfPointsPair & pairs);
    /*! Resolves bouncing for first body
        \param[in] b1 first body
        \param[in,out] n1 normal part of first velocity
        \param[in] b2 second body
        \param[in,out] n2 normal part of second velocity
        \param[in] index index for coefficients
     */
    void resolveNormalSpeed(p2d::Body * b1, p2d::Vector & n1, p2d::Body * b2, const p2d::Vector & n2, int index);
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
