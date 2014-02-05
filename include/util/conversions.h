/*! \file conversions.h
	\author HiddenSeeker

	Provides conversions from parsed JSON values to basic types 
 */
#include "../3rdparty/picojson/picojson.h"
#include "../sadcolor.h"

namespace util
{

/*! Tries to parse unsigned char value
	\param[in] v value of JSON
	\param[out] c a char
	\return whether it was successfull
 */
bool parse_uchar(const picojson::value & v, unsigned char & c);

/*! Tries to parse unsigned char value
	\param[in] v value of JSON
	\param[out] c a color
	\return whether it was successfull
 */
bool parse_color(const picojson::value & v, sad::Color & c);

}
