#include "keymouseconditions.h"

bool sad::KeyHoldCondition::check(const sad::input::AbstractEvent & e)
{
    const sad::input::KeyEvent & ke = static_cast<const sad::input::KeyEvent &>(e);
    return ke.Key == m_key;
}

void sad::KeyHoldCondition::setKey(sad::KeyboardKey key)
{
    m_key = key;
}

sad::input::AbstractHandlerCondition * sad::KeyHoldCondition::clone()
{
    return new sad::KeyHoldCondition(m_key);
}


sad::SpecialKeyHoldCondition::SpecialKeyHoldCondition(sad::SpecialKey key) 
: m_key(key)
{

}

bool sad::SpecialKeyHoldCondition::check(const sad::input::AbstractEvent & e)
{
    const sad::input::KeyEvent & ke = static_cast<const sad::input::KeyEvent &>(e);
    sad::input::KeyEvent & k = const_cast<sad::input::KeyEvent &>(ke);
    bool result = false;
    switch(m_key)
    {
        case sad::SpecialKey::HoldsControl: result = k.CtrlHeld; break;
        case sad::SpecialKey::HoldsAlt: result = k.AltHeld; break;
        case sad::SpecialKey::HoldsShift: result = k.ShiftHeld; break;
    }
    return result;
}

void sad::SpecialKeyHoldCondition::setKey(sad::SpecialKey key)
{
    m_key = key;
}

sad::input::AbstractHandlerCondition* sad::SpecialKeyHoldCondition::clone()
{
     sad::SpecialKeyHoldCondition* result =  new sad::SpecialKeyHoldCondition(m_key);
     return result;
}

bool sad::MouseButtonHoldCondition::check(const sad::input::AbstractEvent & e)
{
    const sad::input::MouseEvent & ke = static_cast<const sad::input::MouseEvent &>(e);
    return ke.Button == m_button;
}

void sad::MouseButtonHoldCondition::setButton(sad::MouseButton button)
{
    m_button = button;
}

sad::input::AbstractHandlerCondition * sad::MouseButtonHoldCondition::clone()
{
    return new sad::MouseButtonHoldCondition(m_button);
}
