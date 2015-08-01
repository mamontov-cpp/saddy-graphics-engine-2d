/*! \file elasticforce.h
	

	Describes an elastic force acting toward first object to second
 */
#pragma once
#include "force.h"
#include "body.h"

namespace sad
{

namespace p2d
{
/*! An elastic force is implemented, using formulo
	- k * dx + b * v * v, where dx is difference, between distance of
	centers of both bodies and default distance, which by default is constructed,
	when creating force, v is a current speed of body, k is coefficient of elasticity 
	(20) by default, b is a resistance coefficient, used to attenuate vibrations.

	Note, that this class describes a force, acting on SECOND object. If you want object
	to have elasticity between both of theme, you must add this force to both objects.
 */
class ElasticForce: public p2d::TangentialForce
{
public:
	/*! Creates new elastic force with default distance, taken from distance between two
		bodies
		\param[in] first a first body, linked to current body
		\param[in] second a current body, which force is acting towards
		\param[in] elasticity an elasticity coefficient
		\param[in] resistance a resistant coefficient
	 */
	ElasticForce(p2d::Body * first, p2d::Body * second, double elasticity = 20, double resistance = 0.005);
	/*! Returns a value of elastic force, computing formulo - k * dx + b * v
		\return a value for the force
	 */
	virtual const p2d::Vector & value() const;
	/*! Sets a default distance, which must persist between two bodies
		\param[in] dist a distance
	 */
	inline void setDefaultDistance(double dist)  { m_defaultdistance = dist; }
	/*! Returns a default distance, which should persist between two bodies
		\return a distance
	 */
	inline double defaultDistance() const { return m_defaultdistance; }
	/*! Sets elasticity coefficient for force
		\param[in] elasticity coefficient
	 */
	inline void setElasticity(double elasticity) { m_elasticity = -elasticity; }
	/*! Returns elasticity coefficient
		\return elasticity coefficient
	 */
	inline double elasticity() const { return -m_elasticity; } 
	/*! Sets a resistance coefficient
		\param[in] resistance a resistance coefficient
	 */
	inline void setResistance(double resistance) { m_resistance = -resistance; }
	/*! Returns a resistance coefficient
		\return resistance coefficient
	 */
	inline double resistance() const { return -m_resistance; }
private:
	 p2d::Body * m_first;          //!< A first body, which is linked to current body
	 p2d::Body * m_second;         //!< A second body, which is force is acting towards
	 double  m_defaultdistance;    //!< An initial distance between bodies, which must persist

	 double  m_elasticity; //!< An elasticity coefficient, used to describe elasticity
	 double  m_resistance; //!< A resistant coeffficient
};

}

}
