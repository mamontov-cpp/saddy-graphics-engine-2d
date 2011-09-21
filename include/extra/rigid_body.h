/*! \file   rigid_body.h
    \author HiddenSeeker

	Contains a definition of simple rigid body
*/
#include "../primitives/hrect.h"
#include "../sprite.h"
#pragma once
/*! Declares a bounding box for RigidBody
*/
typedef hst::rect< ::s3d::point> BoundingBox;
/*! Declares a vector, which only two dimensions are used
*/
typedef ::s3d::point vector32;
/*! \class RigidBody
	Determines a rigid body for physics simulations
*/
class RigidBody
{
 protected:
	      BoundingBox               m_new_point;    //!< New point of moving object
		  BoundingBox               m_old_point;    //!< Old point
		  float                     m_interval;     //!< Time interval between moving
		  ::s3d::point              m_velocity;     //!< Declares a velocity
		  ::s3d::point              m_force;        //!< Force acting power
 public:
		 /*! Declares empty rigid body
		 */
	     inline RigidBody() { m_interval=0.0f;}
		 /*! Returns a new point
			 \return RigidBody::m_new_point
		 */
		 inline hst::rect< ::s3d::point>   & newPoint() { return m_new_point; }
		 /*! Returns a new point
			 \return RigidBody::m_new_point
		 */
		 inline const hst::rect< ::s3d::point>   & newPoint() const { return m_new_point; }
		 /*! Returns an old point
			 \return RigidBody::m_old_point
		 */
		 inline hst::rect< ::s3d::point>   & oldPoint() { return m_old_point; }
		 /*! Returns an old point
			 \return RigidBody::m_old_point
		 */
		 inline const hst::rect< ::s3d::point>   & oldPoint() const { return m_old_point; }
		 /*! Returns an interval
			 \return RigidBody::m_interval
		 */
		 inline float & interval() { return m_interval;}
		 /*! Returns an interval
			 \return RigidBody::m_interval
		 */
		 inline const float & interval()  const { return m_interval;}
		 /*! Returns a velocity
			 \return RigidBody::m_velocity
		 */
		 inline ::s3d::point & v() { return m_velocity;}
		 /*! Returns a velocity
			 \return RigidBody::m_velocity
		 */
		 inline const ::s3d::point & v() const { return m_velocity;}
		 /*! Returns a force
			 \return RigidBody::m_force
		 */
		 inline ::s3d::point & f() { return m_force;}
		 /*! Returns a force
			 \return RigidBody::m_force
		 */
		 inline const ::s3d::point & f() const { return m_force;}
		 /*! Destructor
		 */
	     virtual ~RigidBody();
};
/*! \class Collidable
	Declares a collidable sprite
*/
class Collidable: public RigidBody, public Sprite
{
 SAD_NODE
 public:
	     /*! Creates a collidable object with following parameters
			  \param[in] tex   texture
			  \param[in] rect  rectangle for sprite
			  \param[in] texrect rectangle for texture coordinates(in coordinate range, NOT in 0..1)
		 */
		 inline Collidable(
			               sad::Texture * tex, 
				           const hst::rect< ::s3d::point> & rect,
				           const hRectF  & texrect
						   ):RigidBody(),Sprite(tex,rect,hRectF(texrect[3],texrect[2],texrect[1],texrect[0]))
		 {}
		 /*! Renders as spite
		 */
		 virtual void render()=0;
		 /*! Removes an object from scene
		 */
		 virtual void die();
		 /*! Detects a collision between two collidable
			 \param[in] o other object
			 \return    object
		 */
		 bool collides(Collidable * o);
		 /*! Destructor
		 */
		 virtual ~Collidable();
};
/*! Tests two objects as rigid bodies
*/
bool testCollidables(void * b1,void * b2);
/*! Returns orthogonal vector for passed vector.
	Calculation uses an scalar multiplication properties to produce correct result.
	
	If zero-vector passed, it returns(0,1,0). 
	
	Otherwise it solves equation like x*v.x+y*v.y=0. y=-v.x/v.y. If v.y = 0 - (0,1,0)
	\param[in] v vector
	\return orthogonal vector
*/
vector32 ortho(const vector32 & v);
/*! Normalizes a vector
	\param[in,out] v vector
*/
inline void normalize(vector32 & v)
{
	if (v.x()!=0 && v.y()!=0)
	{ float modu=sqrtf(v.x()*v.x()+v.y()*v.y()); v.setX(v.x()/modu); v.setY(v.y()/modu); }
	else
	{ v.setX(1); v.setY(1);}
}
/*! Performs scalar multiplication between vectors
	\param[in] v1 vector1
	\param[in] v2 vector2
*/
inline float scalar(const vector32 & v1, const vector32 & v2) { return v1.x()*v2.x()+v1.y()*v2.y();}
/*! Tests, whether two bodies collides
	\param[in] old_1 old position of first body
	\param[in] new_1 new position of first body
	\param[in] old_2 old position of second body
	\param[in] new_2 new position of second body
	\return   true, if bodies collide.
*/
bool collides(const BoundingBox & old_1,const BoundingBox & new_1,
			  const BoundingBox & old_2,const BoundingBox & new_2
			  );
/*! Tests, whether body and line are colliding
	\param[in] old old position of body
	\param[in] nwp  new position of body
	\param[in] p1  first point of line
	\param[in] p2  second point of line
	\return true if colliding
*/
bool collides(const BoundingBox & old, const BoundingBox & nwp,
			  const ::s3d::point & p1, const ::s3d::point & p2 );
/*! Tests, whether body and line are colliding
	\param[in] b body
	\param[in] p1 first point of line
	\param[in] p2 second point of line
	\return true if colliding
*/ 
bool collides(const RigidBody & b,const ::s3d::point & p1, const ::s3d::point & p2);
/*! Moves box on distance
	\param[in,out] box moving box
	\param[in]     v   vector
*/
inline void move(BoundingBox & box, const vector32 & v)
{
  for (int i=0;i<4;i++) box[i]+=v;
}
/*! Returns  a collision time. This version thinks, that moving of second body is
    momentary, so you must use it if it don't moves or moves really fast.
	\param[in] b1 body, that collision time is being calculated. MUST have velocity and interval
	\param[in] b2 second body
	\param[out] pos position at this time
	\return    collision time
*/
float collision_time(const RigidBody & b1, const RigidBody & b2, BoundingBox & pos);
/*! Returns  a collision time. This version checks on moving second body 2
	\param[in] b1 body, that collision time is being calculated. MUST have velocity and interval
	\param[in] b2 second body. MUST have velocity and time, equal to body 2
	\param[out] pos1 position at this time of first body
	\param[out] pos2 position at this time of second body
	\return    collision time
*/
float collision_time2(const RigidBody & b1, const RigidBody & b2, BoundingBox & pos1,BoundingBox & pos2);
