/*! \file dbtypename.h
    

    Describes a simple template class for type name as string
 */
#pragma once
#include "../sadstring.h"

namespace sad
{

class Object;
    
namespace db
{
class Object;

namespace internal
{
    /*! Fetches cached type name for pointer by it's base name, or makes new one, if it does not exists
        \param[in] base_name name of base type
        \return name for pointer
     */
    const sad::String&  fetchTypeNameForPointer(const sad::String& base_name);
    
    /*! Fetches cached type name for template type with one parameter
        \param[in] type_name a type name
        \param[in] arg1 first argument
        \return cached type name
     */
    const sad::String&  fetchTypeNameForTemplate1(
        const sad::String& type_name,
        const sad::String& arg1
    );
    
    /*! Fetches cached type name for template type with two parameters
        \param[in] type_name a type name
        \param[in] arg1 first argument
        \param[in] arg2 second argument
        \return cached type name
     */
    const sad::String&  fetchTypeNameForTemplate2(
        const sad::String& type_name,
        const sad::String& arg1,
        const sad::String& arg2		
    );
    /*! Fetches cached type name for template type with three parameters
        \param[in] type_name a type name
        \param[in] arg1 first argument
        \param[in] arg2 second argument
        \param[in] arg3 third argument		
        \return cached type name
     */
    const sad::String&  fetchTypeNameForTemplate3(
        const sad::String& type_name,
        const sad::String& arg1,
        const sad::String& arg2,	
        const sad::String& arg3			
    );	
    /*! Fetches cached type name for template type with one parameter
        \param[in] type_name a type name
        \param[in] arg1 first argument
        \param[in] arg2 second argument
        \param[in] arg3 third argument
        \param[in] arg4 fourth argument		
        \return cached type name
     */
    const sad::String&  fetchTypeNameForTemplate4(
        const sad::String& type_name,
        const sad::String& arg1,
        const sad::String& arg2,	
        const sad::String& arg3,		
        const sad::String& arg4			
    );		
}

template<
    typename _Type
>
class TypeName
{
public:

    /*! Yes value for enum
     */
    typedef char yes;
    /*! No value for enum
     */
    typedef long long no;

    static yes check(sad::Object*);
    static no  check(...);

    enum { SFINAE_BASE_CHECK = sizeof(check(static_cast<_Type*>(0))) == sizeof(yes) };


    /*! A special name for a type 
     */
    static sad::String Name;
    /*! A basic name for a pointer type (equal to a Name for a plain type)
     */
    static sad::String BaseName;
    /*! Whether type is an object (or points to it for pointers)
     */
    static const bool IsSadObject;
    /*! A default constructor, being called just to make sure, that static fields of class are initialized
     */
    static inline  void init()
    {
    }
    /*! Returns whether base type (without pointer size) is sad::Object descendant
        \return whether type is sad sad::Object descendant
     */
    static inline bool isSadObject()
    {
        return sad::db::TypeName<_Type>::IsSadObject;
    }

    /*! Returns name for a type name
     */ 
    static inline const sad::String& name()
    {
        return  sad::db::TypeName<_Type>::Name;
    }
    /*! Returns base name for a type
     */
    static inline const sad::String& baseName()
    {
        return sad::db::TypeName<_Type>::BaseName;
    }
    /*! Defines, whether object cast could be applied
     */
    enum ObjectCastValueHelper
    {
        CAN_BE_CASTED_TO_OBJECT  = false,
        POINTER_STARS_COUNT = 0
    };
};

template<
    typename _Type
>
class TypeName<_Type *>
{
public:

    enum { SFINAE_BASE_CHECK = false };

    /*! Defines, whether sad::Object cast could be applied
     */
    enum  ObjectCastValueHelper
    {
        CAN_BE_CASTED_TO_OBJECT  = (sad::db::TypeName<_Type>::POINTER_STARS_COUNT == 0) && sad::db::TypeName<_Type>::SFINAE_BASE_CHECK,
        POINTER_STARS_COUNT = (sad::db::TypeName<_Type>::POINTER_STARS_COUNT + 1),
    };
    /*! A default constructor, being called just to make sure, that static fields of class are initialized
     */
    static inline void init()
    {

    }
    /*! Returns name for a type name
     */ 
    static inline const sad::String& name()
    {
        sad::db::TypeName<_Type>::init();
        return sad::db::internal::fetchTypeNameForPointer(sad::db::TypeName<_Type>::name());
    }
    /*! Returns base name for a type
     */
    static inline const sad::String& baseName()
    {
        sad::db::TypeName<_Type>::init();
        return sad::db::TypeName<_Type>::baseName();
    }

    /*! Returns whether base type (without pointer size) is sad::Object descendant
        \return whether type is sad sad::Object descendant
     */
    static inline bool isSadObject()
    {
        return sad::db::TypeName<_Type>::isSadObject();
    }

    
};

template<
    typename _Type
>
struct IsSadObject
{
    static const bool value = false;
};

}

}

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT_ENUM             
#define DECLARE_TYPE_AS_SAD_OBJECT_ENUM(TYPE)        \
namespace sad { namespace db{ template<> struct IsSadObject< TYPE > { static const bool value = true; }; } }; 
#endif

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT
/*! Declares type compile-time metadata as sad::Object descendant
 */
#define DECLARE_TYPE_AS_SAD_OBJECT(TYPE)                                \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
template<> const bool sad::db::TypeName< TYPE >::IsSadObject  = true;   \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;            
#endif

#ifndef  DECLARE_TYPE_AS_SAD_OBJECT_EXPLICIT
/*! Declares type compile-time metadata as sad::Object descendant for types, which has
    multiple inheritance
 */
#define DECLARE_TYPE_AS_SAD_OBJECT_EXPLICIT(TYPE)            \
sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;        \
const bool sad::db::TypeName< TYPE >::IsSadObject  = true;   \
sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;            
#endif

#ifndef  DECLARE_COMMON_TYPE
/*! Declares type compile-time metadata as not a sad::Object descendant
 */
#define DECLARE_COMMON_TYPE(TYPE)                                             \
template<> sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;              \
template<> const bool sad::db::TypeName< TYPE >::IsSadObject  = false;        \
template<> sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;          
#endif

/*! Sometimes, it's not enough to just declare object. It could fail, if there is
    multiple inheritance from parent object. In that case, explicitly state parent type
    in PARENT parameter of this macro and TYPE for common type
 */
#define DECLARE_DB_TYPENAME_VIA_EXPLICIT_PARENT( PARENT, TYPE )               \
namespace sad                                                                 \
{                                                                             \
                                                                              \
namespace db                                                                  \
{                                                                             \
                                                                              \
template<>                                                                    \
class TypeName< TYPE >                                                        \
{                                                                             \
public:                                                                           \
    enum { SFINAE_BASE_CHECK = sad::db::TypeName< PARENT >::SFINAE_BASE_CHECK };  \
                                                                                  \
    static sad::String Name;                                                      \
    static sad::String BaseName;                                                  \
    static const bool IsSadObject;                                                \
    static inline  void init() { }                                                \
    static inline bool isSadObject()                                              \
    {                                                                             \
        return sad::db::TypeName< TYPE >::IsSadObject;                            \
    }                                                                             \
    static inline const sad::String& name()                                       \
    {                                                                             \
        return  sad::db::TypeName< TYPE >::Name;                                  \
    }                                                                             \
    static inline const sad::String& baseName()                                   \
    {                                                                             \
        return sad::db::TypeName< TYPE >::BaseName;                               \
    }                                                                             \
    enum ObjectCastValueHelper                                                    \
    {                                                                             \
        CAN_BE_CASTED_TO_OBJECT  = false,                                         \
        POINTER_STARS_COUNT = 0                                                   \
    };                                                                            \
};                                                                                \
                                                                                  \
}                                                                                 \
                                                                                  \
}

#define    DEFINE_DEFAULT_INSTANTIATION(TYPE)                                     \
namespace sad                                                                     \
{                                                                                 \
                                                                                  \
namespace db                                                                      \
{                                                                                 \
                                                                                  \
template<                                                                         \
>                                                                                 \
class TypeName<                                                                   \
TYPE                                                                              \
>                                                                                 \
{                                                                                 \
public:                                                                           \
    typedef char yes;                                                             \
    typedef long long no;                                                         \
                                                                                  \
    static yes check(sad::Object*);                                               \
    static no  check(...);                                                        \
                                                                                  \
    enum { SFINAE_BASE_CHECK = false };                                           \
                                                                                  \
                                                                                  \
    static sad::String Name;                                                      \
    static sad::String BaseName;                                                  \
    static const bool IsSadObject;                                                \
    static inline  void init()                                                    \
    {                                                                             \
    }                                                                             \
    static inline bool isSadObject()                                              \
    {                                                                             \
        return IsSadObject;                                                       \
    }                                                                             \
    static inline const sad::String& name()                                       \
    {                                                                             \
        return  Name;                                                             \
    }                                                                             \
    static inline const sad::String& baseName()                                   \
    {                                                                             \
        return BaseName;                                                          \
    }                                                                             \
    enum ObjectCastValueHelper                                                    \
    {                                                                             \
        CAN_BE_CASTED_TO_OBJECT  = false,                                         \
        POINTER_STARS_COUNT = 0                                                   \
    };                                                                            \
};                                                                                \
                                                                                  \
}                                                                                 \
                                                                                  \
}


#ifndef  DECLARE_INSTANTIATED_COMMON_TYPE
/*! Declares type compile-time metadata as not a sad::Object descendant
 */
#define DECLARE_INSTANTIATED_COMMON_TYPE(TYPE)                     \
sad::String sad::db::TypeName< TYPE >::Name  = #TYPE;              \
const bool sad::db::TypeName< TYPE >::IsSadObject  = false;        \
sad::String sad::db::TypeName< TYPE >::BaseName  = #TYPE;          
#endif

#include "internal/pairdefinitions.h"
