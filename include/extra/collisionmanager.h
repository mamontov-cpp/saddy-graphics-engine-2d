/*! \file   collisionmanager.h
    \author HiddenSeeker
	
	This file contains a basic definition of collision primitives
	Definition of CollisionHandler and CollisionManager are placed here
*/
#include "../templates/hhash.hpp"
#include "../templates/hpair.hpp"
#include "../templates/sinstance.h"
#include "../os/mutex.h"
#include <time.h>
#include "../input.h"
#pragma once

/*! Colllision tester for objects
*/
typedef bool(*ObjectTester)(void*,void*);

class CollisionHandler
{
 private:
	     /*! Function, that will be called on collision. 
		     In first object will be a pointer to first group, on
			 second is a second group
		 */
	     void (*fun)(void * o1, void * o2);
 protected:
		 /* Default constructor
		 */
	    inline CollisionHandler() {}
 public:
	     /*! Creates a handler
		     \param[in] f called function
		 */
		 CollisionHandler(void (*f)(void * o1, void * o2));
		 /*! Clones an existing handler
		     \return new handler
		 */
		 virtual CollisionHandler * clone() const;
		 /*! Calls a handler
		     \param[in] o1 object of first group
			 \param[in] o2 object of second group
		 */
		 virtual void operator()(void * o1,void * o2);
		 /*  Destructor
		 */
		 virtual ~CollisionHandler();
};
/*! Declares a handler for classes
*/
template<
typename _Class1,
typename _Class2
>
class CMHandler: public CollisionHandler
{
private:
	     void (*m_funt)(_Class1 * o1, _Class2 * o2);
public:
		 /*! Creates a handler
		     \param[in] f called function
		 */
	    CMHandler(void (*f)(_Class1 * o1, _Class2 * o2)) {m_funt=f;}
		 /*! Clones an existing handler
		     \return new handler
		 */
		 virtual CollisionHandler * clone() const { return new CMHandler(m_funt); }
		 /*! Calls a handler
		     \param[in] o1 object of first group
			 \param[in] o2 object of second group
		 */
		 virtual void operator()(void * o1,void * o2) { if (m_funt) m_funt(static_cast<_Class1 *>(o1),static_cast<_Class2 *>(o2));}
		 /*  Destructor
		 */
		 virtual ~CMHandler() {}
};
/*! Class,that provides a managing of collisions in game
    Use ::test() to detect a collision and call handler.
*/
class CollisionManager
{
 private:
	 /*! Groups of objects, that is being tested
	 */
	 hst::hash<int, hst::vector<void*> >  m_objects;
	 /*! Hash for reverse-object search
	 */
	 hst::hash<void*,int>     m_reverse_objects;
	 /*! Vector of tasks for addition of objects
	 */
	 hst::vector< hst::pair<int,void *> >  m_adding_tasks;
	 /*! Vector of tasks for removing ob objects
	 */
	 hst::vector< void * > m_remove_tasks;
	 /*! Lock for task addition
 	 */
	 os::mutex  m_add_lock;
	 /*! Lock for task removing
	 */
	 os::mutex  m_remove_lock;
	 /*! Flags, that points  for rescanning note
	 */
	 bool m_rescan_line;
	 /*! Class of testing task
	 */
	 class TestTask
	 {
	  public:
		      //! First object type
		      int type1;
			  //! Second object type
			  int type2;
			  //! Collision tester for those types
			  ObjectTester tester;
			  //!< Handler, which is used if collides
			  CollisionHandler * handler;
			  /*! Constructor
			  */
			  inline TestTask()  {type1=0;type2=0;tester=0;handler=NULL;}
			  /*! Destructor
			  */
			  inline ~TestTask() { delete handler;}
			  /*! Copy constructor
				  \param[in] o object
			  */
			  inline TestTask(const TestTask & o) {type1=o.type1; type2=o.type2; tester=o.tester; if (o.handler) handler=o.handler->clone(); else handler=NULL;  }
			  /*! Testing data.
			  */
			  inline void test(void* op1,void* op2) {if (tester(op1,op2)) (*handler)(op1,op2); }
	 };
	 
	 //! Tasks for testing
	 hst::vector<TestTask> m_tasks;
	 //! Tasks for removing
	 hst::vector< hst::pair<int,int> > m_task_for_removal;
	 //! Tasks for adding to data
	 hst::vector< hst::pair< 
		                     hst::pair<int,int>,
							 hst::pair<ObjectTester,CollisionHandler *>
	                        >  
	             > m_task_to_add;
     //! Mutex for changing tasks vector
	 os::mutex  m_test_tasks_adds_lock;
	 //! Mutex for changing tasks vector
	 os::mutex  m_test_tasks_remove_lock;
	 /*! Commits adding every test task
	 */
	 void  commitAddingTestTasks();
	 /*! Adds new testing task to vector
		 \param[in] id1 type 1
		 \param[in] id2 type 2
		 \param[in] t   tester functions
		 \param[in] h   handler
	 */
	 void   commitTestTaskImmediately(int id1,int id2,ObjectTester t, CollisionHandler * h);
	 /*! Removes a collision task
		 \param[in] id1 type1
		 \param[in] id2 type2
	 */
	 void   removeTestTask(int id1,int id2);
	 /*! Finds test task in list
		 \param[in] id1  first id1
		 \param[in] id2  second id2
		 \return -1 if not found
	 */
	 int findTestTask(int id1,int id2);
	 /*! Removes a task from specified position immediately
		 \param[in] pos position of task
	 */
	 void  removeTestTaskImmediately(int pos);
	 /*! Removes every test task 
	 */
	 void commitTestTaskRemoval();
	 /*! Finds an object in hashes
		 \param[in] p objects
		 \return -1 if not found, otherwise - position
	 */
	 int  findObject(void * p);
	 /*! Immediately adds new object
	 */
	 void commitObjectAdding();
	 /*! Immediately removes objects
	 */
	 void commitObjectRemoval();
	 /*! Tests for collision a group
		 \param[in] i position of group in TestTask vector
	 */
	 void testForCollision(int i);
	 /*! Tests for collision all groups
	 */
	 void testEveryGroup();
	 /*! Constructor of manager
	 */
	 CollisionManager();
     /*! Copy constructor
		 \param[in] o object
	 */
	 CollisionManager(const CollisionManager & o);
	 /*! Instance
	 */
	 static CollisionManager * Instance;
	 /*! If instance is initialized
	 */
	 static bool               Initialized;
	 /*! Frees instance
	 */
	 static void free();
	 /*! Returns an instance of objects
		 \return instance
	 */
	 static CollisionManager * ref();
 public:
    /*! Binds a new handler and tester to object
		\param[in] id1    first object id
		\param[in] id2    second object id
		\param[in] tester tester function for collision testing
		\param[in] h      handler, that handles all collisions
	*/
    static void bind(int id1,int id2, ObjectTester tester, CollisionHandler * h);
	/*! Unbinds a handler for object groups
		\param[in] id1  first object id
		\param[in] id2  second object id
	*/
	static void unbind(int id1,int id2);
	/*! Adds an new object
		\param[in] id  object typeid
		\param[in] obj object
	*/
	static void add(int id, void * obj);
	/*! Removes an object
		\param[in] obj object
	*/
	static void remove(void * obj);
	/*! Tests an objects for collision
	*/
	static void test();
	/*! Flushes all objects
	*/
	static void flush();
	/*! Tests for rescanning data
	*/
	static void rescan();
};
/*! Adds a collision testing task as a post-render task
*/
void addTestingTask();
/*! Removes a collision testing task
*/
void killTestingTask();
