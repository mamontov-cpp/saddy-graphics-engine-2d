// ReSharper disable once CppDoxygenUnresolvedReference
/*! \file ../include/object.h
    

    Defines a universal object class, used as container for metadata
 */
#pragma once
#include "classmetadatacontainer.h"
#include "db/dbtypename.h"
#include "db/dbobject.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "3rdparty/picojson/picojson.h"

namespace sad
{

class Object:public sad::db::Object
{
 public:
     /*! A metadata for class, must be implemented in any descendant
         \return metadata
      */
     static sad::ClassMetaData * globalMetaData();
     /*!  Returns a metadata for class
          \return metadata
      */ 
     virtual sad::ClassMetaData * metaData() const;
     /*! Returns a name for class
         \return name of class
      */
     virtual const sad::String & className() const;
     /*! Returns name of class for metadata
         \return name of class for metadata
     */
     virtual const sad::String& serializableName() const override;
     /*! An object data
      */
     virtual ~Object() override;
     /*! A basic introspection capability. Checks, whether object has specified type
        \param[in] name name of class
        \return in basic implementation - false
     */
     virtual bool isInstanceOf(const sad::String& name) override;
     /*! Performs checked casting to object, throws exception on error
         \return type if it can be casted, otherwise throws an exception
      */
     template<typename _Dest> _Dest * as();
     /*! Performs chained checked casting to object, 
         throws exception on error. First time casts it to T1, second time 
         to T2
         \return type if it can be casted, otherwise throws an exception
      */
     template<typename T1, typename T2> T2 * as();
};

/*! An exception, which is thrown, when one object can not be casted to another
 */
class InvalidCastException
{
 private:
     /*! A name of real type of object
      */
     sad::String m_from_name;
     /*! A name of type, which we were casting to
      */
     sad::String m_to_name;
     /*! A message for type
      */
     sad::String m_message;
 public:
     /*! Creates new exception with specified parameters
         \param[in] from_name real type of object
         \param[in] to_name type, which we were casting to
      */
     InvalidCastException(const sad::String & from_name, const sad::String & to_name);
     /*! A real type of object
         \return real type of object
      */
     const sad::String & fromName() const;
     /*! A type, which we were casting to
         \return type, which we were casting to
      */ 
     const sad::String & toName() const;
     /*! A human-readable message for object
         \return a message for exception
      */
     const sad::String & message() const;
};


}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Object)

namespace sad 
{
/*! Performs a checked cast from one type to another.
    Used to widen domain, or when sad::Object::as does not work
    (multiple inheritance with some uncommon path).

    It is highly unlikely to use it with multiple inheritance, because
    when casting from base sad::Object to second ancestor, or third, due
    to behaviour of upcast we will get invalid pointer, because compiler
    will fail to compute offset of second class in structure.
    
    \param[in] arg argument
    \return result of cast
 */
template<typename _Dest, typename _Src> _Dest * checked_cast(_Src * arg)
{                                                                
    _Dest * result;                                       
    const sad::String & dest_name = _Dest::globalMetaData()->name();      
    if (arg->metaData()->canBeCastedTo(dest_name) == false)      
    {                                                            
        throw sad::InvalidCastException(arg->metaData()->name(), dest_name); 
    }          

    if (arg->metaData()->casts().contains(dest_name))
    {
        sad::Object * o = arg->metaData()->casts()[dest_name]->cast(static_cast<void*>(arg));
        result = reinterpret_cast<_Dest*>(o); 
    }
    else
    {
        result = static_cast<_Dest*>(arg); 
    }                                                            
    return result;                                               
}  


}

/*! Declares methods as for specified class
 */
#define DECLARE_CASTS_AS_METHODS(CLASSNAME)                      \
template<typename _Dest> _Dest * CLASSNAME ::as()                \
{                                                                \
    return sad::checked_cast<_Dest, CLASSNAME > (this);          \
}                                                                \
                                                                 \
template<typename T1, typename T2> T2 * CLASSNAME ::as()         \
{                                                                \
    T1 * tmp = sad::checked_cast<T1, CLASSNAME >(this);          \
    T2 * result = sad::checked_cast<T2, T1>(tmp);                \
    return result;                                               \
}                                                                \

DECLARE_CASTS_AS_METHODS(sad::Object)

                                                              

/*! Use this macro to paste type in other macro
 */
#define SAD_TYPE_PASTE(TYPE) TYPE


/*! Use this macro to define a descendant of sad::Object in your include file
 */
#define SAD_OBJECT												\
protected:                                                      \
        static sad::ClassMetaData * m_global_metadata;          \
public:															\
        virtual sad::ClassMetaData * metaData() const;			\
        static sad::ClassMetaData * globalMetaData();			\

/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object,
    where NAMEDCLASS should be name of current class and PARENT  - name of his parent class
 */
#define DECLARE_SOBJ_INHERITANCE(NAMEDCLASS, PARENT)			                                                   \
DECLARE_TYPE_AS_SAD_OBJECT( NAMEDCLASS )                                                                           \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	                                                       \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 	                                                   \
{																                                                   \
    if (m_global_metadata != nullptr) return m_global_metadata;                                                       \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParent< SAD_TYPE_PASTE(PARENT) >(#NAMEDCLASS);  \
    return m_global_metadata;																	                   \
}																								                   \
sad::ClassMetaData * NAMEDCLASS ::metaData() const												                   \
{                                                                                                                  \
    return NAMEDCLASS ::globalMetaData();                                                                          \
}                                                


/*! Use this macro to define in source files,
    that this object is inherited from descendant of sad::Object,
    where NAMEDCLASS should be name of current class and PARENT  - name of his parent class
 */
#define DECLARE_SOBJ_INHERITANCE_TEMPLATE(NAMEDCLASS, PARENT)			                                           \
DECLARE_TYPE_AS_SAD_OBJECT( NAMEDCLASS )                                                                           \
template<> sad::ClassMetaData * NAMEDCLASS ::m_global_metadata = nullptr;	                                           \
template<> sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 	                                       \
{																                                                   \
    if (m_global_metadata != nullptr) return m_global_metadata;                                                       \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParent< SAD_TYPE_PASTE(PARENT) >(#NAMEDCLASS);  \
    return m_global_metadata;																	                   \
}																								                   \
template<> sad::ClassMetaData * NAMEDCLASS ::metaData() const												       \
{                                                                                                                  \
    return NAMEDCLASS ::globalMetaData();                                                                          \
}

/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object,
    where NAMEDCLASS should be name of current class and PARENT  - name of his parent class.
    CASTOBJECT is callback for class metadata to add method for casting to CASTCLASS
 */
#define DECLARE_SOBJ_INHERITANCE_WITH_CAST(NAMEDCLASS, PARENT, CASTCLASS, CASTMETHOD)			 \
DECLARE_TYPE_AS_SAD_OBJECT( NAMEDCLASS )                                                         \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	                                     \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 	                                 \
{																                                 \
    if (m_global_metadata != nullptr) return m_global_metadata;                                     \
    sad::AbstractClassMetaDataCastFunction * f =                                                 \
    sad::MetaDataCastFunctionFamily< NAMEDCLASS >::cast(CASTMETHOD);                             \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParentAndCast<                \
        SAD_TYPE_PASTE(PARENT)                                                                   \
    >(#NAMEDCLASS, #CASTCLASS, f);                                                               \
    return m_global_metadata;																	 \
}																							 	 \
sad::ClassMetaData * NAMEDCLASS ::metaData() const												 \
{                                                                                                \
    return NAMEDCLASS ::globalMetaData();                                                        \
}                                                

/*! Use this macro to define, that this class is direct descendant of sad::Object in your source 
    file. NAMEDCLASS is name of your class
 */ 
#define DECLARE_SOBJ(NAMEDCLASS) DECLARE_SOBJ_INHERITANCE(NAMEDCLASS, sad::Object)

/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object other classes,
    where NAMEDCLASS should be name of current class and PARENT1, PARENT2 
    - name of his parent classes
 */
#define DECLARE_SOBJ_INHERITANCE2(NAMEDCLASS, PARENT1, PARENT2)			 \
DECLARE_TYPE_AS_SAD_OBJECT_EXPLICIT( NAMEDCLASS )                        \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	             \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 			 \
{																		 \
    if (m_global_metadata != nullptr) return m_global_metadata;             \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParent< SAD_TYPE_PASTE(PARENT1),  SAD_TYPE_PASTE(PARENT2) >(#NAMEDCLASS);  \
    return m_global_metadata;																	                                              \
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
    return NAMEDCLASS ::globalMetaData();                                                       \
}    

/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object other classes,
    where NAMEDCLASS should be name of current class and PARENT1, PARENT2, PARENT3 
    - name of his parent classes
 */
#define DECLARE_SOBJ_INHERITANCE3(NAMEDCLASS, PARENT1, PARENT2, PARENT3) \
DECLARE_TYPE_AS_SAD_OBJECT_EXPLICIT( NAMEDCLASS )                        \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	             \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 			 \
{																		 \
    if (m_global_metadata != nullptr) return m_global_metadata;             \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParent< SAD_TYPE_PASTE(PARENT1),  SAD_TYPE_PASTE(PARENT2), SAD_TYPE_PASTE(PARENT3)  >(#NAMEDCLASS);  \
    return m_global_metadata;																	                                                                        \
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
    return NAMEDCLASS ::globalMetaData();                                                       \
}   

/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object other classes,
    where NAMEDCLASS should be name of current class and PARENT1, PARENT2, PARENT3, PARENT4 
    - name of his parent classes
 */
#define DECLARE_SOBJ_INHERITANCE4(NAMEDCLASS, PARENT1, PARENT2, PARENT3, PARENT4) \
DECLARE_TYPE_AS_SAD_OBJECT_EXPLICIT( NAMEDCLASS )                                 \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	                      \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 					  \
{																				  \
    if (m_global_metadata != nullptr) return m_global_metadata;                      \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParent< SAD_TYPE_PASTE(PARENT1),  SAD_TYPE_PASTE(PARENT2), SAD_TYPE_PASTE(PARENT3), SAD_TYPE_PASTE(PARENT4)  >(#NAMEDCLASS);  \
    return m_global_metadata;																	                                                                                                 \
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
    return NAMEDCLASS ::globalMetaData();                                                       \
}   



/*! Use this macro to define in source files, 
    that this object is inherited from descendant of sad::Object,
    where NAMEDCLASS should be name of current class and PARENT  - name of his parent class
    INDEX should be used as private Index in tables
 */
#define DECLARE_SOBJ_INHERITANCE_WITH_INDEX(NAMEDCLASS, PARENT, INDEX)	 \
sad::ClassMetaData * NAMEDCLASS ::m_global_metadata=nullptr;	     \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 	 \
{																 \
    if (m_global_metadata != nullptr) return m_global_metadata;     \
    m_global_metadata = sad::ClassMetaDataContainer::ref()->getWithParentAndIndex< SAD_TYPE_PASTE(PARENT) >(#NAMEDCLASS, INDEX);  \
    return m_global_metadata;																	                                  \
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
    return NAMEDCLASS ::globalMetaData();                                                       \
}                                                
