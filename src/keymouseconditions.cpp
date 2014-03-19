#include "keymouseconditions.h"

bool sad::KeyHoldCondition::check(const sad::input::AbstractEvent & e)
{
	const sad::input::KeyEvent & ke = static_cast<const sad::input::KeyEvent &>(e);
	return ke.Key == m_key;
}

sad::input::AbstractHanderCondition * sad::KeyHoldCondition::clone()
{
	return new sad::KeyHoldCondition(m_key);
}


bool sad::MouseButtonHoldCondition::check(const sad::input::AbstractEvent & e)
{
	const sad::input::MouseEvent & ke = static_cast<const sad::input::MouseEvent &>(e);
	return ke.Button == m_button;
}

sad::input::AbstractHanderCondition * sad::MouseButtonHoldCondition::clone()
{
	return new sad::MouseButtonHoldCondition(m_button);
}
