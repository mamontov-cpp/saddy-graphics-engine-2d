/** \file commandchangeobserver.h
	\author HiddenSeeker
	An observer for working with element changing data
 */
#include <templates/hstring.h>
#include <marshal/variant.h>
#pragma once

/*! A special observer for notifications of command changes
 */
class CommandChangeObserver
{
 public:
	/** Submits event to observer
		\param[in] eventType type of event
		\param[in] v    an associated value
	 */
	virtual void submitEvent(const hst::string & eventType, const sad::Variant & v) = 0;
};