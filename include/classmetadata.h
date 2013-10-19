/*! \file   classmetadata.h
    \author HiddenSeeker

	\brief  Definition of class meta data, which consists of name of class and it's parent names.

	This file contains a definition for class metadata.
*/
#pragma once
#include "sadstring.h"
#include "sadvector.h"

namespace sad
{
/*! \class ClassMetaData
	This is basic class for meta data. It contains all of basic methods for getting required 
	meta data for class. Currently only name and ancestors for class are supported.		
 */
class ClassMetaData
{
 private:
	/*! A special private index for class
	 */
    unsigned int   m_private_index;
	/*! A name for class data
	 */
	sad::String m_name;
	/*! An ancestor list for current class  
	 */
	sad::Vector<ClassMetaData *> m_ancestors;
 public:
	/*! A name for class is defined by macro @see SAD_DEFINE_BASIC_OBJECT, SAD_DEFINE_OBJECT
		\param[in] name name of class
	 */
	void setName(const sad::String & name);
	/*! Returns a name for class
		\return name of class
	 */
	const sad::String& name() const;
	/*! Adds a new ancestor with name
		\param[in] name name of ancestor class
	 */
	void addAncestor(const sad::String & name);  
	/*! Returns a true if class has ancestor with specified name
		\param[in] name name of ancestor
		\return whther it can be cated to type
	 */
	bool canBeCastedTo(const sad::String & name);
	/*! Returns true if class can be casted from this class to another class
		\param[in] name name of descendant
		\return whether it can be casted to this type
	 */
	bool canBeCastedFrom(const sad::String & name);
	/*! Returns special index, which can be used in open-adressed tables
		\return index
	 */
	unsigned int privateIndex() const;
	/*! Sets new private index
	 */
	void setPrivateIndex(unsigned int privateIndex);
};

}
