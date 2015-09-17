#include <sadscopedlock.h>
#include <sadmutex.h>

sad::ScopedLock::ScopedLock(sad::Mutex * m)
: m_m(m)
{
    m_m->lock();
}

sad::ScopedLock::~ScopedLock()
{
    m_m->unlock();
}
