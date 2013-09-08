#include "bouncesolver.h"
#include <log/log.h>

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
	m_first = b1;
	m_second = b2;
	p2d::SetOfPointsPair pairs;
	this->solveTOIFCP(pairs);
	if (pairs.size() > 0 &&  (m_toi > 0 || is_fuzzy_zero(m_toi, COLLISION_PRECISION)))
	{
		this->performBouncing(pairs);
	}
	else
	{
		logFCPError();
	}
}


void BounceSolver::solveTOIFCP(p2d::SetOfPointsPair & pairs)
{
	m_av1 = m_first->averageChangeIndependentTangentialVelocity();
	m_av2 = m_second->averageChangeIndependentTangentialVelocity();

	pairs = m_find->invoke(m_first->currentShape(),   m_av1,
						   m_second->currentShape(),  m_av2
						   );
	if (pairs.size() > 0)
	{
		// Compute time of impact
		double x1 = pairs[0].p1().x();
		double y1 = pairs[0].p1().y();
		double x2 = pairs[0].p2().x();
		double y2 = pairs[0].p2().y();

		double avx1 = m_av1.x();
		double avy1 = m_av1.y();
		double avx2 = m_av2.x();
		double avy2 = m_av2.y();

		double time = 0;
		if (non_fuzzy_zero(avx2 - avx1))
		{
			time = (x1 - x2) / (avx2 - avx1);
		}
		else
		{
			time = (y1 - y2) / (avy2 - avy1);
		}
				
		time -= COLLISION_PRECISION;
		m_toi = time;
		SL_DEBUG(fmt::Format("{0}") << m_toi);
	}
}

/*! Implement solving for bouncing
	\param[in] pairs pairs of contact points
 */
void BounceSolver::performBouncing(const p2d::SetOfPointsPair & pairs)
{
	//TODO: here is performed a simple bouncing off for an objects. 
	//      But there can be friction and|or rotation
	//      Speeds, how they are changing?

	double m1 = m_first->weight().value();
	double m2 = m_second->weight().value();

	p2d::Point normal1; 
	m_first->currentShape()->normalToPointOnSurface(pairs[0].p1(), normal1);
	

	p2d::Point normal2;
	m_second->currentShape()->normalToPointOnSurface(pairs[0].p2(), normal2);

	p2d::Vector v1 = m_first->tangentialVelocityAt(m_toi);
	double project1 = p2d::scalar(v1, normal1);
	p2d::Vector normalPart1 = normal1;
	normalPart1 *= project1;
	p2d::Vector cachedNormal1 = normalPart1;

	p2d::Vector tangentialPart1 = v1;
	tangentialPart1 -= normalPart1;

	p2d::Vector v2 = m_second->tangentialVelocityAt(m_toi);
	double project2 = p2d::scalar(v2, normal2);
	p2d::Vector normalPart2 = normal2;
	normalPart2 *= project2;
	p2d::Vector cachedNormal2 = normalPart2;

	p2d::Vector tangentialPart2 = v2;
	tangentialPart2 -= normalPart2;

	this->resolveNormalSpeed(m_first, normalPart1, m_second, cachedNormal2);		
	m_first->correctTangentialVelocity(normalPart1 + tangentialPart1);		
	m_first->correctPosition(m_av1 * m_toi);
		
	
	this->resolveNormalSpeed(m_second, normalPart2, m_first, cachedNormal1);		
	m_second->correctTangentialVelocity(normalPart2 + tangentialPart2);		
	m_second->correctPosition(m_av2 * m_toi);		
}


void BounceSolver::logFCPError()
{
	hst::string tpl = "Cannot find a contact points. Performing an object dump\n";
	tpl <<  "1st body: \n{0}\n";
	tpl <<  "velocity ({1},{2})\n";
	tpl <<  "2nd body: \n{3}\n";
	tpl <<  "velocity ({4},{5})\nTOI: {6} \n";
		
	p2d::Point center1 = m_first->currentShape()->center();

	p2d::Point center2 = m_second->currentShape()->center();

	SL_CRITICAL(fmt::Format(tpl) << m_first->currentShape()->dump()
							     << m_av1.x() << m_av1.y()
								 << m_second->currentShape()->dump()
								 << m_av2.x() << m_av2.y()
								 << m_toi
			   );
}

void BounceSolver::resolveNormalSpeed(p2d::Body * b1, p2d::Vector & n1, p2d::Body * b2, const p2d::Vector & n2)
{
	p2d::Vector vn1 = n1;
	n1 *= -1;
	if (b1->weight().isInfinite() == false && b2->weight().isInfinite() == false
		|| b1->weight().isInfinite() == true && b2->weight().isInfinite() == true)
	{
		double m1 = b1->weight().value();
		double m2 = b2->weight().value();
		// If both object has infinite weight, we resolve them as 
		// equal weight objects
		if (b1->weight().isInfinite())
		{
			m1 = 1;
			m2 = 1;
		}
		n1 += ((vn1 * m1 + n2 * m2) / (m1 + m2)) * 2 ;
		return;
	}
	// If only first object has infinite weight
	// his speed won't change
	if (b1->weight().isInfinite())
	{
		return;
	}
	if (b2->weight().isInfinite())
	{
		n1 =  (n2 * 2 - vn1);
		return;
	}
}

