#include <p2d/ghostoptions.h>
#include <cstdlib>

p2d::GhostOptions::GhostOptions(bool persistent, int ticks)
: m_persistent(persistent), m_ticks(ticks)
{
}

bool p2d::GhostOptions::ghost() const
{
	bool result = false;
	if (m_persistent)
	{
		result  = true;
	} 
	else
	{
		if (m_ticks > 0)
		{
			result = true;
		}
	}
	return result;
}

p2d::GhostOptions p2d::GhostOptions::disabled()
{
	return p2d::GhostOptions(false, 0);
}

p2d::GhostOptions p2d::GhostOptions::forever()
{
	return p2d::GhostOptions(true);
}

p2d::GhostOptions p2d::GhostOptions::oneTime()
{
	return p2d::GhostOptions(false, 1);
}

p2d::GhostOptions * p2d::GhostOptions::step() const
{
	p2d::GhostOptions * result = new p2d::GhostOptions(false, 0);
	if (m_persistent)
	{
		*result = p2d::GhostOptions(true);
	}
	else
	{
		if (m_ticks > 0)
		{
			*result = p2d::GhostOptions(false, m_ticks - 1);
		}
	}
	return result;
}

p2d::GhostOptionsState::GhostOptionsState()
{
	m_inner = NULL;
}

p2d::GhostOptionsState::~GhostOptionsState()
{
	delete m_inner;
	if (m_next_value.exists())
	{
		delete m_next_value.data();
	}
}

void p2d::GhostOptionsState::setCurrent(p2d::GhostOptions * opts)
{
	delete m_inner;
	m_inner = opts;
}

void p2d::GhostOptionsState::push(p2d::GhostOptions * opts)
{
	if (m_next_value.exists())
	{
		delete m_next_value.data();
	}
	m_next_value.setValue(opts);
}

void p2d::GhostOptionsState::step()
{
	if (m_next_value.exists())
	{
		delete m_inner;
		m_inner = m_next_value.data();
		m_next_value.clear();
	}
	else
	{
		p2d::GhostOptions * g = m_inner->step();
		delete m_inner;
		m_inner = g;
	}
}

bool p2d::GhostOptionsState::userChanged() const
{
	return m_next_value.exists();
}

p2d::GhostOptions * p2d::GhostOptionsState::nextOptions()
{
	p2d::GhostOptions * p = NULL;
	if (m_next_value.exists())
	{
		 p = m_next_value.data();
	}
	return p;
}

bool p2d::GhostOptionsState::value() const
{
	return m_inner->ghost();
}

