/*! \file hfsmtransitionrepository.h
    

    A transition repository for hierarchical finite state machine is placed here
 */
#pragma once
#include "hfsmtransition.h"

#include "../sadhash.h"
#include "../sadstring.h"

namespace sad
{

namespace hfsm
{

class Machine;

/*! A transition repository for storing and getting a transitions
 */
class TransitionRepository
{
public:
    /*! Constructs empty repository
     */
    TransitionRepository();
    /*! Frees all transitions from repository
     */
    virtual ~TransitionRepository();
    /*! Sets a linked finite state machine to repository
        \param[in] machine a finite state machine
     */
    virtual void setMachine(sad::hfsm::Machine * machine);
    /*! Returns a linked finite state machine to repository
        \return finite state machine for repository
     */
    virtual sad::hfsm::Machine * machine() const;
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
    /*! Invokes a transition between two states
        \param[in] from a full name of state from which transition is performed
        \param[in] to   a full name of state to which transition is performed
     */
    virtual void invoke(
        const sad::String & from, 
        const sad::String & to
    );
    /*! Tests, whether we could remove all state mentions
     */
    virtual void removeStateMentions(
        const sad::String & state
    );
protected:
    /*! A transition map in repository
     */
    typedef sad::Hash<sad::String, sad::Hash<sad::String, sad::hfsm::Transition *> > TransitionMap;
    /*! Cleans a transitions list, removing transitions
        \param[in] t a transitions
     */
    void clearTransitions(const sad::Hash<sad::String, sad::hfsm::Transition *> & t);
    /*! A machine, linked to repository
     */
    sad::hfsm::Machine * m_machine;
    /*! A transitions
     */
    TransitionMap m_transitions;
};

}

}
