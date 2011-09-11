/*! \file   extcheker.h
    \author HiddenSeeker
	Provides a simple check for OpenGL extensions.
*/
#pragma once

namespace ext
{
	/*! Checks for an extension, marked by name. Returns false if not found, or can't
		obtain extension list
	*/
	bool presented(const char * name);
	/*! Returns an extension list
		\return NULL if can't be obtained
	*/
	const char * list();
}
