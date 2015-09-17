#pragma once
#include <sadvector.h>
#include <resource/error.h>

/*! Returns amount of errors of specified  type
    \return errors count
 */
inline int count_errors_of_type(
    const  sad::Vector<sad::resource::Error *> & errors, 
    const sad::String & type
)
{
    int result = 0;
    for(size_t i = 0; i < errors.size(); i++)
    {
        if (errors[i]->metaData()->canBeCastedTo(type))
        {
            result += 1;
        }
    }
    return result;
}