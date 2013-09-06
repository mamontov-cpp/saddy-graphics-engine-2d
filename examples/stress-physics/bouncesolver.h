/*! \file bouncesolver.h
	A solver for bouncing bodies, when they are colliding
 */
#include <p2d/body.h>
#include <p2d/findcontactpoints.h>
#pragma once

/*! A bounce solver allows to easily solve problems with 
	bouncing of various objects. 
	
	Due to hard problems with force detection it is HIGHLY recommended, that
	objects must be checked against with p2d::CollisionTest, before calling a
	bounce. 

	This must be performed, because BounceSolver CA
 */
class BounceSolver
{
protected:
	p2d::FindContactPoints * m_find;  //!< Current algorithm for finding a contact poinnts
	p2d::Body * m_first;   //!< First body to test against
	p2d::Body * m_second;  //!< Second body to test against
	
	p2d::Vector m_av1;   //!< An approximated speeed for first body
	p2d::Vector m_av2;   //!< An approximated speed for second body
	
	/*! Performs bouncing off a solver
		\param[in] pairs a pairs
	 */ 
	void performBouncing(const p2d::SetOfPointsPair & pairs);
	/*! Approximately solves time of impact and finds contact points
	 */
	void solveTOIFCP(const p2d::SetOfPointsPair & pairs);	
public:
	/*! Constructs new solver
	 */
	BounceSolver();
	/*! A solver for bounces
	 */
	~BounceSolver();
	/*! A solver, for bodies
		\param[in] b1 first body
		\param[in] b2 second body
	 */
	void bounce(p2d::Body * b1, p2d::Body * b2);
};