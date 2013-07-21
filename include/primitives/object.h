/*! \file object.h
	\author HiddenSeeker

	Defines a universal object class, used as container for metadataa
 */
#include "classmetadatacontainer.h"
#pragma once

namespace sad
{

class Object
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
	 const hst::string & name() const;
	 /*! An object data
	  */
	 virtual ~Object();
	 /*! Performs checked casting to object, throws exception on error
		 \return type if it can be casted, otherwise throws an exception
	  */
	 template<typename _Dest> _Dest * as();
};

/*! An exception, which is thrown, when one object can not be casted to another
 */
class InvalidCastException
{
 private:
	 /*! A name of real type of object
	  */
	 hst::string m_fromname;
	 /*! A name of type, which we were casting to
	  */
	 hst::string m_toname;
	 /*! A message for type
	  */
	 hst::string m_message;
 public:
	 /*! Creates new exception with specified parameters
		 \param[in] fromname real type of object
		 \param[in] toname type, which we were casting to
	  */
	 InvalidCastException(const hst::string & fromname, const hst::string & toname);
	 /*! A real type of object
		 \return real type of object
	  */
	 const hst::string & fromName();
	 /*! A type, which we were casting to
		 \return type, which we were casting to
	  */ 
	 const hst::string & toName();
	 /*! A human-readable message for object
		 \return a message for exception
	  */
	 const hst::string & message();
};


}


template<typename _Dest> _Dest * sad::Object::as()
{
	_Dest * result = NULL;
	hst::string destname = _Dest::globalMetaData()->name();
	if (this->metaData()->canBeCastedTo(destname) == false)
	{
		throw sad::InvalidCastException(this->name(), destname);
	} 
	else
	{
		result = static_cast<_Dest*>(this);
	}
	return result;
}
/*! Use this macro to define a descendant of sad::Object in your include file
 */
#define SAD_OBJECT												\
																\
public:															\
		virtual sad::ClassMetaData * metaData() const;			\
		static sad::ClassMetaData * globalMetaData();			\

/*! Use this macro to define in source files, 
	that this object is inherited from descendant of sad::Object,
	where NAMEDCLASS should be name of current class and PARENT  - name of his parent class
 */
#define DECLARE_SOBJ_INHERITANCE(NAMEDCLASS, PARENT)			 \
																 \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 	 \
{																 \
	bool created = false;																		\
	sad::ClassMetaData * result = sad::ClassMetaDataContainer::ref()->get(#NAMEDCLASS, created);\
	if (created)																				\
	{																							\
		result->addAncestor(#PARENT);															\
	}																							\
	return result;																				\
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
	return NAMEDCLASS ::globalMetaData();                                                       \
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
															        	 \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 			 \
{																		 \
	bool created = false;																		\
	sad::ClassMetaData * result = sad::ClassMetaDataContainer::ref()->get(#NAMEDCLASS, created);\
	if (created)																				\
	{																							\
		result->addAncestor(#PARENT1);															\
		result->addAncestor(#PARENT2);															\
	}																							\
	return result;																				\
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
															        	 \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 			 \
{																		 \
	bool created = false;																		\
	sad::ClassMetaData * result = sad::ClassMetaDataContainer::ref()->get(#NAMEDCLASS, created);\
	if (created)																				\
	{																							\
		result->addAncestor(#PARENT1);															\
		result->addAncestor(#PARENT2);															\
		result->addAncestor(#PARENT3);															\
	}																							\
	return result;																				\
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
															        			  \
sad::ClassMetaData * NAMEDCLASS ::globalMetaData()	  		 					  \
{																				  \
	bool created = false;																		\
	sad::ClassMetaData * result = sad::ClassMetaDataContainer::ref()->get(#NAMEDCLASS, created);\
	if (created)																				\
	{																							\
		result->addAncestor(#PARENT1);															\
		result->addAncestor(#PARENT2);															\
		result->addAncestor(#PARENT3);															\
		result->addAncestor(#PARENT4);															\
	}																							\
	return result;																				\
}																								\
sad::ClassMetaData * NAMEDCLASS ::metaData() const												\
{                                                                                               \
	return NAMEDCLASS ::globalMetaData();                                                       \
}
