/*! \file bouncesolver.h
	A solver for bouncing bodies, when they are colliding
 */
#include <p2d/body.h>
#include <p2d/findcontactpoints.h>
#pragma once

class BounceSolver
{
protected:
	p2d::FindContactPoints * m_find;
	p2d::Body * m_first;
	p2d::Body * m_second;
	
	/*! Performs bouncing off a solver
		\param[in] pairs a pairs
	 */ 
	void performBouncing(const p2d::SetOfPointsPair & pairs);
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