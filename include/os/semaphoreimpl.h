/*! \file      semaphoreimpl.h
    \author    mamontov-cpp

    Defines a semaphore as simple synchonization primitive. 
    Semaphore blocks execution, when
    it's value less than zero, and does not
    block execution otherwise.
*/
#pragma once

#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <sys/stat.h>
    #include  <semaphore.h>
#endif

namespace sad
{

namespace os
{

/*!  \class SemaphoreImpl
     
     A  semaphore is  simple synchronization primitive. 
    Semaphore blocks execution, when
    it's value less than zero, and does not
    block execution otherwise
*/
class SemaphoreImpl
{
public:
    /*! Constructs new semaphore with specified values 
        \param[in] currentvalue current value of semaphore
        \param[in] maxvalue maximum value of semaphore
     */
    SemaphoreImpl(unsigned int currentvalue, unsigned int maxvalue); 
    /*! Frees memory from implementation part
     */
    ~SemaphoreImpl();
    /*! Consumes specified amount of resources, decrementing semaphore value. 
        Will block execution, when semaphore becomes less or equal to zero
        \param[in] amount how much should be consumed
     */ 
    void consume(unsigned int amount);
    /*! Increments semaphore value
        \param[in] amount how much should value increase. Does not block execution
     */
    void release(unsigned int amount);
    /*! Returns current value of semaphore.
        Can block execution in some implementations
        \return current value of semaphore
     */
    int    value() const;
protected:

#ifdef WIN32
    int    m_max_value; //!< Maximum value, which can be hold in semaphore
    HANDLE m_s;			//!< Semaphore
#else
    sem_t  m_s; //!< Semaphore
#endif

private:
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other mutex
     */
    SemaphoreImpl(const sad::os::SemaphoreImpl & o);
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other mutex
        \return self-reference
     */	
    sad::os::SemaphoreImpl & operator=(const sad::os::SemaphoreImpl & o);
};

}

}

