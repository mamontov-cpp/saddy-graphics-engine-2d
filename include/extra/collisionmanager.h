/*! \file   collisionmanager.h
    \author HiddenSeeker
	
	This file contains a basic definition of collision primitives
	Definition of CollisionHandler and CollisionManager are placed here
*/
#include "collidable.h"
#include "collision.h"
#include "../templates/hhash.hpp"
#include "../templates/hpair.hpp"
#include <time.h>
#pragma once

/*! Identifier of collision group by id.
    ID - is a type identifier of node. It will be an id of sad::BasicNode
*/
typedef int CollisionGroupID;


class CollisionHandler
{
 private:
	     /*! Function, that will be called on collision. 
		     In first object will be a pointer to first group, on
			 second is a second group
		 */
	     void (*fun)(Collidable * o1, Collidable * o2);
		 /* Default constructor
		 */
	     CollisionHandler();
 public:
	     /*! Creates a handler
		     \param[in] f called function
		 */
		 CollisionHandler(void (*f)(Collidable * o1, Collidable * o2));
		 /*! Calls a handler
		     \param[in] o1 object of first group
			 \param[in] o2 object of second group
		 */
		 virtual void operator()(Collidable * o1,Collidable * o2);
		 /*  Destructor
		 */
		 virtual ~CollisionHandler();
};

/*! Class,that provides a manager of collision
    Use ::detect() to detect a collision and call handler.
	Firstly, you must add an object by add. You can safely  remove it
	any time, using delete
*/
class CollisionManager
{
 private:
	 /*! Groups of registered objects
	 */
	 static hst::hash<int,hst::vector<BoundingBox> >  m_boxes;
	 /*! Groups of registered objects
	 */
	 static hst::hash<int,hst::vector<Collidable *> >  groups;
	 /*! Delete list
	 */
	 static hst::vector< hst::pair<int,Collidable *> >          m_delete;
	 /*! Add list
	 */
	 static hst::vector< hst::triplet<int,BoundingBox,Collidable *> > m_add;
	 /*! Handlers
	 */
	 static hst::hash< hst::pair<int,int>,CollisionHandler * > m_handlers;

	 /*! Immediately adds an object
	 */
	 static void imAdd(int id, const BoundingBox & a,Collidable * b);
	 /*! Immediately deletes an object
	 */
	 static void imDelete(int id,Collidable * a); 
	 /*! Scans a group
	     \param[in] g1 group 1
		 \param[in] g2 group 2
		 \param[in] h handler
	 */
	 static void scanGroup(int g1,int g2,CollisionHandler & h);
 public:
	/*! Updates a rectangle for singletone object.
	    This object must have only ONE instance, or it will set other object rectangle
	*/
	static void updateSingle(Collidable * a);
	/*! Adds a collidable object
	*/
	static void add(Collidable * a);
	/*! Removes a collidable object
	*/
	static void remove(Collidable * b);
	/*! Flushes a collidable object
	*/
	static void flush();
	/*! Detects a collision
	*/
	static void detect();
	/*! Binds a new handler
	*/
	static void bind(CollisionGroupID id1, CollisionGroupID id2, CollisionHandler * h);
	/*! Unbinds a new handler
	*/
	static void unbind(CollisionGroupID id1,CollisionGroupID id2);
	/*! Flushes all handlers
	*/
	static void flushHandlers();
};

/*! Test frquency in clocks
*/
#define COLLISION_TEST_FREQUENCY 75
/*! Class, that tests a collision
    Add it to scene to test a collision  
*/
class CollisionTester: public sad::BasicNode  
{
 private:
	      clock_t m_lastclock;  //!< Last clock time
 public:
	    CollisionTester();

		/*! Tests a collision every COLLISION_TEST_FREQUENCY clocks
		*/
		void render();

		/*! Destroys a tester
		*/
		~CollisionTester();
};
