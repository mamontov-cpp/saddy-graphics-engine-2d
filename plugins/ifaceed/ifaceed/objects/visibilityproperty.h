/** \file visibilityproperty.h
	\author HiddenSeeker

	Describes a non-saveable visibility property
 */
#include "marshal/property.h"
#pragma once

/** A visibility property data
 */
class VisibilityProperty: public MappedField<bool>
{
 public:
	 /** Constructs new field with initial value
		 \param[in] ptr   field pointer
		 \param[in] init  initial value
	   */
	 inline VisibilityProperty( bool * ptr, bool init): MappedField<bool>(ptr,init) {}
	 virtual bool saveable() const;
};