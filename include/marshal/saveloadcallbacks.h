/*! \file   saveloadcallbacks.h
	\author HiddenSeeker

	Here defined a callbacks for saving and loading types of properties of marshalled objects
 */
#include "abstractproperty.h"
#include <primitives/hpoint.h>
#include <primitives/hrect.h>
#include <sadcolor.h>
#include <sstream>
#pragma once


DEFINE_PROPERTY_TYPESTRING( int )
DEFINE_PROPERTY_TYPESTRING( unsigned int )
DEFINE_PROPERTY_TYPESTRING( long )
DEFINE_PROPERTY_TYPESTRING( unsigned long )
DEFINE_PROPERTY_TYPESTRING( long long )
DEFINE_PROPERTY_TYPESTRING( unsigned long long )
DEFINE_PROPERTY_TYPESTRING( float )
DEFINE_PROPERTY_TYPESTRING( double )
DEFINE_PROPERTY_TYPESTRING( bool )
DEFINE_PROPERTY_TYPESTRING( sad::String )
DEFINE_PROPERTY_TYPESTRING( sad::Vector<int> )
DEFINE_PROPERTY_TYPESTRING( hPointF )
DEFINE_PROPERTY_TYPESTRING( hRectF )
DEFINE_PROPERTY_TYPESTRING( sad::Vector<hPointF> )
DEFINE_PROPERTY_TYPESTRING( sad::Color )
/*! A template callback for loading some properties
 */
template<typename T>
class SaveLoadCallback
{
 public:
	 static T load(ActionContext * context,const sad::String & str, const sad::String & typestring = abstract_names::type_string<T>::type())
	{
		std::stringstream stream(str.data());
	    T result = 0;
	    if (!(stream >> result))
	    {
		 throw new serializable::InvalidPropertyValue(typestring,str,context);
	    }

		return result;
	}
	static  sad::String save(const T & obj)
	{
		std::ostringstream stream;
		stream << obj;

		return sad::String(stream.str().c_str());
	}
};

template<>
class SaveLoadCallback<sad::String>
{
 public:
	static sad::String load(ActionContext * context,
						    const sad::String & str, 
							const sad::String & typestring = abstract_names::type_string<sad::String>::type());
	static sad::String save(const sad::String & obj);
};

template<>
class SaveLoadCallback< sad::Vector<int> >
{
 public:
	static sad::Vector<int> load(ActionContext * context,
								const sad::String & str, 
								const sad::String & typestring = abstract_names::type_string< sad::Vector<int> >::type());
	static sad::String save(const sad::Vector<int> & obj);
};


template<>
class SaveLoadCallback<hPointF>
{
 public:
	static hPointF load(ActionContext * context,
						    const sad::String & str, 
							const sad::String & typestring = abstract_names::type_string< hPointF >::type() );
	static sad::String save(const hPointF & obj);
};


template<>
class SaveLoadCallback<hRectF>
{
 public:
	static hRectF load(ActionContext * context,
					   const sad::String & str, 
					   const sad::String & typestring = abstract_names::type_string<hRectF>::type());
	static sad::String save(const hRectF & obj);
};


template<>
class SaveLoadCallback< sad::Vector<hPointF> >
{
 public:
	static sad::Vector<hPointF> load(ActionContext * context,
						             const sad::String & str, 
									 const sad::String & typestring = abstract_names::type_string< sad::Vector<hPointF> >::type());
	static sad::String save(const sad::Vector<hPointF> & obj);
};


template<>
class SaveLoadCallback< bool >
{
 public:
	static bool load(ActionContext * context,
				     const sad::String & str, 
					 const sad::String & typestring = abstract_names::type_string<bool>::type());
	static sad::String save(const bool & obj);
};


template<>
class SaveLoadCallback< sad::Color >
{
 public:
	 static sad::Color load(ActionContext * context,
				     const sad::String & str, 
					 const sad::String & typestring =  abstract_names::type_string<sad::Color>::type());
	 static sad::String save(const sad::Color & obj);
};
