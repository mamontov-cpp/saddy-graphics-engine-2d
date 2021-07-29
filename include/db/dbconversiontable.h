/*! \file dbconversiontable.h
    

    Defines a table for conversion between types
 */
#pragma once
#include "../sadptrhash.h"
#include "../sadstring.h"
#include "../sadmutex.h"
#include "dbtypename.h"

namespace sad
{
    
namespace db
{

/*! An abstract class, which converts one class to other
 */
class AbstractTypeConverter
{
public:
/*! Must perform conversion and assignment from source type, in source pointer
    to destination type dest
    \param[in] source source converted type
    \param[in] dest destination converted type
 */
virtual void convert(void * source, void * dest) = 0;
/*! This class should be inherited
 */
virtual ~AbstractTypeConverter();
};

/**
 * A converter, which uses type cast operator to convert one type to another 
 */
template<
    typename _FromType,
    typename _ToType
>
class ImplicitTypeConverter: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
    inline ImplicitTypeConverter()
    {
        
    }
    /*! Converts source value from another and to another type
        \param[in] source source converted type
        \param[in] dest destination converted type
     */
    virtual void convert(void * source, void * dest)
    {
        *((_ToType *)dest) =  (_ToType)(*(_FromType *)source);
    }
    /*! Can be inherited
     */
    virtual ~ImplicitTypeConverter()
    {
        
    }
};

/**
 * A converter, which uses constructor to convert one type to another.
 * Required _ToType to be constructable from _FromType and have valid
 * assignment operator.
 */
template<
    typename _FromType,
    typename _ToType
>
class ConverterViaConstructor: public sad::db::AbstractTypeConverter
{
public:
    /*! Creates new converter
     */
    inline ConverterViaConstructor()
    {
        
    }
    /*! Converts source value from another and to another type
        \param[in] source source converted type
        \param[in] dest destination converted type
     */
    virtual void convert(void * source, void * dest)
    {
        *((_ToType *)dest) =  _ToType(*(_FromType *)source);
    }
    /*! Can be inherited
     */
    virtual ~ConverterViaConstructor()
    {
        
    }
};

/** A list of conversion table for conversions of types
 */
class ConversionTable
{
public:
    /*! Creates new conversion table
     */
    ConversionTable();
    /*! Destroys a conversion table
     */
    ~ConversionTable();
    /*! Inserts a conveter into table. Replaces an existing converter, erasing it, if
        needed
        \param[in] from a typename from which conversion is performed
        \param[in] to a typename, which we should confirm
        \param[in] c a converter
     */
    void add(const sad::String & from, 
             const sad::String & to, 
             sad::db::AbstractTypeConverter * c);
    /*! Declares implicit conversion from one type to other
     */
    template< typename _FromType, typename _ToType>
    void declareImplicit()
    {
        sad::db::TypeName<_FromType>::init();
        sad::db::TypeName<_ToType>::init();		
        add(sad::db::TypeName<_FromType>::name(), 
            sad::db::TypeName<_ToType>::name(),
            new sad::db::ImplicitTypeConverter<_FromType, _ToType>()
           );
    }
    /*! Gets converter from one type to another
        \param[in] from a typename for conversion from type
        \param[in] to a typename for conversion to 
        \return nullptr, if not found or converter
     */
    sad::db::AbstractTypeConverter * converter(
        const sad::String & from, 
        const sad::String & to
    );

    /*! Adds is sad object flag for type T to a conversion table
     */
    template<
        typename T
    >
    void declareIsSadObjectFlag() 
    {
        this->m_sad_object_flags_lock.lock();
        sad::db::TypeName<T>::init();
        if (m_sad_object_flags.contains(sad::db::TypeName<T>::name()) == false) 
        {
            m_sad_object_flags.insert(sad::db::TypeName<T>::name(), sad::db::TypeName<T>::isSadObject());	
        }
        this->m_sad_object_flags_lock.unlock();
    }
    /*! Tests, whether type is sad object
        \param[in] name name of type, which type must be determined
        \return whether it's sad object
     */
    inline bool isSadObject(const sad::String & name) const 
    {
        bool result = false;
        sad::db::ConversionTable* me = const_cast<sad::db::ConversionTable*>(this);
        me->m_sad_object_flags_lock.lock();
        if (m_sad_object_flags.contains(name))
        {
            result = m_sad_object_flags[name];
        }
        me->m_sad_object_flags_lock.unlock();
        return result;
    }
    /*! Returns instance of conversion table
        \return instance of table
     */
    static sad::db::ConversionTable * ref();
protected:
    /*! Frees instance of conversion table
     */
    static void freeInstance();
    /*! An instance for conversion table
     */
    static sad::db::ConversionTable * m_instance;
    /*! A list of conversions for table
     */
    sad::Hash<sad::String, sad::Hash<sad::String, sad::db::AbstractTypeConverter*> > m_converters;
    /*! A hash of types to flags, which indicate, whether type is sad object kind
     */
    sad::Hash<sad::String, bool> m_sad_object_flags;
    /*! Lock for sad m_sad_object_flags container
     */
    sad::Mutex m_sad_object_flags_lock;
    /*! Lock for converters
     */
    sad::Mutex m_converters_lock;
};

}

}
