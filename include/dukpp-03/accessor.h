/*! \file accessor.h
 
    Defines an accessor for getting a property, defined in schema
 */
#pragma once
#include "basiccontext.h"

namespace sad
{
    
namespace dukpp03
{

/*! A scriptable property accessor, which can access objects property via schema
 */
class Accessor: public ::dukpp03::Callable<sad::dukpp03::BasicContext>
{
public:
    /*! Makes new accessor
        \param[in] name of property
     */
    Accessor(const sad::String& name);
    /*! Performs check, whether callable can be called, without actually calling an object in context
        \param[in] c context
        \return pair
     */
    virtual std::pair<int, bool> canBeCalled(sad::dukpp03::BasicContext* c) override;
    /*! Could be inherited
     */
    virtual ~Accessor() override;
    /*! Returns false
        \return false
     */
    virtual bool canBeCalledAsConstructor() override;
protected:
    /*! Checks remaining arguments for context
        \param[in] o object
        \param[in] c context
        \return amount of matched arguments
     */
    virtual int checkRemainingArguments(sad::db::Object* o, sad::dukpp03::BasicContext* c) const;
    /*! Checks first argument to be called
        \param[in] c context
        \param[in] throw_error if true, throws error if cannot be called
        \return nullptr if method cannot be called
     */
    sad::db::Object* checkThis(sad::dukpp03::BasicContext* c, bool throw_error) const;
    /*! A name of property to access
     */
    sad::String m_name;
};  
    

}

}
