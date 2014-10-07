/*! \file   saveloadcallbacks.h
	\author HiddenSeeker

	Here defined a callbacks for saving and loading types of properties of marshalled objects
 */
#pragma once
#include "../sadstring.h"
#include "../sadpoint.h"
#include "../sadrect.h"
#include "../sadcolor.h"
#include "../3rdparty/format/format.h"

#include <sstream>
#include <stdexcept>

namespace abstract_names
{

template<
	typename T
>
class type_string
{
public:
	static sad::String type()
	{
		return "TYPE";
	}
};

}

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
			std::string formatstring = "Invalid value {0} for type {1}";
			std::string message = fmt::str( fmt::Format(formatstring)
				<< str
				<< typestring
			);
			throw std::logic_error(message);
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
