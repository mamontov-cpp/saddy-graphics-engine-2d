/*! \file keydecoder.h
	\author HiddenSeeker

	Defines a decoder for converting key symbols from OS to Saddy key symbols.
 */
#pragma once
#include "os/windowhandles.h"
#include "../sadhash.h"

namespace sad
{

class Window;

namespace os
{
/*! A saddy key typedef
 */
typedef int  Key;

#ifdef WIN32
/*! A key, received from OS
 */
typedef WPARAM  SystemKey;

#endif

#ifdef X11
/*! A key, received from OS
 */
typedef ::KeySym SystemKey;

#endif

/*! Converts key symbols from OS to Saddy key symbols
 */ 
class KeyDecoder
{
public:
	/*! Creates new key decoder
	 */
	KeyDecoder();
	/*! Decodes a system key, converting it from common key to
		current
		\param[in] key a system key
		\return decoded key from saddy
	 */
	sad::os::Key decode(sad::os::SystemKey key);
	/*! Converts system key to character
		\param[in] key key, received by system
		\param[in] win a window, where other parameters should come from
		\return converted key
	 */
	char convert(sad::os::SystemKey key, sad::Window * win);
protected:
	/*! Fills table with mapppings
	 */
	void init();
	/*! A recoding table for decoder
	 */
	sad::Hash<sad::os::SystemKey, sad::os::Key> m_table;
};

}

}
