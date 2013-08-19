#include <p2d/force.h>
#include <cstdlib>

p2d::Force::Force(const p2d::Vector &  value, 
				  bool persistent,
				  int ticks
				 )
: m_value(value), m_persistent(persistent), m_ticks(ticks) 
{

}

p2d::Vector p2d::Force::value() const
{
	return m_value;	
}

p2d::Force * p2d::Force::step() const
{
	p2d::Force * force = new p2d::Force();
	if (m_persistent)
	{
		*force = p2d::Force(m_value, true, 0);
	}
	else
	{
		if (m_ticks > 0)
		{
			*force = p2d::Force(m_value,  false, m_ticks - 1);
		}
	}
	return force;
}

void p2d::Force::setValue(const p2d::Vector & v)
{
	m_value = v;
}

void  p2d::Force::makePersistent()
{
	m_persistent = true;
	m_ticks = 0;
}

void  p2d::Force::makeImpulseFor(int ticks)
{
	m_persistent = false;
	m_ticks = ticks;
}

void p2d::Force::disable()
{
	m_persistent = false;
	m_ticks = 0;
}

p2d::Force p2d::Force::zero()
{
	return p2d::Force(p2d::Vector(0,0), false, 0);
}

p2d::Force p2d::Force::persistent(const p2d::Vector & v)
{
	return p2d::Force(v, true);
}

p2d::Force p2d::Force::impulse(const p2d::Vector & v)
{
	return p2d::Force(v, false, 1);
}

p2d::ForceState::ForceState()
{
	m_force = NULL;
}

p2d::ForceState::~ForceState()
{
	delete m_force;
	if (m_next_value.exists())
	{
		delete m_next_value.data();
	}
}

void p2d::ForceState::setCurrent(p2d::Force * force)
{
	delete m_force;
	m_force = force;
}

void p2d::ForceState::push(p2d::Force * force)
{
	if (m_next_value.exists())
	{
		delete m_next_value.data();
	}
	m_next_value.setValue(force);
}

void p2d::ForceState::step()
{
	if (m_next_value.exists())
	{
		delete m_force;
		m_force = m_next_value.data();
		m_next_value.clear();
	}
	else
	{
		p2d::Force * f = m_force->step();
		delete m_force;
		m_force = f;
	}
}

bool p2d::ForceState::userChanged() const
{
	return m_next_value.exists();
}

p2d::Force * p2d::ForceState::nextForce()
{
	p2d::Force * p = NULL;
	if (m_next_value.exists())
	{
		 p = m_next_value.data();
	}
	return p;
}

p2d::Vector p2d::ForceState::value() const
{
	return m_force->value();
}

