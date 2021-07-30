/*! \file setter.h
    
    Defines a setter for setting a property, defines in schema
 */
#pragma once
#include "accessor.h"

namespace sad
{
    
namespace dukpp03
{

/*! Defines a getter for schema properties
 */
class Setter: public sad::dukpp03::Accessor
{
public:
    /*! Makes new getter
        \param[in] name of property
     */
    Setter(const sad::String& name);  
    /*! Could be inherited
     */
    virtual ~Setter();
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual Callable<sad::dukpp03::BasicContext>* clone() override;
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments() override;
    /*! Performs actual function call
        \param[in] c context
     */
    virtual int _call(sad::dukpp03::BasicContext* c) override;  
protected:
    /*! Checks remaining arguments for context
        \param[in] o object
        \param[in] c context
        \return amount of matched arguments
     */
    int checkRemainingArguments(sad::db::Object* o, sad::dukpp03::BasicContext* c) const override;
};  
    

}

}
