/*! \file animations/setstate/typedcommand.h
    

    Defines a command to be invoked with specified argument
 */
#pragma  once
#include "abstractsetstatecommand.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! A command, which can be invoked with specified argument
 */
template<
    typename T
>
class TypedCommand: public sad::animations::setstate::AbstractSetStateCommand
{
public:
    /*! Invokes a command with specified argument
        \param[in] a argument
     */
    virtual void call(const T& a) = 0;
    /*! Must be inherited
     */
    virtual ~TypedCommand() override { }
};

};

}

}
