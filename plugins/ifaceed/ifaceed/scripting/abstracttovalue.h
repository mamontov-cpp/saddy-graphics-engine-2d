/*! \file abstracttovalue.h
    

    Converts a scriptvalue to specific value
 */
#pragma once
#include <QScriptEngine>
#include <maybe.h>

namespace scripting
{

/*! An abstract class for converting it to value
 */
template<
    typename T
>
class AbstractToValue
{
public:
    /*! Converts a value to value
     */
    inline AbstractToValue()
    {
        
    }
    /*! Can be inherited
     */
    virtual ~AbstractToValue()
    {
        
    }
    /*! Converts a resulting value to a value
        \param[in] v value
        \return result
     */
    virtual sad::Maybe<T> toValue(const QScriptValue& v) = 0;
};


}
