/*! \file bouncesolver.h
	A solver for bouncing bodies, when they are colliding
 */
#include <p2d/body.h>
#include <p2d/findcontactpoints.h>
#pragma once

/*! A bounce solver allows to easily solve problems with 
	bouncing of various objects. 
	
	Due to hard problems with fastly changed forces it is HIGHLY recommended, that
	objects must be checked for collision with p2d::CollisionTest, before calling a
	bounce. 
 */
class BounceSolver
{
protected:
	p2d::FindContactPoints * m_find;  //!< Current algorithm for finding a contact poinnts
	p2d::Body * m_first;   //!< First body to test against
	p2d::Body * m_second;  //!< Second body to test against
	
	p2d::Vector m_av1;   //!< An approximated speed for first body
	p2d::Vector m_av2;   //!< An approximated speed for second body
	
	double      m_toi;  //!< Current time of impact

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
	 */
	void resolveNormalSpeed(p2d::Body * b1, p2d::Vector & n1, p2d::Body * b2, const p2d::Vector & n2);
public:
	/*! Constructs new solver
	 */
	BounceSolver();
	/*! Removes allocated algorithm for finding data
	 */
	~BounceSolver();
	/*! Returns current method for finding contact points
		\return current method
	 */
	inline p2d::FindContactPoints * find() { return m_find; }
	/*! Sets method for finding contact points
		\param[in] new method
	 */
	inline void setFind(p2d::FindContactPoints * find ) 
	{ 
		delete m_find; 
		m_find = find;
	}
	/*! A solver, for bodies
		\param[in] b1 first body
		\param[in] b2 second body
	 */
	void bounce(p2d::Body * b1, p2d::Body * b2);
	/*! Logs error, when unable to find contact points
	 */
	virtual void logFCPError();
};
