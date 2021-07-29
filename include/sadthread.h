/*! \file sadthread.h
    

    Describes a cross-platform implementation of thread
 */
// ReSharper disable CppRedundantInlineSpecifier
#pragma once
#include "sadthreadexecutablefunction.h"

namespace sad
{

namespace os
{
    class ThreadImpl;
}

/*! Describes a cross-platform thread, that can be read
 */
class Thread
{
public:
    /*! A special code, which is returned, when thread is cancelled, or called
        pthread_exit or other functions
     */
    static const int Cancelled;
    /*! Creates new empty thread
     */
    inline Thread() // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        this->initialize(new sad::util::EmptyThreadExecutableFunction() );
    }
    /*! Initializes thread with empty executable function
        \param[in] f function
     */
    inline Thread(sad::AbstractThreadExecutableFunction * f) // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        this->initialize(f);
    }
    /*! Constructs new thread as copy of current. Also current data of running
        thread is copied
        \param[in] o other thread
     */
    Thread(const sad::Thread & o);
    /*! Copies thread. Stops old thread if running.
        \param[in] o other thread
        \return self-reference
     */
    sad::Thread& operator=(const sad::Thread & o);
    /*! Constructs new thread with specified function
        \param[in] f function
     */
    inline Thread(void (*f)() )  // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        this->initialize(new sad::util::FreeZeroArgVoidExecutableFunction(f) );
    }
    /*! Constructs new thread with specified function, returning it's code when exiting
        \param[in] f function
     */
    inline Thread(int (*f)() ) // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        this->initialize(new sad::util::FreeZeroArgIntExecutableFunction(f) );
    }
    /*! Constructs new thread with specified function
        \param[in] f function
     */
    template<
        typename _FunctionType
    >
    inline Thread(const std::function<_FunctionType>& f )
    {
        this->initialize(new sad::util::FreeZeroArgStdExecutableFunction<_FunctionType>(f) );
    }
    /*! Constructs new thread with specified function
        \param[in] f function
     */
    inline Thread(const std::function<int()>& f ) // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        this->initialize(new sad::util::FreeZeroArgStdIntExecutableFunction(f) );
    }
    /*! Creates new thread, executing void function with one argument
        \param[in] f function
        \param[in] a argument
     */
    template<
        typename _FunctionArg,
        typename _InvokedArg
    >
    inline Thread(void (*f)(_FunctionArg), const _InvokedArg & a)
    {
        this->initialize(new sad::util::FreeOneArgVoidExecutableFunction<_FunctionArg, _InvokedArg>(f, a));
    }
    /*! Creates new thread, executing int function with one argument and returning it's code
        as argument
        \param[in] f function
        \param[in] a argument
     */
    template<
        typename _FunctionArg,
        typename _InvokedArg
    >
    inline Thread(int (*f)(_FunctionArg), const _InvokedArg & a)
    {
        this->initialize(new sad::util::FreeOneArgIntExecutableFunction<_FunctionArg, _InvokedArg>(f, a));
    }
    /*! Creates new thread, which executes void method on specified object and returns zero
     */
    template<
        typename _ClassName,
        typename _InvokedClassName
    >
    inline Thread(
        _ClassName * o,
        void (_InvokedClassName::*m)()
    )
    {
        this->initialize(
            new sad::util::FreeZeroArgVoidMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName
            >(o, m)
        );
    }
    /*! Creates new thread, which executes int method on specified object and returning
        returned code
     */
    template<
        typename _ClassName,
        typename _InvokedClassName
    >
    inline Thread(
        _ClassName * o,
        int (_InvokedClassName::*m)()
    )
    {
        this->initialize(
            new sad::util::FreeZeroArgIntMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName
            >(o, m)
        );
    }
    /*! Creates new thread, which executes void method on specified object 
        with specified arg and returning zero
     */
    template<
        typename _ClassName,
        typename _InvokedClassName,
        typename _MethodArg,
        typename _InvokedArg
    >
    inline Thread(
        _ClassName * o,
        void (_InvokedClassName::*m)(_MethodArg),
        const _InvokedArg & a
    )
    {
        this->initialize(
            new sad::util::FreeOneArgVoidMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName,
            _MethodArg,
            _InvokedArg
            >(o, m, a)
        );
    }
    /*! Creates new thread, which executes int method on specified object 
        with specified arg and returning his code
     */
    template<
        typename _ClassName,
        typename _InvokedClassName,
        typename _MethodArg,
        typename _InvokedArg
    >
    inline Thread(
        _ClassName * o,
        int (_InvokedClassName::*m)(_MethodArg),
        const _InvokedArg & a
    )
    {
        this->initialize(
            new sad::util::FreeOneArgIntMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName,
            _MethodArg,
            _InvokedArg
            >(o, m, a)
        );
    }
    /*! Deletes implementation pointer
     */
    virtual ~Thread();
    /*! Runs a thread
        \return true if succeeded, otherwise false
     */
    bool run();
    /*! Stops a thread
     */
    void stop();
    /*! Blocks execution and tries to fetch result of execution of thread
     */
    int exitCode() const;
    /*! Blocks execution and waits until thread is stopped
     */
    void wait();
    /*! Blocks execution and waits for specified time, until thread is finished with execution,
        or time is expired
        \param[in] milliseconds how long should execution be blocked
     */
    void wait(int milliseconds);
    /*! Tests, whether thread  is finished
        \return whether thread finished his job
     */
    bool running() const;
protected:
    /*! A real implementation of thread
     */
    sad::os::ThreadImpl * m_dptr;
    /*! Initializes a thread with new executable function
        Note, that this is done with purpose of separating construction of sad::Thread
        with template argument from construction of platform-dependent sad::os::ThreadImpl
     */
    void initialize(sad::AbstractThreadExecutableFunction * f);
};


}
