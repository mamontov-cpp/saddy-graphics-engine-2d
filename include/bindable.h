/*! \file bindable.h
   
    Describes bindable abstract object
 */
#pragma once

namespace sad
{
/*! The class for object, that can be bound
 */
class Bindable
{
public:
    /*! Binds object
     */
    virtual void bind() = 0;
    /*! Can be inherited
     */
    virtual ~Bindable();
};

}
