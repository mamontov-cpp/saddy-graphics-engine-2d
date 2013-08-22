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
 protected:
	 typedef hst::pair<hst::string, hst::string> type_pair_t;
	 typedef hst::pair<type_pair_t, p2d::BasicCollisionHandler *> types_with_handler_t;
	 typedef hst::hash<p2d::Body *, hst::vector<hst::string> > bodies_to_types_t;
	 typedef hst::pair<p2d::BasicCollisionEvent, p2d::BasicCollisionHandler*> reaction_t;
	 typedef hst::vector<reaction_t> reactions_t;
public:
	 /*! Compares two reactions, returns true if time of impact of first is lesser
		 than second
		 \param[in] r1 first reaction
		 \param[in] r2 second reaction
		 \return whether less
	  */
	 static bool compare(const reaction_t & r1, const reaction_t & r2);
protected:
	 /*! This flag is set to true, when world is stepping
	  */
	 bool   m_stepping;
	 /*! An added bodies while stepping
	  */
	 hst::vector<p2d::Body *> m_added_queue;
	 /*! A removed bodies while stepping
	  */
	 hst::vector<p2d::Body *> m_removed_queue;
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
	 /*! Steps all body options, like ghost options and body
	  */
	 virtual void stepDiscreteChangingValues();
	 /*! Steps a position and velocities
		 \param[in] time a time step size
	  */
	 virtual void stepPositionsAndVelocities(double time);
	 /*! Executes a reactions for a world
		 \param[in] reactions found reactions
	  */ 
	 virtual void executeCallbacks(reactions_t & reactions);
	 /*! Sorts callbacks. Ascending order of time of impact
		 \param[in] reactions found reactions
	  */
	 virtual void sortCallbacks(reactions_t & reactions);
	 /*! Find specific collision events and populates reactions
		 \param[in] reactions a reactions
	  */
	 virtual void findEvents(reactions_t & reactions);
	 /*! Finds a specifif collision event and populates reaction
		 \param[in] reactions a reactions
		 \param[in] twh  types and handlers
	  */
	 virtual void findEvent(reactions_t & reactions, const types_with_handler_t & twh);
	 /*! Finds and executes callbacks
	  */
	 virtual void findAndExecuteCollisionCallbacks();
	 /*! Adds new handler
		 \param[in] h handler
		 \param[in] t1 first type
		 \param[in] t2 second type
	  */
	 virtual void addHandler(
		 p2d::BasicCollisionHandler * h, 
		 const hst::string & t1, 
		 const hst::string & t2
	  );
	 /*! Peforms all insertions and removal of queued bodies
	  */
	 void performQueuedActions();
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
		 this->addHandler(h, t1, t2);
		 return h;
	 }
	 /*! Adds new handler	
		 \param[in] p handler
		 \return created handler
	  */
	 p2d::BasicCollisionHandler *
	 addHandler( void (*p)(const p2d::BasicCollisionEvent &));
	 /*! Adds new handler
	     \param[in] o binded object
		 \param[in] p new handler
		 \return created inner handler
	  */
	 template<typename _Class, typename T1, typename T2>
	 p2d::BasicCollisionHandler *
		 addHandler( _Class * o, void (_Class::*p)(const p2d::CollisionEvent<T1, T2> &))
	 {
		 p2d::BasicCollisionHandler * h = 
			 new p2d::MethodCollisionHandler<_Class,T1, T2>(o,p);
		 hst::string t1 = T1::globalMetaData()->name();
		 hst::string t2 = T2::globalMetaData()->name();
		 this->addHandler(h, t1, t2);
		 return h;
	 }
	 /*! Adds new handler
		 \param[in] o class
		 \param[in] p new handler
		 \return created inner handler
	  */
	 template<typename _Class>
	 p2d::BasicCollisionHandler *
		 addHandler(_Class * o, void (_Class::*p)(const p2d::BasicCollisionEvent &))
	 {
		 p2d::BasicCollisionHandler * h = 
			 new p2d::MethodCollisionHandler<_Class,p2d::Body, p2d::Body>(o, p);
		 hst::string b = "p2d::Body";
		 this->addHandler(h, b, b);
		 return h;
	 }
	 /*! Removes handler from a world
		 \param[in] h registered handler
	  */
	 void removeHandler(p2d::BasicCollisionHandler * h);
	 /*! Adds new body in system
		 \param[in] b body
	  */
	 virtual void addBody(p2d::Body * b);
	 /*! Removes body from system. Frees memory at end
		  \param[in] b body
	  */
	 virtual void removeBody(p2d::Body * b);
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
	 /*! Steps a world by specified time
		 \param[in] time a size of time step
	  */
	 void step(double time);
	 /*! Clears a world
	  */ 
	 void clear();
};

}

