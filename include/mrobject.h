/*! \file mrobject.h
    
    Defines an object with multiple parent objects
 */
#pragma once
#include "sadhash.h"
#include <algorithm>

namespace sad
{

/*! An object with multiple parent objects
 */ 
class MRObject
{
public:
    /*! Constructs object with zero parent references
     */
    inline MRObject()
    {
    }
    /*! Adds parent object to object
        \param[in] m a new parent object
     */
    inline void addParent(sad::MRObject* m)
    {
        if (m_parents.contains(m))
        {
            m_parents[m]++;
        }
        else
        {
            m_parents.insert(m, 1);
        }
    }
    /*! Removes a parent object from object
        \param[in] m an old parent object
     */
    inline void removeParent(sad::MRObject* m)
    {
        if (m_parents.contains(m))
        {
            if (m_parents[m] > 1)
            {
                m_parents[m]--;
            }
            else
            {
                m_parents.remove(m);
            }
        }
    }
    /*! Returns true if current object is parent for some other objects
        \param[in] m object
        \return true if this object is parent
     */ 
    inline bool isParent(sad::MRObject* m) const
    {
        sad::Vector<const sad::MRObject*> local;
        return this->isParent(m, local);
    }
    /*! Returns true if current object is parent for some other objects
        \param[in] m object
        \param[in] visited list of already visited objects
        \return true if this object is parent
     */
    inline bool isParent(sad::MRObject* m, const sad::Vector<const sad::MRObject*>& visited) const
    {
        if (m == this)
        {
            return true;
        }
        if (std::find(visited.begin(), visited.end(), this) != visited.end())
        {
            return false;
        }
        sad::Vector<const sad::MRObject*> local = visited;
        local.push_back(this);
        bool result = false;
        for(sad::Hash<sad::MRObject*, size_t>::const_iterator it = m_parents.const_begin(); it != m_parents.const_end(); ++it)
        {
            result = result || it.key()->isParent(m, local);
        }
        return result;
    }
protected:
    /*! A parent nodes and vectors
     */
    sad::Hash<sad::MRObject*, size_t> m_parents;
};
    
    
}