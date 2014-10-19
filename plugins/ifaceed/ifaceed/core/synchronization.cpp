#include "synchronization.h"

core::Synchronization::Synchronization()
: m_waitforsaddy(false), m_waitformain(false),
  m_waitforsaddylock(QReadWriteLock::Recursive),
  m_waitformainlock(QReadWriteLock::Recursive)
{

}

core::Synchronization::~Synchronization()
{

}

void core::Synchronization::startSynchronizationWithMainThread()
{
    m_waitformainlock.lockForWrite();
    m_waitformain = true;
    m_waitformainlock.unlock();
}

void core::Synchronization::startSynchronizationWithSaddyThread()
{
    m_waitforsaddylock.lockForWrite();
    m_waitforsaddy = true;
    m_waitforsaddylock.unlock();
}

void core::Synchronization::waitForSaddyThread()
{
    bool shouldwait = true;
    while(shouldwait)
    {
        m_waitforsaddylock.lockForRead();
        shouldwait = m_waitforsaddy;
        m_waitforsaddylock.unlock();
    }
}

void core::Synchronization::waitForMainThread()
{
    bool shouldwait = true;
    while(shouldwait)
    {
        m_waitformainlock.lockForRead();
        shouldwait = m_waitformain;
        m_waitformainlock.unlock();
    }
}

void core::Synchronization::awakeMainThread()
{
    m_waitforsaddylock.lockForWrite();
    m_waitforsaddy = false;
    m_waitforsaddylock.unlock();
}

void core::Synchronization::awakeSaddyThread()
{
    m_waitformainlock.lockForWrite();
    m_waitformain = false;
    m_waitformainlock.unlock();
}


