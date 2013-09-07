/*! \file       semaphore.h
     \author    HiddenSeeker
     Contains crossplatform semaphore implementation
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


namespace os
{
 /*!  \class semaphore
       Simple semaphore class, which can wait on 0, and release to maximum value
 */
 class semaphore
 {
   private:
#ifdef WIN32
                int    m_v; //!< Max value
                HANDLE m_s; //!< Semaphore
#else
                sem_t  m_s; //!< Semaphore
#endif
   public:
                /*! Constructor
                     \param[in] beginvalue startingvalue of semaphore
                     \param[in] maxvalue   maximum value of semaphore
                */
                 semaphore(unsigned int beginvalue=0, unsigned int maxvalue=32767); 
                /*! Destructor
               */
               ~semaphore();
               /*! Consumes an amount of resources
                    \param[in] v amount of resource to be consumed
               */
               void consume(unsigned int v);
               /*! Releases an amount of resources
                    \param[in] v amount of resorces to be released
              */
               void release(unsigned int v);
              /*! Returns a current value of semaphore
                   \return value
              */ 
              int    value();
 };

}
