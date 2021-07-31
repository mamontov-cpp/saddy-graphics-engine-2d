#include "synchronization.h"

core::Synchronization::Synchronization()
: m_wait_for_saddy(false), m_wait_for_main(false),
  m_wait_for_saddy_lock(QReadWriteLock::Recursive),
  m_wait_for_main_lock(QReadWriteLock::Recursive)
{

}

core::Synchronization::~Synchronization() = default;

void core::Synchronization::startSynchronizationWithMainThread()
{
    m_wait_for_main_lock.lockForWrite();
    m_wait_for_main = true;
    m_wait_for_main_lock.unlock();
}

void core::Synchronization::startSynchronizationWithSaddyThread()
{
    m_wait_for_saddy_lock.lockForWrite();
    m_wait_for_saddy = true;
    m_wait_for_saddy_lock.unlock();
}

void core::Synchronization::waitForSaddyThread()
{
    bool should_wait = true;
    while (should_wait)
    {
        m_wait_for_saddy_lock.lockForRead();
        should_wait = m_wait_for_saddy;
        m_wait_for_saddy_lock.unlock();
    }
}

void core::Synchronization::waitForMainThread()
{
    bool should_wait = true;
    while (should_wait)
    {
        m_wait_for_main_lock.lockForRead();
        should_wait = m_wait_for_main;
        m_wait_for_main_lock.unlock();
    }
}

void core::Synchronization::awakeMainThread()
{
    m_wait_for_saddy_lock.lockForWrite();
    m_wait_for_saddy = false;
    m_wait_for_saddy_lock.unlock();
}

void core::Synchronization::awakeSaddyThread()
{
    m_wait_for_main_lock.lockForWrite();
    m_wait_for_main = false;
    m_wait_for_main_lock.unlock();
}


