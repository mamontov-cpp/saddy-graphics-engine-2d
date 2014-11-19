#include "p2d/world.h"

double sad::p2d::World::timeStep() const
{
	return m_time_step;
}

sad::p2d::World::World()
{
	m_time_step = 1;
	m_transformer = new p2d::CircleToHullTransformer(*(p2d::CircleToHullTransformer::ref()));
	m_detector = new p2d::SimpleCollisionDetector();
}

sad::p2d::World::~World()
{
	delete m_transformer;
	delete m_detector;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
        it.key()->delRef();
	}
	for(size_t i = 0;  i < m_callbacks.count(); i++)
	{
		delete m_callbacks[i].p2();
	}
}

sad::p2d::CircleToHullTransformer * sad::p2d::World::transformer()
{
	return m_transformer;
}


void sad::p2d::World::setDetector(sad::p2d::CollisionDetector * d)
{
	delete m_detector;
	m_detector = d;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		it++
	   )
	{
		it.key()->setSamplingCount(d->sampleCount());
	}
}


void sad::p2d::World::setTransformer(sad::p2d::CircleToHullTransformer * t)
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


void sad::p2d::World::removeHandler(sad::p2d::BasicCollisionHandler * h)
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
void sad::p2d::World::addNow(p2d::Body * b)
{
	sad::Vector<sad::String> groups;
	for(sad::Hash<sad::String, sad::Vector<p2d::Body*> > ::iterator it = m_groups.begin();
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
    b->addRef();
	m_allbodies.insert(b, groups);
	for(size_t i = 0; i < groups.size(); i++)
	{
		m_groups[groups[i]] << b;
	}
	b->setSamplingCount(m_detector->sampleCount());
	b->setWorld(this);
}

void sad::p2d::World::removeNow(p2d::Body * b)
{
	if (m_allbodies.contains(b))
	{
		sad::Vector<sad::String> groups = m_allbodies[b];
		for(size_t i = 0; i < groups.size(); i++)
		{
			m_groups[groups[i]].removeAll(b);
		}
		m_allbodies.remove(b);
        b->delRef();
	}
}

void sad::p2d::World::splitTimeStepAt(double time)
{
	m_splitted_time_step.setValue(time);
}

void sad::p2d::World::addHandler(
		 sad::p2d::BasicCollisionHandler * h, 
		 const sad::String & t1, 
		 const sad::String & t2
)
{
	if (m_groups.contains(t1) == false)
	{
		m_groups.insert(t1, sad::Vector<p2d::Body *>());
	}
	if (m_groups.contains(t2) == false)
	{
		 m_groups.insert(t2, sad::Vector<p2d::Body *>());
	}
	type_pair_t tp(t1, t2);
	types_with_handler_t twh(tp, h);
	m_callbacks << twh;
}

void sad::p2d::World::step(double time)
{
	performQueuedActions();
	lockChanges();
	m_time_step = time;
	while ( sad::non_fuzzy_zero(m_time_step) )
	{
		m_splitted_time_step.clear();
		buildBodyCaches();
		findAndExecuteCollisionCallbacks();
		if (m_splitted_time_step.exists())
		{
			stepPositionsAndVelocities(m_splitted_time_step.value());
			m_time_step -= m_splitted_time_step.value();
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
	stepDiscreteChangingValues(time);
	unlockChanges();
	performQueuedActions();
}

void sad::p2d::World::stepDiscreteChangingValues(double time)
{
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		++it
	   )
	{
		it.key()->stepDiscreteChangingValues(time);
	}
}

void sad::p2d::World::buildBodyCaches()
{
	double t = this->timeStep();
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		++it
	   )
	{
		it.key()->TimeStep = t;
		it.key()->buildCaches();
	}
}

void sad::p2d::World::stepPositionsAndVelocities(double time)
{
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		++it
	   )
	{
		it.key()->stepPositionsAndVelocities(time);
	}
}

void sad::p2d::World::executeCallbacks(reactions_t & reactions)
{
	for (size_t i = 0; i < reactions.count(); ++i)
	{
		reactions[i].p2()->invoke(reactions[i].p1());
		// If time splitted, we should erase all callbacks
		if (m_splitted_time_step.exists())
		{
			reactions.clear();
		}
	}
}

bool sad::p2d::World::compare(const reaction_t & r1, const reaction_t & r2)
{
	return r1.p1().m_time < r2.p1().m_time;
}

void sad::p2d::World::sortCallbacks(reactions_t & reactions)
{
	std::sort(reactions.begin(), reactions.end(), p2d::World::compare);
}

void sad::p2d::World::findAndExecuteCollisionCallbacks()
{
	reactions_t reactions;
	findEvents(reactions);
	sortCallbacks(reactions);
	executeCallbacks(reactions);
}

void sad::p2d::World::findEvents(reactions_t & reactions)
{
	for (size_t i = 0; i < m_callbacks.count(); i++)
	{
		findEvent(reactions, m_callbacks[i]);
	}
}

void sad::p2d::World::findEvent(reactions_t & reactions, const types_with_handler_t & twh)
{
	double step = this->timeStep();
	if (m_groups.contains(twh.p1().p1()) && m_groups.contains(twh.p1().p2()))
	{
		sad::Vector<p2d::Body *> & g1 = m_groups[twh.p1().p1()];
		sad::Vector<p2d::Body *> & g2 = m_groups[twh.p1().p2()];
		bool not_same_group = (twh.p1().p1() != twh.p1().p2());
		p2d::BasicCollisionHandler * h = twh.p2();
		for (size_t i = 0; i < g1.count(); i++)
		{
			size_t jmin = i + 1;
			if (not_same_group) jmin = 0;
			for (size_t j = jmin; j < g2.count(); j++)
			{
				p2d::Body * b1 = g1[i];
				p2d::Body * b2 = g2[j];
				if (b1 != b2 && b1->isGhost() == false && b2->isGhost() == false)
				{
					sad::Maybe<double> time;
					b1->TimeStep = step;
					b2->TimeStep = step;
					time = m_detector->collides(b1, b2, m_time_step);
					if (time.exists())
					{
						BasicCollisionEvent ev(b1, b2, time.value());
						reactions << reaction_t(ev, h);
					}
				}
			}
		}
	}

}



void sad::p2d::World::clearNow()
{
	// To make no problems, with iterators, step through bodies
	// as vector
	sad::Vector<p2d::Body *> bodies;
	for( bodies_to_types_t::iterator it = m_allbodies.begin();
		it != m_allbodies.end();
		++it
	   )
	{
		bodies << it.key();
	}
	for(size_t i = 0; i < bodies.count(); i++)
	{
		removeNow(bodies[i]);
	}
}

sad::p2d::BasicCollisionHandler *
sad::p2d::World::addHandler( void (*p)(const sad::p2d::BasicCollisionEvent &))
{
	p2d::BasicCollisionHandler * h = 
	new p2d::FunctionCollisionHandler<p2d::Body, p2d::Body>(p);
	sad::String b = "p2d::Body";
	this->addHandler(h, b, b);
	return h;
}

void sad::p2d::World::remove(p2d::Body * body)
{
	body->setUserObject(NULL);
	body->clearListeners();
	this->sad::TemporarilyImmutableContainer<p2d::Body>::remove(body);
}
