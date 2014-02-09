/*! \file resource/error.h
	\author HiddenSeeker
	
	Contains definition of class Error.

	This is a basic class for all resource-related errors
 */
#pragma once
#include <stdexcept>
#include "../sadstring.h"
#include "../3rdparty/format/format.h"
#include "../object.h"

namespace sad
{
	

namespace resource
{

/*! \class Error
	
	A basic class for all resource-related errors
 */
class Error:public sad::Object, public std::runtime_error
{	
SAD_OBJECT
public:	
	/*! Constructs new error
		\param[in] message a error message
	 */
	inline Error(const std::string & message) : std::runtime_error(message)
	{
		
	}
	/*! This class can be inherited 
	 */
	virtual ~Error();
};

/*! \class FileLoadError

	A error, which determines a problem with loading of some file.
 */
class FileLoadError : public resource::Error
{
SAD_OBJECT
public:
	/*! Formats error
		\param[in] file name of file
		\return name of file
	 */
	inline static sad::String format_error(const sad::String & file)
	{
		return fmt::str(fmt::Format("Cannot load file \"{0}\"") << file);
	}

	/*! Constructs a error
		\param[in] name a name of resource
	 */
	inline FileLoadError(const sad::String & name)
	: resource::Error(resource::FileLoadError::format_error(name)), m_name(name)
	{
		
	}

	/*! This class can be inherited 
	 */
	virtual ~FileLoadError();
protected:
	/*! A name of file.
	 */
	sad::String m_name;
};

/*! \class ResourceLoadError
	A resource loading error
 */
class ResourceLoadError : public resource::Error
{
SAD_OBJECT
public:
	/*! Formats error
		\param[in] file name of file
		\return name of file
	 */
	inline static sad::String format_error(const sad::String & name)
	{
		sad::String errorres = name;
		if (errorres.length() == 0)
			errorres = "anonymous";
		return fmt::str(fmt::Format("Cannot load or reload resource \"{0}\"") << errorres);
	}

	/*! Constructs a error for resource
		\param[in] name a name of resource
	 */
	inline ResourceLoadError(const sad::String & name)
	: resource::Error(resource::ResourceLoadError::format_error(name)), m_name(name)
	{
		
	}

	/*! This class can be inherited 
	 */
	virtual ~ResourceLoadError();
protected:
	/*! A name of resource.
	 */
	sad::String m_name;
};


/*! \class MissingResource

	Describes a error, which occurs, when referenced resource is missing in new file, read
	while reloading a list of resources. A resource can be referenced by other objects,
	@see resource::AbstractLink for details.
 */
class MissingResource: public resource::Error  
{	
SAD_OBJECT
public:
	/*! Formats error
		\param[in] file name of file
		\return name of file
	 */
	inline static sad::String format_error(const sad::String & name)
	{
		sad::String errorres = name;
		if (errorres.length() == 0)
			errorres = "anonymous";
		return fmt::str(fmt::Format("Cannot reload resource \"{0}\", referenced by other resources, but missed in reloaded file") 
						<< errorres);
	}

	/*! Constructs a error for missing resource
		\param[in] name a name of resource
	 */
	inline MissingResource(const sad::String & name)
	: resource::Error(resource::MissingResource::format_error(name)), m_name(name)
	{
		
	}

	/*! This class can be inherited 
	 */
	virtual ~MissingResource();
protected:
	/*! A name of resource.
	 */
	sad::String m_name;
};

/*! \class MissingResource

	Describes a error, which occurs, when such resource already exists in current database
	and cannot be replaced. Occurs, when user tries to load seaparate file
 */
class ResourceAlreadyExists: public resource::Error  
{	
SAD_OBJECT
public:
	/*! Formats error
		\param[in] file name of file
		\return name of file
	 */
	inline static sad::String format_error(const sad::String & name)
	{
		sad::String errorres = name;
		if (errorres.length() == 0)
			errorres = "anonymous";
		return fmt::str(fmt::Format("Cannot load resource \"{0}\", which is already exists in current database") 
						<< errorres);
	}

	/*! Constructs a error for missing resource
		\param[in] name a name of resource
	 */
	inline ResourceAlreadyExists(const sad::String & name)
	: resource::Error(resource::ResourceAlreadyExists::format_error(name)), m_name(name)
	{
		
	}

	/*! This class can be inherited 
	 */
	virtual ~ResourceAlreadyExists();
protected:
	/*! A name of resource.
	 */
	sad::String m_name;
};

}

}
