/*! \file sadsemaphore.h
    
    
    Defines a semaphore as simple synchonization primitive. Semaphore blocks execution, when
    it's value less than zero, and does not
    block execution otherwise.
 */
#pragma once

namespace sad
{

namespace os
{
class SemaphoreImpl;
}

class Semaphore
{
public:
    /*! Constructs new semaphore with specified values 
        \param[in] current_value current_value of semaphore
        \param[in] max_value maximum value of semaphore
     */
    Semaphore(unsigned int current_value = 0, unsigned int max_value = 32767);
    /*! Semaphore can not be copied, but for conformance with standard containers,
        this constructor creates new semaphore with default parameters
        \param[in] o other semaphore
     */
    Semaphore(const sad::Semaphore & o);
    /*! Semaphore can not be copied, so assignment won't do anything.
        \param[in] o other semaphore
        \return self-reference
     */
    sad::Semaphore & operator=(const sad::Semaphore & o);
    /*! Frees memory from implementation part
     */
    virtual ~Semaphore();
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
    int value() const;
protected:
    /*! Implementation part of semaphore
     */
    sad::os::SemaphoreImpl * m_dptr;
};

}
