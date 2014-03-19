/** \file   scenelayerproperty.h
	\author HiddenSeeker

	Describes a data, that consists of layer in scene of property
 */
#include "marshal/abstractproperty.h"
#pragma once

class SceneLayerProperty:public AbstractProperty
{
 private:
		 bool m_loaded_data;     //!< Whether scene layer deferred
		 int  m_layer_deferred;  //!< Deferred layer data
		 sad::Variant * m_variant; //!< Variant value of data
 public:
	 /** Default property is invalid and empty
	  */
	 SceneLayerProperty();
	 /** Default data objects 
	  */
	 ~SceneLayerProperty();
	 /** Sets a new value of data
		 \param[in] value value data
		 \param[in] context context data
	  */
	 void set(const sad::Variant & value);
	 /** Returns a property value
		 \param[in] context context data
		 \return context data
	  */
	 virtual sad::Variant * get() const;

	 /** Returns a string representation of property
		 \return string representation
	  */
	 virtual sad::String save() const;
	 /** Loads data from string
		 \param[in] str      string data
		 \param[in] context  action context
	  */
	 virtual void load(const sad::String & str);
	 /** Whether prop is saveable
		 \return saveable
	  */
	 virtual bool saveable() const;
};
