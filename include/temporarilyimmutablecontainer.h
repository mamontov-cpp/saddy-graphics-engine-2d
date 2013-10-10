/*! \file temporarilyimmutablecontainer.h
	\author HiddenSeeker

	Describes a temporarily immutable container, which is locked for actions, in between of 
	performing some actions, like inserting or deleting or clearing a container.
 */
#pragma once
#include "os/mutex.h"
#include "sadvector.h"


namespace sad
{

template< typename _Object >
class TemporarilyImmutableContainer
{
 protected:
	 /*! A described command type for queue of commands
	  */
	 enum CommandType
	 {
		CT_ADD,
		CT_REMOVE,
		CT_CLEAR
	 };
	 /*! A command, which should be executed inside of queue
	  */
	 struct QueuedCommand
	 {
		 CommandType Type;  //!< An action, which should be performed
		 _Object * Object;  //!< An object, which action is applied to
	 };
	 /*! A queued commands container
	  */
	 sad::Vector<QueuedCommand> m_command_queue;
	 /*! A lock, for adding a commands into lock
	  */
	 ::os::mutex                  m_command_queue_lock;
	 /*! A container changing lock
	  */
	 ::os::mutex                 m_mutability_lock;
	 /*! A flag, whether actions is locked
	  */
	 bool                       m_lock_changes;
	 /*! Pushes new command to queue with specified type and object
		 \param[in] type a type of command
		 \param[in] o object
	  */
	 void pushCommand(CommandType type, _Object * o)
	 {
		QueuedCommand c;
		c.Type = type;
		c.Object = o;

		m_command_queue_lock.lock();
		m_command_queue << c;
		m_command_queue_lock.unlock();
	 }
	 /*! Locks a changes inside of container
	  */
	 inline void lockChanges() { m_lock_changes = true; }
	 /*! Unlocks a changes inside  of container
	  */
	 inline void unlockChanges() { m_lock_changes = false; }
	 /*! Tests, whether container is locked
		 \return whether container is locked
	  */
	 inline bool containerLocked() const { return m_lock_changes; }
	 /*! Immediately adds an object to container
		  \param[in] o object
	  */
	 virtual void addNow(_Object * o) = 0;
	 /*! Immediately removed an object from container
		  \param[in] o object
	  */
	 virtual void removeNow(_Object * o) = 0;
	 /*! Immediately clears a container
	  */
	 virtual void clearNow() = 0;
	 /*! Performs all queued actions
	  */
	 void performQueuedActions()
	 {
		m_command_queue_lock.lock();
		for(size_t i = 0; i < m_command_queue.count(); i++)
		{
			QueuedCommand & c = m_command_queue[i];
			switch(c.Type)
			{
				case CT_ADD : addNow(c.Object); break;
				case CT_REMOVE: removeNow(c.Object); break;
				case CT_CLEAR:  clearNow(); break;
			};
		}
		m_command_queue.clear();
	    m_command_queue_lock.unlock();
	 }
  public:
	  /*! As a default, container is mutable
	   */
	  TemporarilyImmutableContainer()
	  {
		 m_lock_changes = false;
	  }
	  /*! Adds new object to container
		  \param[in] o object
	   */
	  virtual void add(_Object * o)
	  {
		  if (m_lock_changes)
		  {
			  pushCommand(CT_ADD, o);
		  }
		  else
		  {
			  m_mutability_lock.lock();
			  addNow(o);
			  m_mutability_lock.unlock();
		  }
	  }

	   /*! Removes an object
		   \param[in] o object
	    */
	   virtual void remove(_Object * o)
	   {
		  if (m_lock_changes)
		  {
			  pushCommand(CT_REMOVE, o);
		  }
		  else
		  {
			  m_mutability_lock.lock();
			  removeNow(o);
			  m_mutability_lock.unlock();
		  }
	   }

	   /*! Clears a container
	    */
	   virtual void clear()
	   {
		  if (m_lock_changes)
		  {
			  pushCommand(CT_CLEAR, NULL);
		  }
		  else
		  {
			  m_mutability_lock.lock();
			  clearNow();
			  m_mutability_lock.unlock();
		  }
	   }

	  virtual ~TemporarilyImmutableContainer() {}
};

}
