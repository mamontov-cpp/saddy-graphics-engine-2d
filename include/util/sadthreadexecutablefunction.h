#include <functional>

namespace sad
{

namespace util
{

/*! Defines an empty code, which only returns zero on execution
 */
class EmptyThreadExecutableFunction
:public sad::AbstractThreadExecutableFunction 
{
public:
    /*! Returns zero
        \return 0
     */
    virtual int execute() override;
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override;
};

/*! Executes a function with zero arguments and returns zero
 */
class FreeZeroArgVoidExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
     */
    inline FreeZeroArgVoidExecutableFunction(void (*f)()) : m_f(f)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override;
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override;
protected:
    void (*m_f)();
};

/*! Executes a function with zero arguments and returns code
 */
class FreeZeroArgIntExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
     */
    inline FreeZeroArgIntExecutableFunction(int (*f)()) : m_f(f)
    {
    }
    /*! Executes code  and returns returned function
        \return 0
     */
    virtual int execute() override;
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override;
protected:
    int (*m_f)();
};

/*! Executes a function with zero arguments and returns zero
*/
template<
    typename _FunctionType
>
class FreeZeroArgStdExecutableFunction
    :public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
    */
    inline FreeZeroArgStdExecutableFunction(const std::function<_FunctionType>& f) : m_f(f)
    {
    }
    /*! Executes code  and returns zero
        \return 0
    */
    virtual int execute() override
    {
        (m_f)();
        return 0;
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
    */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeZeroArgStdExecutableFunction<_FunctionType>(*this);
    }
protected:
    std::function<_FunctionType> m_f;
};

/*! Executes a function with zero arguments and returns zero
*/
class FreeZeroArgStdIntExecutableFunction
    :public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
    */
    inline FreeZeroArgStdIntExecutableFunction(const std::function<int()>& f) : m_f(f)
    {
    }
    /*! Executes code  and returns zero
        \return 0
    */
    virtual int execute() override
    {
        return (m_f)();
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
    */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeZeroArgStdIntExecutableFunction(*this);
    }
protected:
    std::function<int()> m_f;
};

/*! Executes a function with one argument and returns zero
 */
template<
    typename _FunctionArg,
    typename _CalledArg
>
class FreeOneArgVoidExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
        \param[in] a argument
     */
    inline FreeOneArgVoidExecutableFunction(
        void (*f)(_FunctionArg),
        const _CalledArg & a
        ) : m_f(f), m_arg(a)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        m_f(m_arg);
        return 0;
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeOneArgVoidExecutableFunction<_FunctionArg, _CalledArg>(*this);
    }
protected:
    void (*m_f)(_FunctionArg);
    _CalledArg m_arg;
};

/*! Executes a function with one argument and returns her returned code
 */
template<
    typename _FunctionArg,
    typename _CalledArg
>
class FreeOneArgIntExecutableFunction
: public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] f function, which will be executed
        \param[in] a argument
     */
    inline FreeOneArgIntExecutableFunction(
        int (*f)(_FunctionArg),
        const _CalledArg & a
        ) : m_f(f), m_arg(a)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        return m_f(m_arg);
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeOneArgIntExecutableFunction<_FunctionArg, _CalledArg>(*this);
    }
protected:
    int (*m_f)(_FunctionArg);
    _CalledArg m_arg;
};


/*! Executes a method with zero argument and returns zero
 */
template<
    typename _ClassName,
    typename _InvokedClassName
>
class FreeZeroArgVoidMethodExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] o an object, where method will be called
        \param[in] m a method of object
     */
    inline FreeZeroArgVoidMethodExecutableFunction(
        _ClassName * o,
        void (_InvokedClassName::*m)()
    ) : m_o(o), m_m(m)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        (m_o->*m_m)();
        return 0;
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeZeroArgVoidMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName>
        (*this);
    }
protected:
    _ClassName * m_o;
    void (_InvokedClassName::*m_m)();
};

/*! Executes a method with zero argument and returns it's returned code
 */
template<
    typename _ClassName,
    typename _InvokedClassName
>
class FreeZeroArgIntMethodExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] o an object, where method will be called
        \param[in] m a method of object
     */
    inline FreeZeroArgIntMethodExecutableFunction(
        _ClassName * o,
        int (_InvokedClassName::*m)()
    ) : m_o(o), m_m(m)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        return (m_o->*m_m)();
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeZeroArgIntMethodExecutableFunction<
            _ClassName, 
            _InvokedClassName>
        (*this);
    }
protected:
    _ClassName * m_o;
    int (_InvokedClassName::*m_m)();
};

/*! Executes a method with one argument and returns zero
 */
template<
    typename _ClassName,
    typename _InvokedClassName,
    typename _MethodArg,
    typename _InvokedArg
>
class FreeOneArgVoidMethodExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] o an object, where method will be called
        \param[in] m a method of object
        \param[in] a argument of method
     */
    inline FreeOneArgVoidMethodExecutableFunction(
        _ClassName * o,
        void (_InvokedClassName::*m)(_MethodArg),
        const _InvokedArg & a
    ) : m_o(o), m_m(m), m_a(a)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        (m_o->*m_m)(m_a);
        return 0;
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeOneArgVoidMethodExecutableFunction<
            _ClassName,
            _InvokedClassName,
            _MethodArg,
            _InvokedArg
        > 
        (*this);
    }
protected:
    _ClassName * m_o;
    void (_InvokedClassName::*m_m)(_MethodArg);
    _InvokedArg m_a;
};

/*! Executes a method with one argument and returns the code, which is returned by it's
    argument
 */
template<
    typename _ClassName,
    typename _InvokedClassName,
    typename _MethodArg,
    typename _InvokedArg
>
class FreeOneArgIntMethodExecutableFunction
:public sad::AbstractThreadExecutableFunction
{
public:
    /*! Creates a function
        \param[in] o an object, where method will be called
        \param[in] m a method of object
        \param[in] a argument of method
     */
    inline FreeOneArgIntMethodExecutableFunction(
        _ClassName * o,
        int (_InvokedClassName::*m)(_MethodArg),
        const _InvokedArg & a
    ) : m_o(o), m_m(m), m_a(a)
    {
    }
    /*! Executes code  and returns zero
        \return 0
     */
    virtual int execute() override
    {
        return (m_o->*m_m)(m_a);
    }
    /*! Creates a clone of executable function
        \returns exact copy of current thread executable function
     */
    virtual AbstractThreadExecutableFunction * clone() const override
    {
        return new sad::util::FreeOneArgIntMethodExecutableFunction<
            _ClassName,
            _InvokedClassName,
            _MethodArg,
            _InvokedArg
        > 
        (*this);
    }
protected:
    _ClassName * m_o;
    int (_InvokedClassName::*m_m)(_MethodArg);
    _InvokedArg m_a;
};

}

}
