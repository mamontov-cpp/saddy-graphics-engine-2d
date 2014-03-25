/*! \file sadmutex.h
	\author HiddenSeeker
	
	Defines a crossplatform nonrecursive mutex
 */
#pragma once
 
namespace sad
{
 
namespace os
{
class MutexImpl;
}
 
class Mutex
{
public:
	/*! Creates new non-locked mutex
	 */
	Mutex();
	/*! Mutex can not be copied, but for conformance with standard containers,
		this constructor creates new mutex.
		\param[in] o other mutex
	 */
	Mutex(const sad::Mutex & o);
	/*! Semaphore can not be copied, so assignment won't do anything.
		\param[in] o other mutex
		\return self-reference
	 */
	sad::Mutex & operator=(const sad::Mutex & o);
	/*! Frees memory from implementation part
	 */
	~Mutex();
	/*! Locks a mutex, blocking execution for other threads
	 */ 
	void lock();
	 /*! Unlocks mutex, allowing other threads to lock it.
      */
    void unlock();
protected:
	/*! Implementation part of semaphore
	 */
	sad::os::MutexImpl * m_dptr;
};
 
}
 