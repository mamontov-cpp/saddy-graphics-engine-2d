#include "p2d/world.h"

double p2d::World::timeStep() const
{
	return m_time_step;
}

p2d::World::World()
{
	m_time_step = 1;
	m_transformer = new p2d::CircleToHullTransformer(*(p2d::CircleToHullTransformer::ref()));
	m_detector = new p2d::SimpleCollisionDetector();
}

p2d::World::~World()
{
	delete m_transformer;
	delete m_detector;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		delete it.key();
	}
	for(size_t i = 0;  i < m_callbacks.count(); i++)
	{
		delete m_callbacks[i].p2();
	}
}

p2d::CircleToHullTransformer * p2d::World::transformer()
{
	return m_transformer;
}


void p2d::World::setDetector(p2d::CollisionDetector * d)
{
	delete m_detector;
	m_detector = d;
	
}


void p2d::World::setTransformer(p2d::CircleToHullTransformer * t)
{
	delete m_transformer;
	m_transformer = t;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		it.key()->trySetTransformer();
	}
}


void p2d::World::removeHandler(p2d::BasicCollisionHandler * h)
{
	for(size_t i = 0;  i < m_callbacks.count(); i++)
	{
		if (m_callbacks[i].p2() == h)
		{
			m_callbacks.removeAt(i);
		}
	}
	delete h;
}


void p2d::World::addBody(p2d::Body * b)
{
	hst::vector<hst::string> groups;
	for(hst::hash<hst::string, hst::vector<p2d::Body*> > ::iterator it = m_groups.begin();
		it != m_groups.end();
		it++)
	{
		if (it.key() == "p2d::Body")
		{
			groups << it.key();
		}
		if (b->userObject() != NULL)
		{
			if (b->userObject()->metaData()->canBeCastedTo(it.key()))
			{
				groups << it.key();
			}
		}
	}
	m_allbodies.insert(b, groups);
	for(size_t i = 0; i < groups.size(); i++)
	{
		m_groups[groups[i]] << b;
	}
}

void p2d::World::removeBody(p2d::Body * b)
{
	if (m_allbodies.contains(b))
	{
		hst::vector<hst::string> groups = m_allbodies[b];
		for(size_t i = 0; i < groups.size(); i++)
		{
			m_groups[groups[i]].removeAll(b);
		}
		m_allbodies.remove(b);
		delete b;
	}
}

void p2d::World::splitTimeStepAt(double time)
{
	m_splitted_time_step.setValue(time);
}



