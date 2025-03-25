/*! \file sadraiiobjectvector.h
 *
 *  A RAII wrapper for object vector for storing sad objects
 */
#pragma once
#include <sadvector.h>
#include <algorithm>

namespace sad
{

template<
    typename T
>
class RAIIObjectVector: public sad::Vector<T*>
{
public:
    /*! By default, it's the same implementation
     */
    RAIIObjectVector() {}

    /*!
     * Initializes vector with related data
     * \param[in] o list of objects
     */
    RAIIObjectVector(std::initializer_list<T*> o) : sad::Vector<T*>(o)
    {
        addRef();
    }

    /*!
     * Stores all objects, adding references
     * \param[in] o object
     */
    RAIIObjectVector(const sad::Vector<T*>& o) : sad::Vector<T*>(o)
    {
        addRef();
    }
    /*!
     * Stores all objects, adding references
     * \param[in] o object
     */
    RAIIObjectVector(sad::Vector<T*>&& o) noexcept
    {
        swap(o);
        addRef();
    }

    /*!
     * Stores all objects, adding references
     * \param[in] o object
     */
    RAIIObjectVector(const sad::RAIIObjectVector<T>& o) : sad::Vector<T*>(o)
    {
        addRef();
    }

    /*!
     * Stores all objects, adding references
     * \param[in] o object
     */
    RAIIObjectVector(sad::RAIIObjectVector<T>&& o) noexcept
    {
        swap(o);
    }

    /*! Copies new vector, clearing current state
     *  \param[in] o other vector
     *  \return self-reference
     */
    RAIIObjectVector<T>& operator=(const sad::Vector<T*> & o)
    {
        if (this != &o)
        {
            if (o.empty())
            {
                clear();
            }
            else
            {
                RAIIObjectVector<T> copy(o);
                swap(copy);
            }
        }
        return *this;
    }

    /*! Copies new vector, clearing current state
     *  \param[in] o other vector
     *  \return self-reference
     */
    RAIIObjectVector<T>& operator=(sad::Vector<T*> && o)
    {
        if (this != &o)
        {
            swap(o);
            addRef();
            delRef(o);
        }
        return *this;
    }
    
    
    /*! Copies new vector, clearing current state
     *  \param[in] o other vector
     *  \return self-reference
     */
    RAIIObjectVector<T>& operator=(const sad::RAIIObjectVector<T> & o)
    {
        if (this != &o)
        {
            if (o.empty())
            {
                clear();
            }
            else
            {
                RAIIObjectVector<T> copy(o);
                swap(copy);
            }
        }
        return *this;
    }
    
    /*! Just swaps all data
     *  \param[in] o other vector
     *  \return self-reference
     */
    RAIIObjectVector<T>& operator=(sad::RAIIObjectVector<T> && o) noexcept
    {
        if (this != &o)
        {
            swap(o);
        }
        return *this;
    }
    
    void stripNull()
    {
        auto it = std::remove_if(this->begin(), this->end(), [](T* o) -> bool { return o == nullptr; });
        erase(it, this->end());
    }

    /*! Clears vector
     */
    // ReSharper disable once CppHidingFunction
    void clear()
    {
        delRef();
        this->sad::Vector<T*>::clear();
    }


    /*! Deletes all references
     */
    ~RAIIObjectVector() override
    {
        delRef();
    }

private:
    /*! Adds reference for all non-null  objects
     */
    void addRef()
    {
        for (T* item : *this)
        {
            if (item)
            {
                item->addRef();
            }
        }
    }

    /*! Deletes reference for all non-null objects
     */
    void delRef()
    {
        delRef(*this);
    }

    /*!
     * Deletes refs on vector
     * \param[in] items  vector of items
     */
    static void delRef(const sad::Vector<T*>& items)
    {
        for (T* item : items)
        {
            if (item)
            {
                item->delRef();
            }
        }
    }
};

}
