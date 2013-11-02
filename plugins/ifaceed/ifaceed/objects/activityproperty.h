/** \file activityproperty.h
	\author HiddenSeeker

	Describes a non-saveable activity property
 */
#include "marshal/property.h"
#pragma once

/** A visibility property data
 */
class ActivityProperty: public MappedField<bool>
{
 public:
	 /** Constructs new field with initial value
		 \param[in] ptr   field pointer
		 \param[in] init  initial value
	   */
	 inline ActivityProperty( bool * ptr, bool init): MappedField<bool>(ptr,init) {}
	 virtual bool saveable() const;
};