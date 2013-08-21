#include "p2d/world.h"

double p2d::World::timeStep() const
{
	return m_time_step;
}

p2d::World::World()
{
	m_time_step = 1;
	m_transformer = new p2d::CircleToHullTransformer(*(p2d::CircleToHullTransformer::ref()));
}

p2d::World::~World()
{
	delete m_transformer;
}

p2d::CircleToHullTransformer * p2d::World::transformer()
{
	return m_transformer;
}
