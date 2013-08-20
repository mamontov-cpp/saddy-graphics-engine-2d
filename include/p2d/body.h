/*! \file body.h
	\author HiddenSeeker

    Describes a body object
 */
#include "../primitives/object.h"
#include "../templates/hstring.h"
#include "collisionshape.h"
#include "force.h"
#include "ghostoptions.h"
#include "tangentialforce.h"
#include "movement.h"
#pragma once

namespace p2d
{
class World;
/*! Describes a body in physics engine
 */
class Body
{
private:
	/*! A weight of specific body
	 */
	p2d::Weight  *   m_weight;
	/*! A ghost options
	 */ 
	p2d::GhostOptionsFlow  * m_options;
	/*! A force, acting on center of body
	 */
	p2d::ForceFlow   *   m_central_force;
	/*! A tangentianl force
	 */
	p2d::TangentialForceFlow   *   m_tangential_force;
	/*! A current shape of data
	 */
	CollisionShape * m_current;
	/*! A temporary shape for returning shape at specific time.
		Selected and freed here.
	 */
	CollisionShape * m_temporary; 
public:
	/*! Returns an inner user-defined object
		\return inner user-defined object
	 */
	virtual sad::Object * userObject() const;
	/*! Returns a type of user-defined object
		\return type of object
	 */
	virtual const hst::string & userType() const;
	/*! Returns a position of body at specified time.
		\param[in] time time, when position is needed
		\return what shape, body had at specified time
	 */
	virtual p2d::CollisionShape & at(double time) const;
	virtual ~Body();
};

}

