/*! \file db/load,h
    
    
    Describes a load operations for a type
 */
#pragma once
#include "dberror.h"
#include "dbtypename.h"
#include "3rdparty/picojson/valuetotype.h"
#include "../object.h"

namespace sad
{

namespace db
{

/*! Load a value of specified type
    \param[in] ptr a value to be saved
    \param[in] v a special value, from which we should load stuff
    \return result
 */
template<
    typename _Type
>
bool Load<_Type>::perform(void * ptr, const picojson::value & v)
{
    if (!ptr)
    {
        throw sad::db::InvalidPointer();
    }
    if (sad::db::TypeName<_Type>::isSadObject())
    {
        return static_cast<sad::Object *>(ptr)->load(v);
    }
    throw sad::db::NotImplemented("sad::db::Load<_Type>::perform");
    // ReSharper disable once CppUnreachableCode
    return false;
}

/*! Declares template load for specified type
 */
#define DECLARE_DB_LOAD_FOR_TYPE( TYPE )                         \
template<>                                                       \
class Load< TYPE >                                               \
{                                                                \
public:                                                          \
static bool perform(void * ptr, const picojson::value & v)       \
{                                                                \
    if (!ptr)                                                    \
        throw sad::db::InvalidPointer();                         \
    sad::Maybe< TYPE >  cast = picojson::to_type< TYPE >(v);     \
    if (cast.exists())                                           \
    {                                                            \
        *( TYPE *)ptr = cast.value();                            \
    }                                                            \
    return cast.exists();                                        \
}                                                                \
};                

DECLARE_DB_LOAD_FOR_TYPE(bool)
DECLARE_DB_LOAD_FOR_TYPE(char)
DECLARE_DB_LOAD_FOR_TYPE(signed char)
DECLARE_DB_LOAD_FOR_TYPE(unsigned char)
DECLARE_DB_LOAD_FOR_TYPE(short)
DECLARE_DB_LOAD_FOR_TYPE(unsigned short)
DECLARE_DB_LOAD_FOR_TYPE(int)
DECLARE_DB_LOAD_FOR_TYPE(unsigned int)
DECLARE_DB_LOAD_FOR_TYPE(long)
DECLARE_DB_LOAD_FOR_TYPE(unsigned long)
DECLARE_DB_LOAD_FOR_TYPE(long long)
DECLARE_DB_LOAD_FOR_TYPE(unsigned long long)
DECLARE_DB_LOAD_FOR_TYPE(float)
DECLARE_DB_LOAD_FOR_TYPE(double)
DECLARE_DB_LOAD_FOR_TYPE(std::string)
DECLARE_DB_LOAD_FOR_TYPE(sad::String)
DECLARE_DB_LOAD_FOR_TYPE(sad::Point2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Point2I)
DECLARE_DB_LOAD_FOR_TYPE(sad::Point3D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Rect2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Color)
DECLARE_DB_LOAD_FOR_TYPE(sad::AColor)
DECLARE_DB_LOAD_FOR_TYPE(sad::Size2D)
DECLARE_DB_LOAD_FOR_TYPE(sad::Size2I)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::Vector<sad::AColor> >)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::Point2D>)
DECLARE_DB_LOAD_FOR_TYPE(sad::dialogue::Phrase)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::dialogue::Phrase*>)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<unsigned long long>)
DECLARE_DB_LOAD_FOR_TYPE(sad::Vector<sad::String>)
DECLARE_DB_LOAD_FOR_TYPE(sad::layouts::LengthValue)


template<typename T>                                                       
class Load< sad::Vector<T> >                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                                
    if (!ptr)                                                    
        throw sad::db::InvalidPointer();                         
    sad::Maybe< sad::Vector<T> >  cast = picojson::ValueToType<sad::Vector<T> >::get(v);     
    if (cast.exists())                                           
    {                                                            
        *static_cast<sad::Vector<T> *>(ptr) = cast.value();                            
    }                                                            
    return cast.exists();                                        
}                                                                
};   

template<typename T1, typename T2>                                                       
class Load< sad::Pair<T1, T2> >                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                                
    if (!ptr)                                                    
        throw sad::db::InvalidPointer();                         
    sad::Maybe< sad::Pair<T1, T2> >  cast = picojson::ValueToType<sad::Pair<T1, T2> >::get(v);     
    if (cast.exists())                                           
    {                                                            
        *static_cast<sad::Pair<T1, T2> *>(ptr) = cast.value();                            
    }                                                            
    return cast.exists();                                        
}                                                                
}; 


template<typename T1, typename T2, typename T3>                                                       
class Load< sad::Triplet<T1, T2, T3> >                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                                
    if (!ptr)                                                    
        throw sad::db::InvalidPointer();                         
    sad::Maybe< sad::Triplet<T1, T2, T3> >  cast = picojson::ValueToType<sad::Triplet<T1, T2, T3> >::get(v);     
    if (cast.exists())                                           
    {                                                            
        *static_cast<sad::Triplet<T1, T2, T3> *>(ptr) = cast.value();                            
    }                                                            
    return cast.exists();                                        
}                                                                
};     

template<typename T1, typename T2, typename T3, typename T4>                                                       
class Load< sad::Quadruplet<T1, T2, T3, T4> >                                               
{                                                                
public:                                                          
static bool perform(void * ptr, const picojson::value & v)       
{                                                                
    if (!ptr)                                                    
        throw sad::db::InvalidPointer();                         
    sad::Maybe< sad::Quadruplet<T1, T2, T3, T4> >  cast = picojson::ValueToType<sad::Quadruplet<T1, T2, T3, T4> >::get(v);     
    if (cast.exists())                                           
    {                                                            
        *static_cast<sad::Quadruplet<T1, T2, T3, T4> *>(ptr) = cast.value();                            
    }                                                            
    return cast.exists();                                        
}                                                                
};   

}

}
