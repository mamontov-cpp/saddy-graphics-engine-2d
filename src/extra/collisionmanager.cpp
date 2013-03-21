#include "collisionmanager.h"
#include "rigid_body.h"
#include "../renderer.h"
#include <stdlib.h>

void Collidable::die()
{
	sad::Renderer::ref()->getCurrentScene()->markForDeletion(this);
	CollisionManager::remove(this);
}

CollisionHandler::CollisionHandler(void (*f)(void * o1, void * o2))
{
	fun=f;
}

CollisionHandler * CollisionHandler::clone() const
{
	CollisionHandler * p=new CollisionHandler(fun);
	return p;
}
void CollisionHandler::operator()(void * o1,void * o2)
{
	fun(o1,o2);
}
CollisionHandler::~CollisionHandler()
{

}
int CollisionManager::findTestTask(int id1,int id2)
{
	for (unsigned int i=0;i<m_tasks.count();i++)
		if ((m_tasks[i].type1==id1 && m_tasks[i].type2==id2) || (m_tasks[i].type1==id2 && m_tasks[i].type2==id1))
			return (int)i;
	return -1;
}
void   CollisionManager::commitTestTaskImmediately(int id1,int id2,ObjectTester t, CollisionHandler * h)
{
	int ft=findTestTask(id1,id2);
    if (ft!=-1) removeTestTaskImmediately(ft);

	m_tasks<<TestTask();
	TestTask & last=m_tasks[m_tasks.count()-1];
	last.type1=id1;
	last.type2=id2;
	last.tester=t;
	last.handler=h;
}
void  CollisionManager::commitAddingTestTasks()
{
	m_test_tasks_adds_lock.lock();
	if (m_task_to_add.count()!=0)
	{
		for (unsigned int i=0;i<m_task_to_add.count();i++)
			commitTestTaskImmediately(m_task_to_add[i].p1().p1(),m_task_to_add[i].p1().p2(),
			                          m_task_to_add[i].p2().p1(),const_cast<CollisionHandler *>(m_task_to_add[i]._2()._2())
						             );
		m_task_to_add.clear();
	}
	m_test_tasks_adds_lock.unlock();
}
void   CollisionManager::removeTestTask(int id1,int id2)
{
	m_task_for_removal<<hst::pair<int,int>(id1,id2);
}
void  CollisionManager::removeTestTaskImmediately(int pos)
{
	m_tasks.removeAt(pos);
}
void CollisionManager::commitTestTaskRemoval()
{
	m_test_tasks_remove_lock.lock();
	if (m_task_for_removal.count()!=0)
	{
		for (unsigned int i=0;i<m_task_for_removal.count();i++)
		{
			int pos=findTestTask(m_task_for_removal[i].p1(),m_task_for_removal[i].p2());
			if (pos!=-1) removeTestTaskImmediately(pos);
		}
		m_task_for_removal.clear();
	}
	m_test_tasks_remove_lock.unlock();
}


CollisionManager::CollisionManager() {}
CollisionManager::CollisionManager(const CollisionManager & o) 
{ 
	m_objects=o.m_objects;
	m_reverse_objects=o.m_reverse_objects;
	m_adding_tasks=o.m_adding_tasks;
	m_remove_tasks=o.m_remove_tasks;
	m_task_for_removal=o.m_task_for_removal;
	m_tasks=o.m_tasks;
}
CollisionManager * CollisionManager::Instance=NULL;
bool               CollisionManager::Initialized=false;
void CollisionManager::free()
{
	delete CollisionManager::Instance;
	CollisionManager::Instance=NULL;
}
CollisionManager * CollisionManager::ref()
{
	if (!CollisionManager::Initialized)
	{
		CollisionManager::Initialized=true;
		CollisionManager::Instance=new CollisionManager();
		atexit(CollisionManager::free);
	}
	return CollisionManager::Instance;
}

int  CollisionManager::findObject(void * p)
{
  if(m_reverse_objects.contains(p))
  {
	  hst::vector<void *> & v=m_objects[m_reverse_objects[p]];
	  for (unsigned int i=0;i<v.count();i++)
		  if (v[i]==p)
			  return i;
	  return -1;
  }
  else 
	  return -1;
}
void CollisionManager::commitObjectAdding()
{
 m_add_lock.lock();
 if (m_adding_tasks.count())
 {
	 for (unsigned int i=0;i<m_adding_tasks.count();i++)
	 {
		 void * data=m_adding_tasks[i].p2();
		 int    type=m_adding_tasks[i].p1();
		 if (!m_reverse_objects.contains(data))
		 {
			 m_reverse_objects.insert(data,type);
			 if (!m_objects.contains(type))
				 m_objects.insert(type,hst::vector<void*>());
			 m_objects[type]<<data;
		 }
	 }
	 m_adding_tasks.clear();
 }
 m_add_lock.unlock();
}

void CollisionManager::commitObjectRemoval()
{
	m_remove_lock.lock();
    if (m_remove_tasks.count())
	{
		for (unsigned int i=0;i<m_remove_tasks.count();i++)
		{
			int pos=findObject(m_remove_tasks[i]);
			if (pos!=-1)
			{
			   m_objects[m_reverse_objects[m_remove_tasks[i]]].removeAt(pos);
			   m_reverse_objects.remove(m_remove_tasks[i]);
			}
		}
		m_remove_tasks.clear();
	}
	m_remove_lock.unlock();
}

void CollisionManager::testForCollision(int i)
{
	do
	{
	m_rescan_line=false;
	TestTask & g=m_tasks[i];
	if (!(m_objects.contains(g.type1) && m_objects.contains(g.type2))) return;
	hst::vector<void *> & t1=m_objects[g.type1];
	hst::vector<void *> & t2=m_objects[g.type2];
	for (unsigned int i1=0;i1<t1.count();i1++)
	{
		for (unsigned int i2=0;i2<t2.count();i2++)
		{
			void * o1=t1[i1];
			void * o2=t2[i2];
			if (o1!=o2)
				if (g.tester(o1,o2))
					(*(g.handler))(o1,o2);
		}
	}
	}
	while (m_rescan_line);
}

void CollisionManager::testEveryGroup()
{
	for (unsigned int i=0;i<m_tasks.count();i++)
		testForCollision(i);
}
void CollisionManager::bind(int id1,int id2, ObjectTester tester, CollisionHandler * h)
{
	if (ref())
	{
		Instance->m_test_tasks_adds_lock.lock();
		Instance->m_task_to_add<<hst::pair< hst::pair<int,int>,
								            hst::pair<ObjectTester,CollisionHandler *>
		                                  >( 
										     hst::pair<int,int>(id1,id2),
											 hst::pair<ObjectTester,CollisionHandler *>
											 (tester,h) 
										   );
		Instance->m_test_tasks_adds_lock.unlock();
	}
}

void CollisionManager::unbind(int id1,int id2)
{
	if (ref())
	{
		Instance->m_test_tasks_remove_lock.lock();
		Instance->m_task_for_removal<<hst::pair<int,int>(id1,id2);
		Instance->m_test_tasks_remove_lock.unlock();
	}
}
void CollisionManager::add(int id, void * obj)
{
	if (ref())
	{
		Instance->m_add_lock.lock();
		Instance->m_adding_tasks<<hst::pair<int,void*>(id,obj);
		Instance->m_add_lock.unlock();
	}
}

void CollisionManager::remove(void * obj)
{
	if (ref())
	{
		Instance->m_remove_lock.lock();
		Instance->m_remove_tasks<<obj;
		Instance->m_remove_lock.unlock();
	}
}
void CollisionManager::rescan()
{
	if (ref())
	{
		Instance->m_rescan_line=true;
	}
}
void CollisionManager::flush()
{
	if (ref())
	{
		Instance->m_objects.clear();
		Instance->m_reverse_objects.clear();
		Instance->m_adding_tasks.clear();
		Instance->m_remove_tasks.clear();
	}
}
void CollisionManager::test()
{
	if (ref())
	{
		Instance->commitAddingTestTasks();
		Instance->commitObjectAdding();
		Instance->commitObjectRemoval(); //If somebody died while rendering we must kill them
		Instance->testEveryGroup();
		Instance->commitObjectRemoval();
		Instance->commitTestTaskRemoval();
	}
}
class CollisionTestingTask: public sad::RepeatingTask
{
 public:
	   inline CollisionTestingTask(): sad::RepeatingTask(CollisionManager::test) 
	   {
		   CollisionManager::flush();
	   }
	   inline ~CollisionTestingTask() {CollisionManager::flush();}
};
typedef Instance<CollisionTestingTask> CTTaskInstance;

void addTestingTask()
{
  CollisionTestingTask * t=new CollisionTestingTask();
  CTTaskInstance::set(t);
  sad::Input::inst()->addPostRenderTask(t);
}

void killTestingTask()
{
	if (CTTaskInstance::ref())
	{
	 CTTaskInstance::ref()->die();
	 CTTaskInstance::zero();
	}
}
