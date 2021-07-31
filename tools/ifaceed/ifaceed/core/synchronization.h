/*! \file synchronization.h
    \author HiddenSeekeer

    Describes primitive for synchronization between
    editor's main thread and editor's renderer thread
    (SaddyThread)
 */
#pragma once
#include <QReadWriteLock>

namespace core
{

/*! A primitive for synchronization between editor's
    main thread and editor's renderer thread (SaddyThread)
 */
class Synchronization
{
public:
    /*! By default threads run non-synchronized, nobody
        waits nobody
     */
    Synchronization();
    /*! Frees memory from mutexes
     */
    ~Synchronization();
    /*! Sets flag of waiting for main thread to true
     */
    void startSynchronizationWithMainThread();
    /*! Sets flag of waiting for renderer's thread
        to true
     */
    void startSynchronizationWithSaddyThread();
    /*! Makes current thread wait for flag of waiting
        for main thread to be set to false
     */
    void waitForMainThread();
    /*! Makes current thread wait for flag of waiting
        for renderer's thread to be set to false
     */
    void waitForSaddyThread();
    /*! Sets flag of waiting for renderer's thread
        to false
     */
    void awakeMainThread();
    /*! Sets flag of waiting for main thread to false
     */
    void awakeSaddyThread();
private:
    /*! A flag, which defines, whether main thread
        should wait for renderer's thread
     */
    bool m_wait_for_saddy;
    /*! A flag, which defines, whether renderer's thread
        should wait for main thread
     */
    bool m_wait_for_main;
    /*! A lock for m_wait_for_saddy
     */
    QReadWriteLock m_wait_for_saddy_lock;
    /*! A lock for m_wait_for_main
     */
    QReadWriteLock m_wait_for_main_lock;
};

}
