/*! \file getaddressoftype.h
    
    Gets address of specified type
 */
#pragma once
#include "../db/dbvariant.h"
#include "../3rdparty/dukpp-03.h"
#include "../animations/animationsanimations.h"

namespace sad
{

class Renderer;
class Window;

namespace resource
{

class Tree;
}

namespace pipeline
{

class Pipeline;

}

namespace layouts
{

class Cell;

}

namespace dukpp03
{

class Context;
class Renderer;
class Thread;
class JSControls;

namespace internal
{
/*! A local conversion table for all conversions
 */
extern sad::db::ConversionTable conversion_table;

}


/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue, bool _IsPointerToAbstractClass, bool _IsSadObject>
struct GetAddressOfType
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};

/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue, bool _IsSadObject>
struct GetAddressOfType<_UnderlyingValue, true, _IsSadObject>
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};


/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue, bool _IsAbstract>
struct GetAddressOfType<_UnderlyingValue, _IsAbstract, true>
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};


/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue>
struct GetAddressOfType<_UnderlyingValue, true, true>
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};


/*! Returns address of type stored in variant.
 */
template<typename _UnderlyingValue>
struct GetAddressOfType<_UnderlyingValue*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue*> getAddress(sad::db::Variant* v)
    {
        sad::Maybe<_UnderlyingValue> result = v->get<_UnderlyingValue>(true, &sad::dukpp03::internal::conversion_table);
        if (result.exists())
        {
            return ::dukpp03::Maybe<_UnderlyingValue*>(&(result.mutableValue()));
        }
        return ::dukpp03::Maybe<_UnderlyingValue*>();
    }
};

template<>
struct GetAddressOfType<sad::dukpp03::Context*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::dukpp03::Context*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::dukpp03::Context*>();
    }
};

template<>
struct GetAddressOfType<sad::Renderer*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::Renderer*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::Renderer*>();
    }
};

template<>
struct GetAddressOfType<sad::animations::Animations*, false, false>
{
public:
    /*! Returns address of type, stored in variant.
    \param[in] v value
    \return empty maybe
    */
    inline static ::dukpp03::Maybe<sad::animations::Animations*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::animations::Animations*>();
    }
};


template<>
struct GetAddressOfType<sad::dukpp03::Renderer*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::dukpp03::Renderer*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::dukpp03::Renderer*>();
    }
};

template<>
struct GetAddressOfType<sad::dukpp03::Thread*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::dukpp03::Thread*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::dukpp03::Thread*>();
    }
};


template<>
struct GetAddressOfType<sad::Window*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::Window*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::Window*>();
    }
};

template<>
struct GetAddressOfType<sad::resource::Tree*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::resource::Tree*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::resource::Tree*>();
    }
};

template<>
struct GetAddressOfType<sad::dukpp03::JSControls*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::dukpp03::JSControls*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::dukpp03::JSControls*>();
    }
};


template<>
struct GetAddressOfType<sad::pipeline::Pipeline*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::pipeline::Pipeline*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::pipeline::Pipeline*>();
    }
};

/*! Returns address of type stored in variant.
 */
template<>
struct GetAddressOfType<const char*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<const char*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<const char*>();
    }
};

/*! Returns address of type stored in variant.
*/
template<>
struct GetAddressOfType<sad::layouts::Cell*, false, false>
{
public:
    /*! Returns address of type, stored in variant - hence nothing for plain types
    \param[in] v value
    \return empty maybe
    */
    static ::dukpp03::Maybe<sad::layouts::Cell*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::layouts::Cell*>();
    }
};


}

}
