/*! \file hfsmmachine.h
    

    Includes definition of hierarchical finite state machine
 */
#pragma once
#include "hfsm/hfsmtransitionrepository.h"
#include "hfsm/hfsmstate.h"

#include "../input/handlerconditions.h"

#include "../pipeline/pipelinetask.h"

#include "../object.h"

namespace sad
{

namespace hfsm
{

/*! Defines a hierarchical state machine. By default machine is in state with empty name, which
    is a top-level state
 */ 
class Machine: public sad::Object
{
SAD_OBJECT
public:
    /*! Creates new empty machine 
     */
    Machine();
    /*! Destroys a machine
     */
    virtual ~Machine();	
    /*! Immediately enters new state. Does nothing if state does not exists.
        At moment of invocation current state of machine will point to new state name,
        with method is called
        and previous state to a state of machine. If state does not exists, nothing is done.
        If previous state of machine and current state of machine match, nothing is done
        \param[in] state full string of state, enumerated from top to bottom with "/" as delimiter.
                         Pass empty string for root state
     */
    virtual void enterState(const sad::String & state);
    /*! Returns state by it's full name enumerated from top to bottom with "/" as delimiter.
        Pass empty string for root state
        \param[in] s state path
        \return pointer to state (nullptr if not found)
     */ 
    virtual sad::hfsm::State * state(const sad::String & s);
    /*! Adds new state where full path determines a parent node to add, defined from top to bottom
        with "/". Any existing node will be replaced. Pass empty string for top node. Also you can
        force adding absent nodes with default states, setting force flag to true
        \param[in] full_path a path for parent node to add
        \param[in] state    a state to be added (nullptr will construct default state as object of class sad::hfsm:State)
        \param[in] force    force inserting new intermediate states
        \return whether state was successfully inserted
     */
    virtual bool addState(const sad::String& full_path, sad::hfsm::State * state = nullptr, bool force = false);
    /*! Removes a state by full path, that determines a parent node. 
        That top level state could be removed, but it will be replaced by new state, not preserving 
        hierarchy. If state full path is not found, it will be removed.
        \param[in] full_path node with full path
     */
    virtual void removeState(const sad::String& full_path);
    /*! Tests, whether state of hierarchical finite state machine exists.
        State defined as full path, separated by "/"
        \param[in] s string
        \return whether state exists
     */
    bool stateExists(const sad::String & s);
    /*! Returns a transition for hierarchical finite state machine
     */
    virtual sad::hfsm::TransitionRepository * transitions() const;
    /*! Adds a transition between states. Existing transition will be replaced
        \param[in] from a full name of state from which transition is performed
        \param[in] to   a full name of state to which transition is performed
        \param[in] t    a transition
     */
    virtual void addTransition(
        const sad::String & from, 
        const sad::String & to,
        sad::hfsm::Transition * t
    );
    /*! Removes a transition between states
        \param[in] from a full name of state from which transition is performed
        \param[in] to   a full name of state to which transition is performed
     */
    virtual void removeTransition(
        const sad::String & from, 
        const sad::String & to	
    );
    /*! Returns a transition between two states (nullptr if not found)
        \param[in] from a full name of state from which transition is performed
        \param[in] to   a full name of state to which transition is performed
     */
    virtual sad::hfsm::Transition * transition(
        const sad::String & from, 
        const sad::String & to
    );
    /*! Returns current state of hierarchical finite state machine
        \return current state
     */
    const sad::String & currentState() const;
    /*! Returns previous state of hierarchical finite state machine
        \return previous state
     */
    const sad::String & previousState() const;
    /*! Tests, whether current state is equal to specified, or parent of it
        \param[in] state a state
     */
    bool isInState(const sad::String & state) const;
protected:
    /*! A current state of machine
     */
    sad::String m_current_state;
    /*! A previous state of machine
     */
    sad::String m_previous_state;
    /*! A top-level state, which contains ALL of states
     */
    sad::hfsm::State * m_top;
    /*! Contains all of transitions
     */
    sad::hfsm::TransitionRepository * m_transitions;
};

/*! A special type of condition, which checks machine's current state
 */
class CurrentStateCheck: public sad::input::AbstractHandlerCondition
{
public:
    /*! Constructs new check
        \param[in] machine a machine, which state should be checked
        \param[in] state a state, which tests should be tested against
     */
    CurrentStateCheck(sad::hfsm::Machine * machine,  const sad::String & state);
    /*! Checks a condition against abstract event
        \param[in] e event
        \return whether we should  run an event
     */
    virtual bool check(const sad::input::AbstractEvent & e) override;
    /*! Clones a condition
        \return new condition, which should be exact copy of current
     */
    virtual sad::input::AbstractHandlerCondition * clone() override;
    /*! Frees state
     */
    virtual ~CurrentStateCheck() override;  // NOLINT(clang-diagnostic-deprecated-copy-dtor)
protected:
    /*! A machine, which will be checked with getting current state
     */
    sad::hfsm::Machine * m_machine;
    /*! A current state, which will be tested against current state
     */ 
    sad::String m_state;
};

/*! A special task, which switches machine state to defined by user
 */
class MachineStateChangeTask: public sad::pipeline::AbstractTask
{
public:
    /*! Constructs new task, which should switch machine's state to a defined one
        \param[in] machine a machine, which state should be checked
        \param[in] state a state, which tests should be tested against
     */
    MachineStateChangeTask(
        sad::hfsm::Machine * machine,
        const sad::String & state
    );
    /*! Destroys an object. 
     */
    virtual ~MachineStateChangeTask() override;
protected:
    /*! Switches machine state to defined by user
     */ 
    virtual void _process() override;
    /*! A machine, which will be checked with getting current state
     */
    sad::hfsm::Machine * m_machine;
    /*! A current state, which will be tested against current state
     */ 
    sad::String m_state;
};

}

}

/*! Creates new condition for comparing name of current state of machine with state name
    \param[in] machine  a machine, which current state is being compared
    \param[in] state    a state name
 */
sad::input::AbstractHandlerCondition * operator*(
    sad::hfsm::Machine * machine,
    const sad::String & state
);
