/*! \file       mutex.h
     \author    HiddenSeeker
     Contains crossplatform mutex implementation,  which uses pthread on Linux and WinAPI on Windows 
*/
#pragma once
#ifdef WIN32
          #include  <windows.h>
#else
          #include  <pthread.h>
#endif

namespace os
{
   /*! \class mutex
        Simple non-recursive mutex class
  */
   class mutex
   {
     private:
#ifdef WIN32
                   HANDLE                 m_m;  //!< Mutex
#else
                   pthread_mutex_t        m_m;  //!< Mutex
#endif
     public:
                    mutex(bool locked=false);
                    ~mutex();
                    void lock();
                    void unlock();
   };
}

