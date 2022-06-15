/*! \file   refcountable.h
    
    Declares a simple reference-countable object.
*/
#pragma once
#include <new>
#include <memory>
#include <functional>

namespace sad
{

/*!
 * Defines a reference countable object, which behaves like intrusive pointer
 */
class RefCountable
{
public:
    /*! Creates a new reference-countable object, with
        zero references on it
     */
    RefCountable();
    /*! Adds references to an object, increasing reference count
     */
    virtual void addRef();
    /*! Removes references to an object, decreasing reference count
     */
    virtual void delRef();
    /*! Deletes referenced instance
     *  \param[in] rc ref countable
     */
    static void delRefInstance(RefCountable* rc);
    /*! Returns count of refs to this object
        \return references
     */
    int refsCount() const;
    /*! A ref countable object could be inherited, so it created a behaviour
        like intrusive shared pointer
     */
    virtual ~RefCountable();
private:
    /*! Stores amount of references to an object
     */
     int m_references;
};

/*! Makes new reference, storing data inside of item
 *  \param[in] args arguments
 *  \return reference
 */
template<typename T, typename... Args>
inline T* new_rc(Args... args)
{
    T* item = new T(args...);
    item->addRef();
    return item;
}

/*! Makes new reference, storing data inside of item
 *  \return reference
 */
template<typename T>
inline T* new_rc()
{
    T* item = new T();
    item->addRef();
    return item;
}

template<typename T>
using RCUPtr = std::unique_ptr<T, std::function<void(sad::RefCountable*)>>;

/*! Makes new reference, storing data inside of item
 *  \param[in] args arguments
 *  \return reference
 */
template<typename T, typename... Args>
inline RCUPtr<T> new_rcu(Args... args)
{
    T* item = new T(args...);
    item->addRef();
    return RCUPtr<T>(item, sad::RefCountable::delRefInstance);
}

/*! Makes new reference, storing data inside of item
 *  \return reference
 */
template<typename T>
inline RCUPtr<T> new_rcu()
{
    T* item = new T();
    item->addRef();
    return RCUPtr<T>(item, sad::RefCountable::delRefInstance);
}



}
