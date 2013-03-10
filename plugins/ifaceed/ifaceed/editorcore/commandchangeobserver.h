/** \file commandchangeobserver
	\author HiddenSeeker
	An observer for working with element changing data
 */
#include <templates/hstring.h>
#include <marshal/variant.h>
#pragma once

class CommandChangeObserver
{
 public:
	/** Submits event to observer
		\param[in] type type of event
	 */
	virtual void submitEvent(const hst::string & eventType, const sad::Variant & v) = 0;
};