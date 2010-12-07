#include "collisionmanager.h"
#include <stdlib.h>

CollisionHandler::CollisionHandler(void (*f)(Collidable * o1, Collidable * o2))
{
	fun=f;
}
void CollisionHandler::operator()(Collidable * o1,Collidable * o2)
{
	fun(o1,o2);
}
CollisionHandler::~CollisionHandler()
{

}


/*! Groups of registered objects
*/
hst::hash<int,hst::vector<Collidable *> >  CollisionManager::groups;
/*! Delete list
*/
hst::vector< hst::pair<int,Collidable *> >          CollisionManager::m_delete;
/*! Add list
*/
hst::vector< hst::triplet<int,BoundingBox,Collidable *> > CollisionManager::m_add;
/*! Handlers
*/
hst::hash< hst::pair<int,int>,CollisionHandler * > CollisionManager::m_handlers;
/*! Bounding boxes
*/
hst::hash<int,hst::vector<BoundingBox> >  CollisionManager::m_boxes;

void CollisionManager::imAdd(int id, const BoundingBox & a,Collidable * b)
{
	if (!groups.contains(id))
	{
		groups.insert(id,hst::vector<Collidable *>());
		m_boxes.insert(id,hst::vector<BoundingBox>());
	}
	groups[id]<<b;
	m_boxes[id]<<a;
}
void CollisionManager::imDelete(int id,Collidable * a)
{
	if (groups.contains(id))
	{
		hst::vector<Collidable *> & o1=groups[id];
		hst::vector<BoundingBox> & o2=m_boxes[id];
		for (int i=0;i<o1.count();i++)
		{
			if (o1[i]==a)
			{
				o1.removeAt(i);
				o2.removeAt(i);
				--i;
			}
		}
	}
}

void CollisionManager::scanGroup(int g1,int g2,CollisionHandler & h)
{
	if (!groups.contains(g1) || !groups.contains(g2)) return;
	if (g1==g2)
	{
		hst::vector<BoundingBox> & bawxes=m_boxes[g1];
        hst::vector<Collidable *> & grs=groups[g1];
        
		for (int i=0;i<grs.count();i++)
		{
			for (int j=i+1;j<grs.count();j++)
			{
				{
				if (optimizedcollides(bawxes[i],
					                  grs[i]->rect(),
									  bawxes[j],
									  grs[j]->rect()
									  ))
				h(grs[i],grs[j]);
				}
				bawxes[i]=grs[i]->rect();
				bawxes[j]=grs[j]->rect();
			}
		}
		return;
	}
	else
	{
       	hst::vector<BoundingBox> & bawxes1=m_boxes[g1];
        hst::vector<Collidable *> & grs1=groups[g1];
        
		hst::vector<BoundingBox> & bawxes2=m_boxes[g2];
		hst::vector<Collidable *> & grs2=groups[g2];
        
		for (int i=0;i<grs1.count();i++)
		{
			for (int j=0;j<grs2.count();j++)
			{
			  {
				if (optimizedcollides(bawxes1[i],
					                  grs1[i]->rect(),
									  bawxes2[j],
									  grs2[j]->rect()
									  ))
				h(grs1[i],grs2[j]);
			   }
			   bawxes1[i]=grs1[i]->rect();
			   bawxes2[j]=grs2[j]->rect();				
			}
		}
	}
}

void CollisionManager::detect()
{
	
	if (m_add.count()!=0 || m_delete.count()!=0)
	{
		for (int i=0;i<m_add.count();i++)
			imAdd(m_add[i].p1(),m_add[i].p2(),m_add[i].p3());
		for (int i=0;i<m_delete.count();i++)
			imDelete(m_delete[i].p1(),m_delete[i].p2());
		m_add.clear();
		m_delete.clear();
	}
	

	hst::hash< hst::pair<int,int>,CollisionHandler * >::iterator it=m_handlers.begin();
	
	for (;it!=m_handlers.end();it++)
	{
		scanGroup(it.key().p1(),it.key().p2(),*(it.value()));
	}
}
void CollisionManager::flushHandlers()
{
	hst::hash< hst::pair<int,int>,CollisionHandler * >::iterator it=m_handlers.begin();
	
	for (;it!=m_handlers.end();it++)
	{
		delete it.value();
	}
}
void CollisionManager::unbind(CollisionGroupID id1,CollisionGroupID id2)
{
	hst::pair<int,int> p(id1,id2);
    if (m_handlers.contains(p))
	{
		delete m_handlers[p];
		m_handlers.remove(p);
	}
}

void CollisionManager::bind(CollisionGroupID id1, CollisionGroupID id2, CollisionHandler * h)
{
	static bool init=false;
	if (!init)
	{
		atexit(CollisionManager::flushHandlers);
		init=true;
	}
	unbind(id1,id2);
	m_handlers.insert(hst::pair<int,int>(id1,id2),h);
}

void CollisionManager::flush()
{
	hst::hash<int,hst::vector<Collidable *> >::iterator it;
    hst::hash<int,hst::vector<BoundingBox> >::iterator it2;
	
	for (it=groups.begin();it!=groups.end();++it)
		it.value().clear();
	for (it2=m_boxes.begin();it2!=m_boxes.end();++it2)
		it2.value().clear();
}

void CollisionManager::add(Collidable * a)
{
	m_add<<hst::triplet<int,BoundingBox,Collidable *>(a->type(),a->rect(),a);
}
void CollisionManager::remove(Collidable * b)
{
	m_delete<<hst::pair<int,Collidable *>(b->type(),b);
}


CollisionTester::CollisionTester()
{
  m_lastclock=0;
}

void CollisionTester::render()
{
 if (clock()-m_lastclock>COLLISION_TEST_FREQUENCY)
 {
	 m_lastclock=clock();
	 CollisionManager::detect();
 }
}
CollisionTester::~CollisionTester()
{

}