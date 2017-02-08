/*! \file getvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "../sadstring.h"
#include "../sadpoint.h"
#include "../sadcolor.h"
#include "../sadsize.h"
#include "../sadvector.h"
#include "../sadhash.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../db/save.h"
#include "../db/load.h"

namespace sad
{

namespace db
{

class Object;    

}

class Object;
    
}

namespace dukpp03
{

namespace internal
{

/*! Tries to get property with double value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] propname a property name
    \return 
 */
::dukpp03::Maybe<double> tryGetDoubleProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
);


/*! Tries to get property with int value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] propname a property name
    \return 
 */
::dukpp03::Maybe<int> tryGetIntProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
);

/*! Tries to get property with unsigned char value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] propname a property name
    \return 
 */
::dukpp03::Maybe<unsigned char> tryGetUnsignedCharProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* propname
);

}


/*! Fetches linear structure from stack
 */
template<
    template<
        typename Value
    > 
    class _LinearStructure,
    typename _ValueType,
    typename _Context
>
class GetLinearStructure
{
public:
     /*! Performs getting value from stack
        \param[in] c context
        \param[in] pos index for stack
        \param[out] result a result value
     */
    inline static void perform(
        _Context* c,
        duk_idx_t pos,
        ::dukpp03::Maybe<_LinearStructure<_ValueType> >& result
    )
    {
        result.clear();
        duk_context* ctx = c->context();
        if (duk_is_array(ctx, pos))
        {
            result.setValue(_LinearStructure<_ValueType>());        
            // ReSharper disable once CppInitializedValueIsAlwaysRewritten
            duk_size_t i = 0, n = duk_get_length(ctx, pos);

            for (i = 0; i < n; i++) 
            {
                if (duk_has_prop_index(ctx, pos, i))
                {
                    duk_get_prop_index(ctx, pos, i);
                    dukpp03::Maybe<_ValueType> val = dukpp03::GetValue<_ValueType, _Context>::perform(c, -1);
                    if (val.exists())
                    {
                        result.mutableValue().push_back(val.value());
                    }
                    else
                    {
                        result.clear();
                        return;
                    }
                    duk_pop(ctx);
                }
            }
        }
    }
};

/*! Fetches dictionary structure from stack
 */
template<
    template<
        typename Key,
        typename Value
    > 
    class _DictionaryStructure,
    typename _ValueType,
    typename _Context
>
class GetDictionaryStructure
{
public:
     /*! Performs getting value from stack
        \param[in] c context
        \param[in] pos index for stack
        \param[out] result a result value
     */
    inline static void perform(
        _Context* c,
        duk_idx_t pos,
        dukpp03::Maybe<_DictionaryStructure<sad::String, _ValueType> >& result
    )
    {
        result.clear();
        duk_context* ctx = c->context();
        if (duk_is_object(ctx, pos))
        {
            result.setValue(_DictionaryStructure<sad::String, _ValueType>());
            duk_enum(ctx, pos, DUK_ENUM_OWN_PROPERTIES_ONLY);

            while (duk_next(ctx, -1 /*enum_index*/, 1 /*get_value*/)) 
            {
                sad::String key = duk_get_string(ctx, -2);
                dukpp03::Maybe<_ValueType> val = dukpp03::GetValue<_ValueType, _Context>::perform(c, -1);
                if (val.exists())
                {
                    result.mutableValue().insert(key, val.value());
                }
                else
                {
                    result.clear();
                    return;
                }
                duk_pop(ctx);
                duk_pop(ctx);
            }            
        }
    }
};

/*! An instantiation for getting constant char pointer
 */
template<>
class GetValue<const char*, sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack 
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
static dukpp03::Maybe<const char*> perform(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos
);
};


/*! An instantiation for getting sad::String
 */
template<>
class GetValue<sad::String, sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack 
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
static dukpp03::Maybe<sad::String> perform(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos
);
};


/*! An instantiation for sad::db::Object
 */
template<>
class GetValue<sad::db::Object*,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
static ::dukpp03::Maybe<sad::db::Object*> perform(
        sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};



/*! An instantiation for sad::Object
 */
template<>
class GetValue<sad::Object*,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Object*> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};

/*! An instantiation for variant
 */
template<>
class GetValue<sad::db::Variant,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::db::Variant> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};



/*! An instantiation for sad::Point2D
 */
template<>
class GetValue<sad::Point2D,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Point2D> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};

/*! An instantiation for sad::Point3D
 */
template<>
class GetValue<sad::Point3D,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Point3D> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};


/*! An instantiation for sad::Point2I
 */
template<>
class GetValue<sad::Point2I,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Point2I> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};


/*! An instantiation for sad::Point3I
 */
template<>
class GetValue<sad::Point3I,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Point3I> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};


/*! An instantiation for sad::Size2D
 */
template<>
class GetValue<sad::Size2D,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Size2D> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};

/*! An instantiation for sad::Size2D
 */
template<>
class GetValue<sad::Size2I,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Size2I> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};


/*! An instantiation for sad::Color
 */
template<>
class GetValue<sad::Color,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::Color> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};


/*! An instantiation for sad::AColor
 */
template<>
class GetValue<sad::AColor,  sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
 */
static ::dukpp03::Maybe<sad::AColor> perform(
    sad::dukpp03::BasicContext* ctx,
    duk_idx_t pos
);

};

/*!  An instantiation for sad::Vector<T>, which converts it from array
 */
template<typename T>
class GetValue<sad::Vector<T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Vector<T> > perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Vector<T> > result;
        dukpp03::GetLinearStructure<sad::Vector, T, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
        return result;
    }
};

/*!  An instantiation for sad::Vector<T>, which converts it from object
 */
template<typename T>
class GetValue<sad::Hash<sad::String, T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Hash<sad::String, T> > perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Hash<sad::String, T> > result;
        dukpp03::GetDictionaryStructure<sad::Hash, T, sad::dukpp03::BasicContext>::perform(ctx, pos, result);
        return result;
    }
};


/*! An instantiation for sad::Maybe<T>, which is null or T on stack
 */
template<typename T>
class GetValue<sad::Maybe<T>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Maybe<T> > perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Maybe<T> > result;
        if (duk_is_null(ctx->context(), pos))
        {
            result.setValue(sad::Maybe<T>());
        }
        else
        {
            dukpp03::Maybe<T> maybe_value = dukpp03::GetValue<T, sad::dukpp03::BasicContext>::perform(ctx, pos);
            if (maybe_value.exists())
            {
                result.setValue(sad::Maybe<T>(maybe_value.value()));
            }
        }
        return result;
    }
};

}