/*! \file duktapecallable.h
    

    Contains definition of function wrapper, callable from Duktape, which will be used as substitution object, used
    to produce template bindings.
 */
#pragma once

namespace sad
{
    
namespace duktape
{
    
class Context;

/*! A definition of function wrapper, callable from Duktape, which will be used as substitution object, used
    to produce template bindings
 */
class DuktapeCallable
{
public:
    /*! Must return copy of callable object of duktape
        \return copy of callable object
     */
    virtual DuktapeCallable* clone() = 0;
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments() = 0;
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c);
    /*! Must be inherited
     */
    virtual ~DuktapeCallable();
};

}

}