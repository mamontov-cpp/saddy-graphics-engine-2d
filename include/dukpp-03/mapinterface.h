/*! \file mapinterface.h
  
    Defines a MapInterface, based on sad::Hash
 */
#pragma once
#include "../sadhash.h"

namespace sad
{

namespace dukpp03
{

template<
    typename _Key,
    typename _Value
>
class MapInterface
{
public:
    /*! A basic iterator
     */
    struct iterator
    {
        /*! An inner iterator
         */
        typename sad::Hash<_Key, _Value>::const_iterator it;
        /*! A parent map
         */ 
        sad::Hash<_Key, _Value> const* parent;
        /*! Returns key
            \return key of map
         */
        const _Key& key()
        {
            return it.key();
        }
        /*! Returns value
            \returns value for element of map
         */
        const _Value& value()
        {
            return it.value();
        }
        /*! Proceeds to next value
         */
        void next()
        {
            ++it;
        }
        /*! Returns true on end
            \return end
         */
        bool end()
        {
            return it == parent->const_end();
        }
    };
    
    MapInterface()
    {
        
    }
    
    
    /*! Returns beginning iterator for container
     */ 
    iterator begin() const
    {
        iterator it;
        it.it = m_map.const_begin();
        it.parent = &m_map;
        return it;
    }
    /*! Inserts new element into map, erasing old value, if needed
        \param[in] k key value
        \param[in] v value
     */
    void insert(const _Key& k, const _Value& v)
    {
        m_map.insert(k, v);
    }
    /*! Removes value by key
        \param[in] k key value
     */
    void remove(const _Key& k)
    {
        m_map.remove(k);
    }
    /*! Returns true, if map contains key value
        \param[in] k key value
        \return whether map contains value
     */
    bool contains(const _Key& k)
    {
        return m_map.contains(k);
    }
    /*! Returns value by specified key
        \param[in] k key
        \return value
     */
    const _Value& get(const _Key& k)
    {
        return m_map[k];
    }
    /*! Clears value of interface
     */
    void clear()
    {
        m_map.clear();
    }
    /*! Inner map
     */
    sad::Hash<_Key, _Value> m_map;
};

}

}
