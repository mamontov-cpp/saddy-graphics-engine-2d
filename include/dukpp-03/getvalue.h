/*! \file getvalue.h

    Defines instantiations for pushing some values on stack of Duktape
 */
#pragma once
#include "basiccontext.h"
#include "../sadstring.h"
#include "../sadpoint.h"
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
}

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


}