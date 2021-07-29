/*! \file   classmetadata.h
    

    \brief  Definition of class meta data, which consists of name of class and it's parent names.

    This file contains a definition for class metadata.
*/
#pragma once
#include "sadstring.h"
#include "sadvector.h"
#include "sadptrhash.h"
#include "classmetadatacastfunction.h"

namespace sad
{
/*! \class ClassMetaData
    This is basic class for meta data. It contains all of basic methods for getting required 
    meta data for class. Currently only name and ancestors for class are supported.		
 */
class ClassMetaData
{
public:
    /*! A list of cast function to metadata
     */
    typedef sad::PtrHash<sad::String, sad::AbstractClassMetaDataCastFunction> CastFunctions;
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
    /*! Adds a new ancestor
        \param[in] ancestor an ancestor
    */
    void addAncestor(sad::ClassMetaData* ancestor);
    /*! Returns casts function lists
        \return casts list
     */
    inline const CastFunctions & casts() const
    {
        return m_casts;	
    }
    /*! Adds casting data to casting all items
        \param[in] name a name of casted type
        \param[in] f a cast function to be casted
     */ 
    inline void addCast(const sad::String & name, sad::AbstractClassMetaDataCastFunction * f)
    {
        if (m_casts.contains(name))
            return;
        m_casts.insert(name, f);
    }
    /*! Returns a true if class has ancestor with specified name
        \param[in] name name of ancestor
        \return whether it can be casted to type
     */
    bool canBeCastedTo(const sad::String & name);
    /*! Tries to cast one type to other if can
        \param[in] o an object
        \param[in] name name of type of other object
        \return result (nullptr if can't)
     */
    sad::Object * castTo(sad::Object * o, const sad::String & name);
    /*! Returns true if class can be casted from this class to another class
        \param[in] name name of descendant
        \return whether it can be casted to this type
     */
    bool canBeCastedFrom(const sad::String & name);
    /*! Returns special index, which can be used in open-address tables
        \return index
     */
    unsigned int privateIndex() const;
    /*! Sets new private index
     */
    void setPrivateIndex(unsigned int privateIndex);
private:
    /*! A special private index for class
     */
    unsigned int   m_private_index = 0;
    /*! A name for class data
     */
    sad::String m_name;
    /*! An ancestor list for current class  
     */
    sad::Vector<ClassMetaData *> m_ancestors;
    /*! A list casted functions to class meta data
     */
    CastFunctions m_casts;
};

}
