/*! \file pointercallback.h
    

    Describes a template callback for calling on resources.
 */
#pragma once


namespace sad
{

namespace util
{
    
/*! A basic callback for calling with pointer to T
 */
template<
    typename T
>
class PointerCallback
{
public:
    inline PointerCallback()
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(T * o) = 0;
    /*! Copies a pointer callback
        \return copy
     */
    virtual PointerCallback<T> * copy() = 0;
    /*! You can implement your own callback
     */
    virtual ~PointerCallback()
    {
        
    }
};

/*! A simple callback with no arguments
 */
template<
    typename T
>
class FreeZeroArgCallback: public PointerCallback<T>
{
public:
    /*! Creates new callback
        \param[in] f a function
     */
    inline FreeZeroArgCallback(void (*f)()) : PointerCallback<T>(), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(T * o) override
    {
        m_f();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<T> * copy() override
    {
        return new sad::util::FreeZeroArgCallback<T>(m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~FreeZeroArgCallback() override
    {
        
    }	
protected:
    void (*m_f)();
};

/*! A simple callback, which receives an argument
 */
template<
    typename T
>
class FreeOneArgCallback: public PointerCallback<T>
{
public:
    /*! Creates new callback
        \param[in] f a function
     */
    inline FreeOneArgCallback(void (*f)(T *)) : PointerCallback<T>(), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(T * o) override
    {
        m_f(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<T> * copy() override
    {
        return new sad::util::FreeOneArgCallback<T>(m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~FreeOneArgCallback() override
    {
        
    }	
protected:
    void (*m_f)(T *);
};


/*! A method call with no arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _CalledObject
>
class MethodZeroArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
     */
    inline MethodZeroArgCallback(_Object * o, void (_CalledObject::*f)()) 
    : PointerCallback<_Type>(), m_o(o), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (m_o->*m_f)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::MethodZeroArgCallback<_Type, _Object, _CalledObject>(m_o, m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~MethodZeroArgCallback() override
    {
        
    }	
protected:
    _Object * m_o;
    void (_CalledObject::*m_f)();
};

/*! A const method call with no arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _CalledObject
>
class ConstMethodZeroArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
     */
    inline ConstMethodZeroArgCallback(_Object * o, void (_CalledObject::*f)() const) 
    : PointerCallback<_Type>(), m_o(o), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (m_o->*m_f)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::ConstMethodZeroArgCallback<_Type, _Object, _CalledObject>(m_o, m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~ConstMethodZeroArgCallback() override
    {
        
    }	
protected:
    _Object * m_o;
    void (_CalledObject::*m_f)() const;
};

/*! A method call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _CalledObject
>
class MethodOneArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
     */
    inline MethodOneArgCallback(_Object * o, void (_CalledObject::*f)(_Type *)) 
    : PointerCallback<_Type>(), m_o(o), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (m_o->*m_f)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::MethodOneArgCallback<_Type, _Object, _CalledObject>(m_o, m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~MethodOneArgCallback() override
    {
        
    }	
protected:
    _Object * m_o;
    void (_CalledObject::*m_f)(_Type *);
};

/*! A const method call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _CalledObject
>
class ConstMethodOneArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
     */
    inline ConstMethodOneArgCallback(_Object * o, void (_CalledObject::*f)(_Type *) const) 
    : PointerCallback<_Type>(), m_o(o), m_f(f)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (m_o->*m_f)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::ConstMethodOneArgCallback<_Type, _Object, _CalledObject>(m_o, m_f);
    }
    /*! You can implement your own callback
     */
    virtual ~ConstMethodOneArgCallback()
    {
        
    }	
protected:
    _Object * m_o;
    void (_CalledObject::*m_f)(_Type *) const;
};


/*! A method composition call with zero arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionZeroArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionZeroArgCallback(
        _Object * o, 
        _RetObject (_FCalledObject::*f)(),
        void (_GCalledObject::*g)()
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionZeroArgCallback<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionZeroArgCallback()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)();
    void (_GCalledObject::*m_g)();
};

/*! A method composition call with zero arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionZeroArgCallbackC1: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionZeroArgCallbackC1(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const,
        void (_GCalledObject::*g)()
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionZeroArgCallbackC1<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionZeroArgCallbackC1()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)() const;
    void (_GCalledObject::*m_g)();
};

/*! A const method composition call with zero arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionZeroArgCallbackC2: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionZeroArgCallbackC2(
        _Object * o, 
        _RetObject (_FCalledObject::*f)(),
        void (_GCalledObject::*g)() const
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionZeroArgCallbackC2<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionZeroArgCallbackC2()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)();
    void (_GCalledObject::*m_g)() const;
};

/*! A const method composition call with zero arguments
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionZeroArgCallbackC12: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionZeroArgCallbackC12(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const,
        void (_GCalledObject::*g)() const
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)();
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionZeroArgCallbackC12<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionZeroArgCallbackC12()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)() const;
    void (_GCalledObject::*m_g)() const;
};

/*! A method composition call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionOneArgCallback: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionOneArgCallback(
        _Object * o, 
        _RetObject (_FCalledObject::*f)(),
        void (_GCalledObject::*g)(_Type*)
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionOneArgCallback<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionOneArgCallback()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)();
    void (_GCalledObject::*m_g)(_Type*);
};

/*! A const method composition call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionOneArgCallbackC1: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionOneArgCallbackC1(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const,
        void (_GCalledObject::*g)(_Type*)
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionOneArgCallbackC1<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionOneArgCallbackC1()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)() const;
    void (_GCalledObject::*m_g)(_Type*);
};

/*! A const method composition call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionOneArgCallbackC2: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionOneArgCallbackC2(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() ,
        void (_GCalledObject::*g)(_Type*) const
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionOneArgCallbackC2<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionOneArgCallbackC2()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)() ;
    void (_GCalledObject::*m_g)(_Type*) const;
};

/*! A const method composition call with one argument
 */
template<
    typename _Type,
    typename _Object,
    typename _FCalledObject,
    typename _RetObject,
    typename _GCalledObject
>
class CompositionOneArgCallbackC12: public PointerCallback<_Type>
{
public:
    /*! Creates new callback
        \param[in] o object
        \param[in] f method
        \param[in] g second method
     */
    inline CompositionOneArgCallbackC12(
        _Object * o, 
        _RetObject (_FCalledObject::*f)() const,
        void (_GCalledObject::*g)(_Type*) const
    ) 
    : PointerCallback<_Type>(), m_o(o), m_f(f), m_g(g)
    {
        
    }
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(_Type * o) override
    {
        (((m_o->*m_f)()) ->* m_g)(o);
    }
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<_Type> * copy() override
    {
        return new sad::util::CompositionOneArgCallbackC12<_Type, _Object, _FCalledObject, _RetObject, _GCalledObject>(m_o, m_f, m_g);
    }
    /*! You can implement your own callback
     */
    virtual ~CompositionOneArgCallbackC12()
    {
        
    }	
protected:
    _Object * m_o;
    _RetObject (_FCalledObject::*m_f)() const;
    void (_GCalledObject::*m_g)(_Type*) const;
};

}

}
