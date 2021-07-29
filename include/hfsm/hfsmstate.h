/*! \file   hfsmstate.h
    

    A state for hierarchical finite state machine
 */
#pragma once
#include "hfsmhandler.h"
#include "hfsmshared.h"

#include "../sadhash.h"
#include "../sadstring.h"
#include "../sadptrvector.h"

#include "../db/dbvariant.h"
#include "../object.h"
#include "../maybe.h"

namespace sad
{

namespace hfsm
{

class Machine;
class State;

typedef sad::Hash<sad::String, sad::hfsm::State *> StateMap;

/*! A state for finite state machine
 */
class State: public sad::Object
{
SAD_OBJECT
public:
    /*! Constructs default state
     */
    State();
    /*! Erases all of children states, freeing their memory
     */
    virtual ~State();
    /*! Sets machine for state
        \param[in] machine
     */
    void setMachine(sad::hfsm::Machine * machine);
    /*! Returns hfsm machine for state
        \return machine
     */
    sad::hfsm::Machine * machine() const;
    /*! Adds new children state. Deletes old state, if state with such name already exists
        \param[in] name name for a state
        \param[in] state  a state (nullptr to create default sad::hfsm::State)
     */
    void addChild(const sad::String & name, sad::hfsm::State * state = nullptr);
    /*! Returns child state by it's name
        \return state data or nullptr if not found
     */
    sad::hfsm::State * child(const sad::String & name) const;
    /*! Returns a children for state map
        \return children states
     */
    const sad::hfsm::StateMap & children() const;
    /*! Removes child state from parent state. Nothing is done, if
        state is absent. Also it will destroy state as object
        \param[in] name name of child state
     */
    void removeChild(const sad::String & name);
    /*! Adds handler for entering state
        \param[in] h handler
        \return h
     */
    virtual sad::hfsm::AbstractHandler* handleEnterWith(sad::hfsm::AbstractHandler * h);
    /*! Adds handler for leaving state
        \param[in] h handler
        \return h
     */
    virtual sad::hfsm::AbstractHandler* handleLeaveWith(sad::hfsm::AbstractHandler * h);
    /*! Adds handler for entering state as function call
        \param[in] h callable handler
        \return created handler
     */
    template<typename _Callable>
    sad::hfsm::AbstractHandler* addEnterHandler(_Callable h)
    {
        return	handleEnterWith(new sad::hfsm::Function<_Callable>(h));
    }
    /*! Adds handler for entering state as call of method to object
        \param[in] o object
        \param[in] p pointer to method
        \return created handler
     */
    template<typename _Object, typename _MethodPointer>
    sad::hfsm::AbstractHandler* addEnterHandler(_Object o, _MethodPointer p)
    {
        return handleEnterWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
    }
    /*! Adds handler for entering state as call of sequential calls of method to object
        \param[in] o object
        \param[in] f first pointer to method
        \param[in] g second pointer to method
        \return created handler
     */
    template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
    sad::hfsm::AbstractHandler* addEnterHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
    {
        return handleEnterWith(new sad::hfsm::MethodComposition<_Object, _MethodFPointer, _MethodGPointer>(o, f, g));
    }
    /*! Adds handler for leaving state as function call
        \param[in] h callable handler
        \return created handler
     */
    template<typename _Callable>
    sad::hfsm::AbstractHandler* addLeaveHandler(_Callable h)
    {
        return handleLeaveWith(new sad::hfsm::Function<_Callable>(h));
    }
    /*! Adds handler for leaving state as call of method to object
        \param[in] o object
        \param[in] p pointer to method
        \return created handler
     */
    template<typename _Object, typename _MethodPointer>
    sad::hfsm::AbstractHandler* addLeaveHandler(_Object o, _MethodPointer p)
    {
        return handleLeaveWith(new sad::hfsm::Method<_Object, _MethodPointer>(o, p));
    }
    /*! Adds handler for leaving state as call of sequential calls of method to object
        \param[in] o object
        \param[in] f first pointer to method
        \param[in] g second pointer to method
        \return created handler
     */
    template<typename _Object, typename _MethodFPointer, typename _MethodGPointer>
    sad::hfsm::AbstractHandler* addLeaveHandler(_Object o, _MethodFPointer f, _MethodGPointer g)
    {
        return handleLeaveWith(new sad::hfsm::MethodComposition<_Object, _MethodFPointer, _MethodGPointer>(o, f, g));
    }
    /*! Enters a state
     */
    virtual void enter();
    /*! Leaves a state
     */
    virtual void leave();
    /*! Sets a shared data for a state
        \param[in] shared a shared data
     */
    virtual void setShared(sad::hfsm::Shared * shared);
    /*! Returns a shared data from a state
        \return shared data from a state
     */
    virtual sad::hfsm::Shared * shared() const;
    /*! Sets parent state for state
        \param[in] state a parent state
     */
    virtual void setParent(sad::hfsm::State * state);
    /*! Returns parent state for state (nullptr if it's top-level state)
        \return parent state
     */
    virtual sad::hfsm::State * parent() const;
    /*! Removes enter event handler
        \param[in] f abstract handler
     */
    virtual void removeEnterHandler(sad::hfsm::AbstractHandler * f);
    /*! Removes leave event handler
        \param[in] f abstract handler
     */
    virtual void removeLeaveHandler(sad::hfsm::AbstractHandler * f);
    /*! Sets state variable
        \param[in] name a name for variable
        \param[in] v a value for variable
     */
    virtual void setVariable(const sad::String& name, const sad::db::Variant& v);
    /*! Returns state variable
        \param[in] name a name for variable
        \return a variable
     */
    virtual sad::Maybe<sad::db::Variant> getVariable(const sad::String& name);
private:
    /*! A handler to call, when entering a state
     */
    sad::PtrVector<sad::hfsm::AbstractHandler> m_enter_handlers;
    /*! A handler to call, when leaving a state
     */
    sad::PtrVector<sad::hfsm::AbstractHandler> m_leave_handlers;
    /*! A shared data for a state
     */
    sad::hfsm::Shared  * m_shared;
    /*! A parent state
     */
    sad::hfsm::State  *  m_parent;
    /*! A children states in machine
     */
    sad::hfsm::StateMap m_children;
    /*! A machine, where state belongs to
     */
    sad::hfsm::Machine * m_machine;
    /*! A variables for a state
     */
    sad::Hash<sad::String, sad::db::Variant> m_variables;
};

}

}
