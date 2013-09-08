/*! \file      thread.h
    \author    HiddenSeeker
    Contains crossplatform thread implementation,  which uses pthread on Linux and WinAPI on Windows 
*/
#pragma once
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include  <windows.h>
#else
    #include  <pthread.h>
#endif

#ifndef INFINITE   
	#define INFINITE 65535
#endif

namespace  os
{
   /*! Thread  function, used in this implementation */
   typedef  void * (*thread_function)(void *);
   
   /*! Class of thread, that provides a basic thread operations
   */
   class  thread
   {
#ifdef WIN32
    public:
	           /*! Information about thread
			   */
			   class info
			   {
			    public:
				       bool running;         //!< Whether thread is running
					   thread_function fun;  //!< Thread function
					   void * parameter;     //!< Parameter
					   /*! Creates an information
					       \param[in] f function
						   \param[in] p parameter
					   */
					   info(thread_function f=NULL, void * p=NULL);
			   };
    private:
			   os::thread::info m_info; //!< Info about thread
			   HANDLE m_thread;         //!< Thread
			   
			   static DWORD WINAPI proxy(LPVOID info);
#else
    public:
                 /*!  Information about thread
                 */
                class info
                {
                  public:
                              bool running;            //!< Whether is thread running
                              bool  waitable;         //!< Whether is thread waitable
                              void * result;            //!< Result of execution thread
                              thread_function fun; //!< Thread function
                              void * parameter;    //!<  Parameter
                               
                              /*! Constructs a thread info
                                   \param[in] _waitable whether we are going to save an information
                              */
                              info(bool _waitable=false);                             
                };
    private:
                  pthread_t             m_thread;
                  pthread_attr_t      m_attr;
                  os::thread::info     m_info;
                   
                  static void * proxy(void * info);    // Proxy thread function
#endif
    public:
                /*! Constructs a thread
                    \param[in] fun           function
                    \param[in] param      parameter for called thread
                    \param[in] waitable   Indicates, whether we going to supports for timed wait (wait without INFINITE)
                */
                thread(thread_function fun, void * param, bool waitable=false);
                /*! Destructor
                */
                ~thread();
                /*!  Detaches a thread. Must be called before os::thread::run()
                */
                void detach();
                /*! Makes a thread waitable. Must be called before os::thread::run()
                */
                void makeWaitable();
                /*! Runs a thread
                     \return true on success
                */
                bool run();
                /*! Stops a thread
                */
                void stop();
                /*! Waits for thread execution
                     NOTE: Timed wait needs an original thread object  and waitable set to true
                     \param[in] time in milliseconds
                     \return true on successful wait
                */
                bool wait(unsigned int time=INFINITE);
                /*! Returns a resulting thread exit code. If waitable, result is equal to os::thread::wait(INFINITE) and returning exit code.
                     NOTE: Needs an original thread object  and waitable set to true
                     \return NULL if not run, otherwise wait for execution
                */
                void * result();
   };           

}

