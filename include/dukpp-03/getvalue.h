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
#include "../p2d/bounds.h"
#include "../p2d/line.h"
#include "../p2d/rectangle.h"
#include "../p2d/circle.h"
#include "../p2d/collisionevent.h"
#include "../layouts/horizontalalignment.h"
#include "../layouts/verticalalignment.h"
#include "../layouts/stackingtype.h"
#include "../layouts/unit.h"
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
    \param[in] property_name a property name
    \return  value
 */
::dukpp03::Maybe<double> tryGetDoubleProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
);


/*! Tries to get property with int value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] property_name a property name
    \return  value
 */
::dukpp03::Maybe<int> tryGetIntProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
);

/*! Tries to get property with unsigned char value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] property_name a property name
    \return value
 */
::dukpp03::Maybe<unsigned char> tryGetUnsignedCharProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
);

/*! Tries to get property with unsigned char value from object
    \param[in] ctx context
    \param[in] pos position of object in stack
    \param[in] property_name a property name
    \return value
 */
::dukpp03::Maybe<sad::Point2D> tryGetPoint2DProperty(
    sad::dukpp03::BasicContext* ctx, 
    duk_idx_t pos,
    const char* property_name
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
            duk_size_t i = 0;
            const duk_size_t n = duk_get_length(ctx, pos);

            for (i = 0; i < n; i++) 
            {
                if (duk_has_prop_index(ctx, pos, static_cast<duk_uarridx_t>(i)))
                {
                    duk_get_prop_index(ctx, pos, static_cast<duk_uarridx_t>(i));
                    dukpp03::Maybe<_ValueType> val = dukpp03::GetValue<_ValueType, _Context>::perform(c, -1);
                    bool fail = false;
                    if (val.exists())
                    {
                        result.mutableValue().push_back(val.value());
                    }
                    else
                    {
                        result.clear();
                        fail = true;
                    }
                    duk_pop(ctx);
                    if (fail)
                    {
                        return;
                    }
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


/*! An instantiation for sad::p2d::Bound
*/
template<>
class GetValue<sad::p2d::Bound*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::Bound*> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};

/*! An instantiation for sad::p2d::Line
 */
template<>
class GetValue<sad::p2d::Line*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::Line*> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};

/*! An instantiation for sad::p2d::Rectangle
 */
template<>
class GetValue<sad::p2d::Rectangle*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::Rectangle*> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};

/*! An instantiation for sad::p2d::Circle
 */
template<>
class GetValue<sad::p2d::Circle*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::Circle*> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};

/*! An instantiation for sad::p2d::CollisionShape
*/
template<>
class GetValue<sad::p2d::CollisionShape*, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::CollisionShape*> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};


/*! An instantiation for sad::p2d::BasicCollisionEvent
*/
template<>
class GetValue<sad::p2d::BasicCollisionEvent, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::p2d::BasicCollisionEvent> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );

};


/*! An instantiation for sad::layouts::HorizontalAlignment
 */
template<>
class GetValue<sad::layouts::HorizontalAlignment, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    static ::dukpp03::Maybe<sad::layouts::HorizontalAlignment> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );
};

/*! An instantiation for sad::layouts::VerticalAlignment
 */
template<>
class GetValue<sad::layouts::VerticalAlignment, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    static ::dukpp03::Maybe<sad::layouts::VerticalAlignment> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );
};


/*! An instantiation for sad::layouts::StackingType
 */
template<>
class GetValue<sad::layouts::StackingType, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] ctx context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::layouts::StackingType> perform(
        sad::dukpp03::BasicContext* ctx,
        duk_idx_t pos
    );
};

/*! An instantiation for sad::layouts::Unit
 */
template<>
class GetValue<sad::layouts::Unit, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
    \param[in] ctx context
    \param[in] pos index for stack
    \return a value if it exists, otherwise empty maybe
    */
    static ::dukpp03::Maybe<sad::layouts::Unit> perform(
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

/*! Tries to get value with property index
    \param[out] result  a result
    \param[in] c context
    \param[in] pos position of object
    \param[in] index an index of property
 */
template<typename T>
void tryGetPropIndex(
    sad::Maybe<T>& result,
    sad::dukpp03::BasicContext* c,
    duk_idx_t pos,
    int index
)
{
    if (duk_has_prop_index(c->context(), pos, index))
    {
        duk_get_prop_index(c->context(), pos, index);
        dukpp03::Maybe<T> val = dukpp03::GetValue<T, sad::dukpp03::BasicContext>::perform(c, -1);
        if (val.exists())
        {
            result.setValue(val.value());
        }
        duk_pop(c->context());
    }
}

/*! An instantiation for sad::Pair<T1,T2>, which is array of two elements on stack
 */
template<
    typename T1, 
    typename T2
>
class GetValue<sad::Pair<T1, T2>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] c context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Pair<T1, T2> > perform(
        sad::dukpp03::BasicContext* c,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Pair<T1, T2> > result;
        duk_context* ctx = c->context();
        if (duk_is_array(ctx, pos))
        {
            // ReSharper disable once CppInitializedValueIsAlwaysRewritten
            const duk_size_t n = duk_get_length(ctx, pos);
            if (n == 2)
            {
                sad::Maybe<T1> r1;
                tryGetPropIndex(r1,c, pos, 0);
                sad::Maybe<T2> r2;
                tryGetPropIndex(r2,c, pos, 1);
                if (r1.exists() && r2.exists())
                {
                    result.setValue(sad::Pair<T1, T2>(r1.value(), r2.value()));
                }
            }            
        }
        return result;
    }
};



/*! An instantiation for sad::Triplet<T1,T2,T3>, which is array of two elements on stack
 */
template<
    typename T1, 
    typename T2,
    typename T3
>
class GetValue<sad::Triplet<T1, T2, T3>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] c context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Triplet<T1, T2, T3> > perform(
        sad::dukpp03::BasicContext* c,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Triplet<T1, T2, T3> > result;
        duk_context* ctx = c->context();
        if (duk_is_array(ctx, pos))
        {
            // ReSharper disable once CppInitializedValueIsAlwaysRewritten
            const duk_size_t n = duk_get_length(ctx, pos);
            if (n == 2)
            {
                sad::Maybe<T1> r1;
                tryGetPropIndex(r1,c, pos, 0);
                sad::Maybe<T2> r2;
                tryGetPropIndex(r2,c, pos, 1);
                sad::Maybe<T3> r3;
                tryGetPropIndex(r3,c, pos, 2);
                if (r1.exists() && r2.exists() && r3.exists())
                {
                    result.setValue(sad::Triplet<T1, T2, T3>(r1.value(), r2.value(), r3.value()));
                }
            }            
        }
        return result;
    }
};



/*! An instantiation for sad::Quadruplet<T1,T2,T3,T4>, which is array of two elements on stack
 */
template<
    typename T1, 
    typename T2,
    typename T3,
    typename T4
>
class GetValue<sad::Quadruplet<T1, T2, T3, T4>, sad::dukpp03::BasicContext>
{
public:
    /*! Performs getting value from stack
        \param[in] c context
        \param[in] pos index for stack
        \return a value if it exists, otherwise empty maybe
     */
    inline static dukpp03::Maybe<sad::Quadruplet<T1, T2, T3, T4> > perform(
        sad::dukpp03::BasicContext* c,
        duk_idx_t pos
    )
    {
        dukpp03::Maybe<sad::Quadruplet<T1, T2, T3, T4> > result;
        duk_context* ctx = c->context();
        if (duk_is_array(ctx, pos))
        {
            // ReSharper disable once CppInitializedValueIsAlwaysRewritten
            const duk_size_t n = duk_get_length(ctx, pos);
            if (n == 2)
            {
                sad::Maybe<T1> r1;
                tryGetPropIndex(r1,c, pos, 0);
                sad::Maybe<T2> r2;
                tryGetPropIndex(r2,c, pos, 1);
                sad::Maybe<T3> r3;
                tryGetPropIndex(r3,c, pos, 2);
                sad::Maybe<T3> r4;
                tryGetPropIndex(r4,c, pos, 3);
                if (r1.exists() && r2.exists() && r3.exists() && r4.exists())
                {
                    result.setValue(sad::Quadruplet<T1, T2, T3, T4>(r1.value(), r2.value(), r3.value(), r4.value()));
                }
            }            
        }
        return result;
    }
};

namespace internal
{


/*! An instantiation for allowing getting properties of sad::db::Object
 */
template<>
class TryGetValueFromObject<sad::db::Object, sad::dukpp03::BasicContext>
{
public:
/*! Performs getting value from stack 
    \param[in] ctx context
    \param[in] pos index for stack
    \param[out] result an output value. Won't be changed if result exists, will be set to result otherwise
 */
inline static void perform(sad::dukpp03::BasicContext* ctx, duk_idx_t pos, ::dukpp03::Maybe<sad::db::Object>& result)
{
	const ::dukpp03::Maybe<sad::db::Object*> o = ::dukpp03::GetValue<sad::db::Object*,  sad::dukpp03::BasicContext>::perform(ctx, pos);
    if (o.exists())
    {
        result.setReference(o.value());
    }
}

};

}

}