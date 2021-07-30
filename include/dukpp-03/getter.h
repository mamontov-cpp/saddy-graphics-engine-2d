/*! \file getter.h
 
    Defines a getter for getting a property, defines in schema
 */
#pragma once
#include "accessor.h"

namespace sad
{
    
namespace dukpp03
{

/*! Defines a getter for schema properties
 */
class Getter: public sad::dukpp03::Accessor
{
public:
    /*! Makes new getter
        \param[in] name of property
     */
    Getter(const sad::String& name);  
    /*! Could be inherited
     */
    virtual ~Getter();
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
    virtual int _call(sad::dukpp03::BasicContext* c)  override;
};  
    

}

}
