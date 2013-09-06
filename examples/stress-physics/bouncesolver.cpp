#include "bouncesolver.h"


BounceSolver::BounceSolver()
{
	m_find = new p2d::FindContactPoints();
	m_first = NULL;
	m_second = NULL;
}

BounceSolver::~BounceSolver()
{	
	delete m_find;
}


void BounceSolver::bounce(p2d::Body * b1, p2d::Body * b2)
{
	m_first = NULL;
	m_second = NULL;
}


void BounceSolver::performBouncing(const p2d::SetOfPointsPair & pairs)
{

}