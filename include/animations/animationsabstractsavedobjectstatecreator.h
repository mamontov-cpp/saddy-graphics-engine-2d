/*! \file animations/animationsabstractsavedobjectstatecreator.h
    \author HiddenSeeker

    Defines a factory creator, which must could create saved object state
 */
#pragma once
#include "animationssavedobjectstate.h"

namespace sad
{

namespace animations
{

/*! This class is used to create states for specified objects
 */
class AbstractSavedObjectStateCreator
{
public:
    /*! Construct a creator by specified key
        \param name a key for storing state in cache
     */
    AbstractSavedObjectStateCreator(const sad::String& name);
    /*! Must be inherited
     */
    virtual ~AbstractSavedObjectStateCreator();
    /*! Clones a saved object state creator
        \return new copy of creator
     */
    virtual AbstractSavedObjectStateCreator* clone() const = 0;
    /*! Creates a state for specified object
        \param o a specified object for creator
        \return a saved object state
     */
    virtual sad::animations::SavedObjectState* create(sad::db::Object* o) = 0;
    /*! Returns a unique key, which defines how should state be saved in cache
		\return unique key
     */
    inline const sad::String& name() const
    {
        return m_name;
    }
    /*! Sets name for a creator
        \param[in] name a name
     */
    inline void setName(const sad::String& name)
    {
        m_name = name;
    }
protected:
    /*! A unique key, which defines how should state be saved
     */
    sad::String m_name;
};

}

}
