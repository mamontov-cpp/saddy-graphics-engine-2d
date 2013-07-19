/*! \file   classmetadata.h
    \author HiddenSeeker

	\brief  Definition of class meta data, which consists of name of class and it's parent names.

	This file contains a definition for class metadata.
*/
#include "../templates/hstring.h"
#include "../templates/hlvector.hpp"
/*! \class ClassMetaData
	This is basic class for meta data. It contains all of basic methods for getting required 
	meta data for class. Currently only name and ancestors for class are supported.		
 */
class ClassMetaData
{
 private:
	/*! A name for class data
	 */
	hst::string m_name;
	/*! An ancestor list for current class  
	 */
	hst::vector<ClassMetaData *> m_ancestors;
 public:
	/*! A name for class is defined by macro @see SAD_DEFINE_BASIC_OBJECT, SAD_DEFINE_OBJECT
		\param[in] name name of class
	 */
	void setName(const hst::string & name);
	/*! Returns a name for class
	 */
	const hst::string& name() const;
	/*! Adds a new ancestor with name
		\param[in] name name of ancestor class
	 */
	void addAncestor(const hst::string & name);  
	/*! Returns a true if class has ancestor with specified name
		\param[in] name name of ancestor
	 */
	bool canBeCastedTo(const hst::string & name);
	/*! Returns true if class can be casted from this class to another class
	 */
	bool canBeCastedFrom(const hst::string & name);
};