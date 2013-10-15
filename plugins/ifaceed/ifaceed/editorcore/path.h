#include <sadstring.h>
#pragma once

namespace path
{
/** Returns directory path for file
	\param[in] path path for file
 */
sad::String directory(const sad::String & path);
/** Concats to paths for file
	\param[in] parent path
	\param[in] path
	\return parent + path in correct format
 */
sad::String concat(const sad::String & parent,const sad::String & path);


}
