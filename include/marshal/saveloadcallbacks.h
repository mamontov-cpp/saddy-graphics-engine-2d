/*! \file   saveloadcallbacks.h
	\author HiddenSeeker

	Here defined a callbacks for saving and loading types of properties of marshalled objects
 */
#include "abstractproperty.h"
#include <primitives/hpoint.h>
#include <primitives/hrect.h>
#include <primitives/hcolor.h>
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
DEFINE_PROPERTY_TYPESTRING( hst::string )
DEFINE_PROPERTY_TYPESTRING( hst::vector<int> )
DEFINE_PROPERTY_TYPESTRING( hPointF )
DEFINE_PROPERTY_TYPESTRING( hRectF )
DEFINE_PROPERTY_TYPESTRING( hst::vector<hPointF> )
DEFINE_PROPERTY_TYPESTRING( hst::color )
/*! A template callback for loading some properties
 */
template<typename T>
class SaveLoadCallback
{
 public:
	 static T load(ActionContext * context,const hst::string & str, const hst::string & typestring = abstract_names::type_string<T>::type())
	{
		std::stringstream stream(str.data());
	    T result = 0;
	    if (!(stream >> result))
	    {
		 throw new serializable::InvalidPropertyValue(typestring,str,context);
	    }

		return result;
	}
	static  hst::string save(const T & obj)
	{
		std::ostringstream stream;
		stream << obj;

		return hst::string(stream.str().c_str());
	}
};

template<>
class SaveLoadCallback<hst::string>
{
 public:
	static hst::string load(ActionContext * context,
						    const hst::string & str, 
							const hst::string & typestring = abstract_names::type_string<hst::string>::type());
	static hst::string save(const hst::string & obj);
};

template<>
class SaveLoadCallback< hst::vector<int> >
{
 public:
	static hst::vector<int> load(ActionContext * context,
								const hst::string & str, 
								const hst::string & typestring = abstract_names::type_string< hst::vector<int> >::type());
	static hst::string save(const hst::vector<int> & obj);
};


template<>
class SaveLoadCallback<hPointF>
{
 public:
	static hPointF load(ActionContext * context,
						    const hst::string & str, 
							const hst::string & typestring = abstract_names::type_string< hPointF >::type() );
	static hst::string save(const hPointF & obj);
};


template<>
class SaveLoadCallback<hRectF>
{
 public:
	static hRectF load(ActionContext * context,
					   const hst::string & str, 
					   const hst::string & typestring = abstract_names::type_string<hRectF>::type());
	static hst::string save(const hRectF & obj);
};


template<>
class SaveLoadCallback< hst::vector<hPointF> >
{
 public:
	static hst::vector<hPointF> load(ActionContext * context,
						             const hst::string & str, 
									 const hst::string & typestring = abstract_names::type_string< hst::vector<hPointF> >::type());
	static hst::string save(const hst::vector<hPointF> & obj);
};


template<>
class SaveLoadCallback< bool >
{
 public:
	static bool load(ActionContext * context,
				     const hst::string & str, 
					 const hst::string & typestring = abstract_names::type_string<bool>::type());
	static hst::string save(const bool & obj);
};


template<>
class SaveLoadCallback< hst::color >
{
 public:
	 static hst::color load(ActionContext * context,
				     const hst::string & str, 
					 const hst::string & typestring =  abstract_names::type_string<hst::color>::type());
	 static hst::string save(const hst::color & obj);
};
