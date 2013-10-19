/*! \file   saveloadcallbacks.h
	\author HiddenSeeker

	Here defined a callbacks for saving and loading types of properties of marshalled objects
 */
#include "abstractproperty.h"
#include <sadpoint.h>
#include <sadrect.h>
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
DEFINE_PROPERTY_TYPESTRING( sad::Point2D )
DEFINE_PROPERTY_TYPESTRING( sad::Rect2D )
DEFINE_PROPERTY_TYPESTRING( sad::Vector<sad::Point2D> )
DEFINE_PROPERTY_TYPESTRING( sad::Color )
/*! A template callback for loading some properties
 */
template<typename T>
class SaveLoadCallback
{
 public:
	 static T load(
		 const sad::String & str, 
		 const sad::String & typestring = abstract_names::type_string<T>::type()
	)
	{
		std::stringstream stream(str.data());
	    T result = 0;
	    if (!(stream >> result))
	    {
		 throw new serializable::InvalidPropertyValue(typestring,str);
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
	static sad::String load(const sad::String & str, 
							const sad::String & typestring = abstract_names::type_string<sad::String>::type());
	static sad::String save(const sad::String & obj);
};

template<>
class SaveLoadCallback< sad::Vector<int> >
{
 public:
	static sad::Vector<int> load(const sad::String & str, 
								 const sad::String & typestring = abstract_names::type_string< sad::Vector<int> >::type());
	static sad::String save(const sad::Vector<int> & obj);
};


template<>
class SaveLoadCallback<sad::Point2D>
{
 public:
	static sad::Point2D load(const sad::String & str, 
							 const sad::String & typestring = abstract_names::type_string< sad::Point2D >::type() );
	static sad::String save(const sad::Point2D & obj);
};


template<>
class SaveLoadCallback<sad::Rect2D>
{
 public:
	static sad::Rect2D load(const sad::String & str, 
						    const sad::String & typestring = abstract_names::type_string<sad::Rect2D>::type());
	static sad::String save(const sad::Rect2D & obj);
};


template<>
class SaveLoadCallback< sad::Vector<sad::Point2D> >
{
 public:
	static sad::Vector<sad::Point2D> load(const sad::String & str, 
										  const sad::String & typestring = abstract_names::type_string< sad::Vector<sad::Point2D> >::type());
	static sad::String save(const sad::Vector<sad::Point2D> & obj);
};


template<>
class SaveLoadCallback< bool >
{
 public:
	static bool load(const sad::String & str, 
					 const sad::String & typestring = abstract_names::type_string<bool>::type());
	static sad::String save(const bool & obj);
};


template<>
class SaveLoadCallback< sad::Color >
{
 public:
	 static sad::Color load(const sad::String & str, 
							const sad::String & typestring =  abstract_names::type_string<sad::Color>::type());
	 static sad::String save(const sad::Color & obj);
};
