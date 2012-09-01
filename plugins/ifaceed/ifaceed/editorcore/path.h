#include <templates/hstring.h>

namespace path
{
/** Returns directory path for file
	\param[in] path path for file
 */
hst::string directory(const hst::string & path);
/** Concats to paths for file
	\param[in] parent path
	\param[in] path
	\return parent + path in correct format
 */
hst::string concat(const hst::string & parent,const hst::string & path);


}
