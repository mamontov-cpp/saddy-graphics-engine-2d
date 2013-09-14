/*! \file   extchecker.h
    \author HiddenSeeker
	Provides a simple check for OpenGL extensions.
*/
#pragma once
#include "templates/hpair.hpp"
namespace ext
{
	/*! Checks for an extension, marked by name. Returns false if not found, or can't
		obtain extension list
	*/
	bool presented(const char * name);
	/*! Parses OpenGL version, returning current used version
		\return version pair <major, minor>
	 */
	hst::pair<int,int> version();
	/*! Returns an extension list
		\return NULL if can't be obtained
	*/
	const char * list();
}
