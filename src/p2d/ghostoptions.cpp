#include <p2d/ghostoptions.h>

p2d::GhostOptions::GhostOptions(bool persistent, int ticks)
: m_persistent(persistent), m_ticks(ticks)
{
}

bool p2d::GhostOptions::ghost()
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
			--m_ticks;
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

