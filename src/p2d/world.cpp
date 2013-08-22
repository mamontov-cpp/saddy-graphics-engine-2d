#include "p2d/world.h"

double p2d::World::timeStep() const
{
	return m_time_step;
}

p2d::World::World()
{
	m_stepping = false;
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
	if (m_stepping)
	{
		m_added_queue << b;
	}

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
	if (m_stepping)
	{
		m_removed_queue << b;
	}

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

void p2d::World::addHandler(
		 p2d::BasicCollisionHandler * h, 
		 const hst::string & t1, 
		 const hst::string & t2
)
{
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
}

void p2d::World::step(double time)
{
	performQueuedActions();
	m_stepping = true;
	m_time_step = time;
	while ( non_fuzzy_zero(m_time_step) )
	{
		m_splitted_time_step.clear();
		findAndExecuteCollisionCallbacks();
		if (m_splitted_time_step.exists())
		{
			stepPositionsAndVelocities(m_splitted_time_step.data());
			m_time_step -= m_splitted_time_step.data();
		}
		else
		{
			stepPositionsAndVelocities(m_time_step);
			m_time_step = 0;
		}
	}
	// Set time step to something to not bug with some interstep calls
	m_time_step = 1;
	// Step forces and body options
	stepDiscreteChangingValues();
	m_stepping = false;
	performQueuedActions();
}

void p2d::World::stepDiscreteChangingValues()
{
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		it.key()->stepDiscreteChangingValues();
	}
}

void p2d::World::stepPositionsAndVelocities(double time)
{
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		it.key()->stepPositionsAndVelocities(time);
	}
}

void p2d::World::executeCallbacks(reactions_t & reactions)
{
	for (size_t i = 0; i < reactions.count(); i++)
	{
		reactions[i].p2()->invoke(reactions[i].p1());
		// If time splitted, we should erase all callbacks
		if (m_splitted_time_step.exists())
		{
			reactions.clear();
		}
	}
}

bool p2d::World::compare(const reaction_t & r1, const reaction_t & r2)
{
	return r1.p1().m_time < r2.p1().m_time;
}

void p2d::World::sortCallbacks(reactions_t & reactions)
{
	std::sort(reactions.begin(), reactions.end(), p2d::World::compare);
}

void p2d::World::findAndExecuteCollisionCallbacks()
{
	reactions_t reactions;
	findEvents(reactions);
	sortCallbacks(reactions);
	executeCallbacks(reactions);
}

void p2d::World::findEvents(reactions_t & reactions)
{
	for (size_t i = 0; i < m_callbacks.count(); i++)
	{
		findEvent(reactions, m_callbacks[i]);
	}
}

void p2d::World::findEvent(reactions_t & reactions, const types_with_handler_t & twh)
{
	hst::hash<p2d::Body*, hst::hash<p2d::Body *, int>> set;
	if (m_groups.contains(twh.p1().p1()) && m_groups.contains(twh.p1().p2()))
	{
		hst::vector<p2d::Body *> & g1 = m_groups[twh.p1().p1()];
		hst::vector<p2d::Body *> & g2 = m_groups[twh.p1().p2()];
		p2d::BasicCollisionHandler * h = twh.p2();
		for (size_t i = 0; i < g1.count(); i++)
		{
			for (size_t j = 0; j < g2.count(); j++)
			{
				p2d::Body * b1 = g1[i];
				p2d::Body * b2 = g2[j];
				bool cancollide = (b1 != b2);
				if (cancollide)
				{
					if (set.contains(b1))
					{
						cancollide = !(set[b1].contains(b2));
					}
				}
				if (cancollide)
				{
					hst::Maybe<double> time;
					time = m_detector->collides(b1, b2, m_time_step);
					if (time.exists())
					{
						BasicCollisionEvent ev(b1, b2, time.data());
						reactions << reaction_t(ev, h);

						// Guard same type groups from executing same event twice
						if (!set.contains(b1)) set.insert(b1, hst::hash<p2d::Body*, int>());
						if (!set.contains(b2)) set.insert(b2, hst::hash<p2d::Body*, int>());
						set[b1].insert(b2, 0);
						set[b2].insert(b1, 0);
					}
				}
			}
		}
	}

}


void p2d::World::performQueuedActions()
{
	for(size_t i = 0; i < m_added_queue.count(); i++)
	{
		addBody(m_added_queue[i]);
	}
	m_added_queue.clear();
	for(size_t i = 0; i < m_removed_queue.count(); i++)
	{
		removeBody(m_removed_queue[i]);
	}
	m_removed_queue.clear();
}

void p2d::World::clear()
{
	// To make no problems, with iterators, step through bodies
	// as vector
	hst::vector<p2d::Body *> bodies;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		bodies << it.key();
	}
	for(size_t i = 0; i < bodies.count(); i++)
	{
		removeBody(bodies[i]);
	}
}

p2d::BasicCollisionHandler *
p2d::World::addHandler( void (*p)(const p2d::BasicCollisionEvent &))
{
	p2d::BasicCollisionHandler * h = 
	new p2d::FunctionCollisionHandler<p2d::Body, p2d::Body>(p);
	hst::string b = "p2d::Body";
	this->addHandler(h, b, b);
	return h;
}

