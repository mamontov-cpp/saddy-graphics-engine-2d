/*! \file keymouseconditions.h
    

    Defines a special kind of conditions, which is used for checking for keycodes in  controls
 */
#pragma once
#include "keycodes.h"
#include "input/handlerconditions.h"
#include "input/events.h"

namespace sad
{

/*! A condition, which is met, when user holds a specified key when key pressed or released event is
    received.
    
    This condition is checked via Key field in KeyEvent
 */
class KeyHoldCondition: public sad::input::AbstractHanderCondition
{
public:
    /*! Creates a new condition, for specified key
        \param[in] key a key, which event will be tested against
     */
    inline KeyHoldCondition(sad::KeyboardKey key) : m_key(key)
    {

    }
    /*! Casts event to a KeyEvent and tests it against conditions
        \param[in] e event
        \return whether we should  run an event
     */
    virtual bool check(const sad::input::AbstractEvent & e);
    /*! Sets key value for condition
        \param[in] key a key code value
     */
    void setKey(sad::KeyboardKey key);
    /*! Clones a condition
        \return clone a condition
     */
    virtual sad::input::AbstractHanderCondition * clone();

private:
    /*! A keyboard key, which event will be tested against
     */
    sad::KeyboardKey m_key;
};

/*! A condition, which is met, when user holds a special key, like Alt, Shift or Control
    and key event for event data
 */
class SpecialKeyHoldCondition: public sad::input::AbstractHanderCondition
{
public:
    /*! Creates new condition for key, which should be pressed, when event is emitted
        \param[in] key a key, which must be hold to trigger condition
     */
    SpecialKeyHoldCondition(sad::SpecialKey key);
    /*! Casts event to a KeyEvent and tests it against conditions
        \param[in] e event
        \return whether we should  run an event
     */
    virtual bool check(const sad::input::AbstractEvent & e);
    /*! Sets key value for condition
        \param[in] key a key code value
     */
    void setKey(sad::SpecialKey key);
    /*! Clones a condition
        \return clone a condition
     */
    virtual sad::input::AbstractHanderCondition* clone();
protected:
    sad::SpecialKey m_key;
};


/*! A conditions, which is met, when user holds a specified mouse button and mouse event is received.
    
 */
class MouseButtonHoldCondition: public sad::input::AbstractHanderCondition
{
public:
    /*! Creates a new condition, for specified mouse button
        \param[in] button a mouse button, which condition will be tested against
     */
    inline MouseButtonHoldCondition(sad::MouseButton button) : m_button(button)
    {

    }
    /*! Casts event to a MouseEvent and tests it against conditions
        \param[in] e event
        \return whether we should  run an event
     */
    virtual bool check(const sad::input::AbstractEvent & e);
    /*! Sets button value for condition
        \param[in] button a button value
     */
    void setButton(sad::MouseButton button);
    /*! Clones a condition
        \return clone a condition
     */
    virtual sad::input::AbstractHanderCondition * clone();
private:
    /*! A keyboard key, which event will be tested against
     */
    sad::MouseButton m_button;
};

}

/*! Appends a condition, when user pressed specified key
    to list of conditions, returning it. Allows to write something like 
    t | sad::Z | sad::HoldCtrl
    \param[in] t type and conditions
    \param[in] key a condition, when user holds a key
    \return type and conditions
 */
inline sad::input::HandlerTypeAndConditions operator&(
    const sad::input::HandlerTypeAndConditions & t,
    sad::KeyboardKey key
)
{
    sad::input::HandlerTypeAndConditions & mt = const_cast<sad::input::HandlerTypeAndConditions &>(t);
    mt._2() << new sad::KeyHoldCondition(key);
    return t;
}

/*! Appends a condition, when user holds specified key
    to list of conditions, returning it. Allows to write something like 
    t & sad::Z & sad::HoldCtrl
    \param[in] t type and conditions
    \param[in] key a condition, when user holds a key
    \return type and conditions
 */
inline sad::input::HandlerTypeAndConditions operator&(
    const sad::input::HandlerTypeAndConditions & t,
    sad::SpecialKey key
)
{
    sad::input::HandlerTypeAndConditions & mt = const_cast<sad::input::HandlerTypeAndConditions &>(t);
    sad::input::AbstractHanderCondition * h = NULL;
    h = new sad::SpecialKeyHoldCondition(key);
    mt._2() << h;
    return t;
}

/*! Appends a condition, when user pressed specified mouse button
    to list of conditions, returning it. Allows to write something like 
    t & sad::MouseLeft
    \param[in] t type and conditions
    \param[in] button a pressed button
    \return type and conditions
 */
inline sad::input::HandlerTypeAndConditions operator&(
    const sad::input::HandlerTypeAndConditions & t,
    sad::MouseButton button
)
{
    sad::input::HandlerTypeAndConditions & mt = const_cast<sad::input::HandlerTypeAndConditions &>(t);
    mt._2() << new sad::MouseButtonHoldCondition(button);
    return t;
}
