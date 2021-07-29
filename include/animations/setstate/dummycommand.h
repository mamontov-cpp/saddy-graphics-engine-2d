/*! \file animations/setstate/dummycommand.h
    

    Defines a dummy command, which does nothing, when invoked
 */
#pragma once
#include "typedcommand.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! A command, which does nothing, when invoked
 */
template<
    typename T
>
class DummyCommand: public sad::animations::setstate::TypedCommand<T>
{
public:
    /*! Invokes a command with specified argument
        \param[in] a argument
     */
    virtual void call(const T& a) override
    {
        
    }

    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const override
    {
        return new sad::animations::setstate::DummyCommand<T>();    
    }

    /*! Can be inherited
     */
    virtual ~DummyCommand() override
    {
        
    }
};

};

}

}
