/*! \file history/database/changeproperty.h
    

    Contains definitions of changed property of database.
 */
#pragma once
#include "../history.h"
#include "../gui/table/delegate.h"

namespace history
{
    
namespace database
{

/*! A command, if property value changed in database
 */
template<
    typename T
>
class ChangeProperty: public history::Command
{
public:
    /*! Constructs new object, if property value is changed in database
        \param[in] old_value old value of property
        \param[in] new_value new value of property
        \param[in] d a delegate for objects
     */
    ChangeProperty(
        const T& old_value,
        const T& new_value,
        gui::table::Delegate* d
    ) 
    : 
    m_old_value(old_value), 
    m_new_value(new_value),
    m_delegate(d)
    {
            
    }
    /*! This command can be inherited
     */
    virtual ~ChangeProperty() override
    {
        
    }
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override
    {
        m_delegate->set(sad::db::Variant(m_new_value));
    }
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override
    {
        m_delegate->set(sad::db::Variant(m_old_value));
    }
protected:
    /*! Old value for property 
     */
    T m_old_value;
    /*! New value for property
     */
    T m_new_value;
    /*! A delegate a property 
     */
    gui::table::Delegate* m_delegate;
};

}

}
