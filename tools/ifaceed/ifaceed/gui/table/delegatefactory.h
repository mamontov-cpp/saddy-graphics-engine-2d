/*! \file delegatefactory.h
    

    A factory for creating delegates for types.
 */
#pragma once
#include "delegate.h"

#include <sadptrhash.h>

namespace gui
{

namespace table
{
    
/*! A factory for creating delegates automatically
 */
class DelegateFactory
{
public:
/*! An abstract factory delegate for creating table delegates
 */
class AbstractDelegate
{
public: 
    /*! Creates new delegate
        \return delegate
     */
    virtual gui::table::Delegate* create() = 0;
    /*! Could be inherited
     */
    virtual ~AbstractDelegate();
};
/*! A delegate for creating  specified delegates
 */
template<
    typename T
>
class Delegate: public gui::table::DelegateFactory::AbstractDelegate
{
public:
    /*! Creates default delegate
     */
    Delegate()
    {
        
    }
    /*! Creates new delegate
        \return delegate
     */
    virtual gui::table::Delegate* create() override
    {
        return new T();
    }
    /*! Destroys delegate
     */
    virtual ~Delegate() override
    {
        
    }
};
    /*! A factory for creating delegates 
     */
    DelegateFactory();
    /*! Could be inherited
     */
    virtual ~DelegateFactory();
    /*! Creates new delegate for factory
        \param[in] s a type name
     */
    virtual gui::table::Delegate* create(const QString & s);
protected:
    /*! Insert new delegate for type T with specified name
        \param[in] name specified name
     */
    template<
        typename T
    >
    void insert(const sad::String& name)
    {
        m_delegates.insert(name, new gui::table::DelegateFactory::Delegate<T>());
    }
    /*! We re-use delegates to create own delegates, still, since there are no other horrible dependencies
     */
    sad::PtrHash<sad::String, gui::table::DelegateFactory::AbstractDelegate> m_delegates;
};

}

}