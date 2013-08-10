#include <p2d/force.h>

p2d::Force::Force(const p2d::Vector &  value, 
				  bool persistent,
				  int ticks
				 )
: m_value(value), m_persistent(persistent), m_ticks(ticks) 
{

}

p2d::Vector p2d::Force::value()
{
	p2d::Vector result(0, 0);
	if (m_persistent)
	{
		result = m_value;
	}
	else
	{
		if (m_ticks > 0)
		{
			result = m_value;
			--m_ticks;
		}
	}
	return result;
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



