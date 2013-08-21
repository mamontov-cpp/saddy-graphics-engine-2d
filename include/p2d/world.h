/*! \file world.h
	\author HiddenSeeker

	Defines world simulation as set of items and callbacks
 */
#pragma once
#include "circletohulltransformer.h"
#include "collisiondetector.h"
#include "simplecollisiondetector.h"
#include "broadcollisiondetector.h"
#include "multisamplingcollisiondetector.h"
#include "collisionhandler.h"
#include "../templates/hhash.hpp"
#include "../templates/hpair.hpp"

namespace p2d
{
class Body;
/*! A world is a set if simulated items, and callbacks used to define
	behaviour between them
 */
class World
{
 private:
	 typedef hst::pair<hst::string, hst::string> type_pair_t;
	 typedef hst::pair<type_pair_t, p2d::BasicCollisionHandler *> types_with_handler_t;
	 typedef  hst::hash<p2d::Body *, hst::vector<hst::string> > bodies_to_types_t;
	 /*! Current time step
	  */
	 double m_time_step;
	 /*! A splitted time step
	  */
	 hst::Maybe<double> m_splitted_time_step;
	 /*! A common transformer for all shapes
	  */
	 p2d::CircleToHullTransformer * m_transformer;
	 /*! A collision dispatcher for testing an items for collision
	  */
	 p2d::CollisionDetector * m_detector;
	 /*! A callbacks, with related types
	  */
	 hst::vector<types_with_handler_t>  m_callbacks;
	 /*! Bodies by groups
	  */
	 hst::hash<hst::string, hst::vector<p2d::Body *> > m_groups;
	 /*! All bodies for checking all information
	  */
	 bodies_to_types_t m_allbodies;
 public:
	 /*! Creates world with default transformer
	  */
	 World();
	 /*! Destroys world
	  */
	 ~World();
	 /*! Returns a transformer for all circles
		 \return a transformer for all circles
	  */
	 p2d::CircleToHullTransformer * transformer();
	 /*! Sets new detector for a world
		 \param[in] d detector
	  */
	 void setDetector(p2d::CollisionDetector * d);
	 /*! Set transformer for a circles
		 \param[in] t transformer
	  */
	 void setTransformer(p2d::CircleToHullTransformer * t);
	 /*! Adds new collision handler with specified callbacks
		 \param[in] p specified handler
		 \return created handler
	  */
	 template<typename T1, typename T2>
	 p2d::BasicCollisionHandler *
	 addHandler( void (*p)(const p2d::CollisionEvent<T1, T2> &))
	 {
		 p2d::BasicCollisionHandler * h = 
			 new p2d::FunctionCollisionHandler<T1, T2>(p);
		 hst::string t1 = T1::globalMetaData()->name();
		 hst::string t2 = T2::globalMetaData()->name();
		 if (m_groups.contains(t1) == false)
		 {
			 m_groups.insert(t1, hst::vector<p2d::Body *>());
		 }
		 if (m_groups.contains(t2) == false)
		 {
			 m_groups.insert(t2, hst::vector<p2d::Body *>());
		 }
		 type_pair_t tp(t1, t2);
		 types_with_handler_t twh(tp, h);
		 m_callbacks << twh;
		 return h;
	 }
	 /*! Adds new handler
		 \param[in] p new handler
		 \return created inner handler
	  */
	 template<typename _Class, typename T1, typename T2>
	 p2d::BasicCollisionHandler *
		 addHandler( void (_Class::*p)(const p2d::CollisionEvent<T1, T2> &))
	 {
		 p2d::BasicCollisionHandler * h = 
			 new p2d::MethodCollisionHandler<_Class,T1, T2>(p);
		 hst::string t1 = T1::globalMetaData()->name();
		 hst::string t2 = T2::globalMetaData()->name();
		 if (m_groups.contains(t1) == false)
		 {
			 m_groups.insert(t1, hst::vector<p2d::Body *>());
		 }
		 if (m_groups.contains(t2) == false)
		 {
			 m_groups.insert(t2, hst::vector<p2d::Body *>());
		 }
		 type_pair_t tp(t1, t2);
		 types_with_handler_t twh(tp, h);
		 m_callbacks << twh;
		 return h;
	 }
	 /*! Removes handler from a world
		 \param[in] h registered handler
	  */
	 void removeHandler(p2d::BasicCollisionHandler * h);
	 /*! Adds new body in system
		 \param[in] b body
	  */
	 void addBody(p2d::Body * b);
	 /*! Removes body from system. Frees memory at end
		  \param[in] b body
	  */
	 void removeBody(p2d::Body * b);
	 /*! When called inside of step() method, makes a world integrate
		 velocities and positions to specified time, and restart step, not
		 stepping through ghostoptions and forces
		 \param[in] time time of item
	  */
	 void splitTimeStepAt(double time);
	 /*! Returns current time step for a world
		 \return a time step for a world
	  */
	 double timeStep() const;
};

}

