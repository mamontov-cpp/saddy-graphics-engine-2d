/** \file commandchangeobserver.h
	\author HiddenSeeker
	An observer for working with element changing data
 */
#include <sadstring.h>
#include <db/dbvariant.h>
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
	virtual void submitEvent(const sad::String & eventType, const sad::db::Variant & v) = 0;
};