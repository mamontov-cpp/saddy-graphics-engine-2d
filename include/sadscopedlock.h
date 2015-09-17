/*! \file sadscopedlock.h
    
    
    Defines a simple scoped lock pattern, as described at http://www.cs.wustl.edu/~schmidt/PDF/ScopedLocking.pdf
 */
#pragma once

namespace sad
{

class Mutex;

/*! Scoped lock takes mutex lock, when created and unlocks it, when destroyed. 
    See http://www.cs.wustl.edu/~schmidt/PDF/ScopedLocking.pdf for details.
 */
class ScopedLock
{
public:
    /*! Takes a lock on mutex.
        \param[in] m mutex
     */
    ScopedLock(sad::Mutex * m);
    /*! Releases lock on mutex
     */
    ~ScopedLock();
protected:
    sad::Mutex * m_m;
private:
    /*! Disabled. DO NOT COPY OTHER LOCK!
        \param[in] o other lock
     */
    ScopedLock(const sad::ScopedLock & o);
    /*! Disabled. DO NOT COPY OTHER LOCK!
        \param[in] o other lock
        \return self-reference
     */
    sad::ScopedLock & operator=(const sad::ScopedLock & o);
};

}
