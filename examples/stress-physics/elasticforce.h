/*! \file elasticforce.h
	\author HiddenSeeker

	Describes an elastic force acting toward first object to second
 */
#include <p2d/force.h>
#include <p2d/body.h>

/*! An elastic force
 */
class ElasticForce: public p2d::TangentialForce
{
 private:
	 p2d::Body * m_first;
	 p2d::Body * m_second;
	 p2d::Point  m_defaultdistance;
 public:
	/*! An elastic force
		\param[in] first a first body
		\param[in] second a second body
	 */
	ElasticForce(p2d::Body * first, p2d::Body * second);

	virtual const p2d::Vector & value() const;
};
