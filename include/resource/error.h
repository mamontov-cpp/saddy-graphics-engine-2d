/*! \file resource/error.h
    
    
    Contains definition of class sad::resource::Error.

    This is a basic class for all resource-related errors
 */
#pragma once
#include <stdexcept>
#include "../sadstring.h"
#include "../3rdparty/format/format.h"
#include "../3rdparty/picojson/picojson.h"
#include "../object.h"
#include "../sadvector.h"

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
    virtual ~Error()  override;
};

/*! \class FileLoadError

    A error, which determines a problem with loading of some file.
 */
class FileLoadError : public sad::resource::Error
{
SAD_OBJECT
public:
    /*! Formats error
        \param[in] file name of file
        \return error string
     */
    inline static sad::String format_error(const sad::String & file)
    {
        return fmt::str(fmt::Format("Cannot load file \"{0}\"") << file);
    }

    /*! Constructs a error
        \param[in] name a name of resource
     */
    inline FileLoadError(const sad::String & name)
    : sad::resource::Error(sad::resource::FileLoadError::format_error(name)), m_file_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~FileLoadError() override;
protected:
    /*! A name of file.
     */
    sad::String m_file_name;
};

/*! \class ResourceLoadError
    A resource loading error
 */
class ResourceLoadError : public sad::resource::Error
{
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name a name of file
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot load or reload resource \"{0}\"") << errors);
    }

    /*! Constructs a error for resource
        \param[in] name a name of resource
     */
    inline ResourceLoadError(const sad::String & name)
    : sad::resource::Error(sad::resource::ResourceLoadError::format_error(name)), m_resource_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~ResourceLoadError()  override;
protected:
    /*! A name of resource.
     */
    sad::String m_resource_name;
};


/*! \class MissingResource

    Describes a error, which occurs, when referenced resource is missing in new file, read
    while reloading a list of resources. A resource can be referenced by other objects,
    @see resource::AbstractLink for details.
 */
class MissingResource: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name a name of missing resource
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot reload resource \"{0}\", referenced by other resources, but missed in reloaded file") 
                        << errors);
    }

    /*! Constructs a error for missing resource
        \param[in] name a name of resource
     */
    inline MissingResource(const sad::String & name)
    : sad::resource::Error(sad::resource::MissingResource::format_error(name)), m_resource_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~MissingResource()  override;
protected:
    /*! A name of resource.
     */
    sad::String m_resource_name;
};

/*! \class ResourceAlreadyExists

    Describes a error, which occurs, when such resource already exists in current database
    and cannot be replaced. Occurs, when user tries to load separate file
 */
class ResourceAlreadyExists: public sad::resource::Error
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name a name of existing resource
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot load resource \"{0}\", which is already exists in current database") 
                        << errors);
    }

    /*! Constructs a error for missing resource
        \param[in] name a name of resource
     */
    inline ResourceAlreadyExists(const sad::String & name)
    : sad::resource::Error(sad::resource::ResourceAlreadyExists::format_error(name)), m_resource_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~ResourceAlreadyExists()  override;
protected:
    /*! A name of resource.
     */
    sad::String m_resource_name;
};

/*! \class CannotDeleteReferencedResource

    Describes a error, which occurs, when such resource already exists in current database
    and cannot be replaced. Occurs, when user tries to load separate file
 */
class CannotDeleteReferencedResource: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name  a name of resource
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot delete resource \"{0}\", which is referenced in database") 
                        << errors);
    }

    /*! Constructs a error for resource
        \param[in] name a name of resource
     */
    inline CannotDeleteReferencedResource(const sad::String & name)
    : sad::resource::Error(sad::resource::CannotDeleteReferencedResource::format_error(name)), m_resource_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~CannotDeleteReferencedResource()  override;
protected:
    /*! A name of resource.
     */
    sad::String m_resource_name;
};

/*! \class UnregisteredFileType

    Describes a error, which occurs, when such resource already exists in current database
    and cannot be replaced. Occurs, when user tries to load separate file
 */
class UnregisteredFileType: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name name of file type
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot load file type with name \"{0}\"") 
                        << errors);
    }

    /*! Constructs a error for unregistered file type
        \param[in] name a type of resource
     */
    inline UnregisteredFileType(const sad::String & name)
    : sad::resource::Error(sad::resource::UnregisteredFileType::format_error(name)), m_resource_type(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~UnregisteredFileType()  override;
protected:
    /*! A name of file.
     */
    sad::String m_resource_type;
};

/*! \class UnregisteredFileType

    Describes a error, which occurs, when such resource required to be created by a factory
    could not be created. Can occur, when reloading a file
 */
class UnregisteredResourceType: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name  a name of resource type
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("Cannot load file type with name \"{0}\"") 
                        << errors);
    }

    /*! Constructs a error for unregistered file type
        \param[in] name a type of resource
     */
    inline UnregisteredResourceType(const sad::String & name)
    : sad::resource::Error(sad::resource::UnregisteredResourceType::format_error(name)), m_file_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~UnregisteredResourceType()  override;
protected:
    /*! A name of file.
     */
    sad::String m_file_name;
};

/*! \class AnonymousResource

    Describes a error, which occurs, when resource has no name
 */
class AnonymousResource: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name a resource name
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("A resource with type  \"{0}\" has no type. Please review your JSON source and name it") 
                        << errors);
    }

    /*! Constructs a error for missing resource
        \param[in] name a name of resource
     */
    inline AnonymousResource(const sad::String & name)
    : sad::resource::Error(sad::resource::AnonymousResource::format_error(name)), m_resource_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~AnonymousResource()  override;
protected:
    /*! A type of resource.
     */
    sad::String m_resource_name;
};

/*! \class FileLoadingNotImplemented

    Describes a error, which occurs, when such resource already exists in current database
    and cannot be replaced. Occurs, when user tries to load separate file
 */
class FileLoadingNotImplemented: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] name a name of file
        \return error string
     */
    inline static sad::String format_error(const sad::String & name)
    {
        sad::String errors = name;
        if (errors.length() == 0)
            errors = "anonymous";
        return fmt::str(fmt::Format("A file of name does not implement  \"{0}\" loading of type") 
                        << errors);
    }

    /*! Constructs a error for missing resource
        \param[in] name a name of resource
     */
    inline FileLoadingNotImplemented(const sad::String & name)
    : sad::resource::Error(sad::resource::FileLoadingNotImplemented::format_error(name)), m_file_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~FileLoadingNotImplemented()  override; 
protected:
    /*! A name of file.
     */
    sad::String m_file_name;
};

/*! \class JSONParseError

    Describes a error, which occurs, when parsing of file failed
 */
class JSONParseError: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \return error string
     */
    inline static sad::String format_error()
    {
        return "Failed to parse JSON";
    }

    /*! Constructs a error for missing resource
     */
    inline JSONParseError()
    : sad::resource::Error(sad::resource::JSONParseError::format_error())
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~JSONParseError()  override;
};

/*! \class MalformedResourceEntry

    Describes an error, when resource entry is malformed for some case
 */
class MalformedResourceEntry: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] v a formatted entry
        \return error string
     */
    inline static sad::String format_error(const picojson::value & v)
    {
	    const std::string error_string = "Malformed entry {0}, please consider revising";
        return str(fmt::Format(error_string) << v.serialize());
    }

    /*! Constructs a error for malformed resource entry
        \param[in] v an invalid entry
     */
    inline MalformedResourceEntry(const picojson::value & v)
    : sad::resource::Error(sad::resource::MalformedResourceEntry::format_error(v))
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~MalformedResourceEntry()  override;
};

/*! \class EmptyTextureAtlas

    Describes an error, when texture atlas file is empty
 */
class EmptyTextureAtlas: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] v a formatted entry
        \return error string
     */
    inline static sad::String format_error(const sad::String & v)
    {
	    const std::string error_string = "Texture atlas file \"{0}\" is empty";
        return str(fmt::Format(error_string) << v);
    }

    /*! Constructs a error for file
        \param[in] name a name of file
     */
    inline EmptyTextureAtlas(const sad::String & name)
    : sad::resource::Error(sad::resource::EmptyTextureAtlas::format_error(name))
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~EmptyTextureAtlas()  override;
};

/*! \class TreeNotFound

    Describes an error, when texture atlas file is empty
 */
class TreeNotFound: public sad::resource::Error  
{	
SAD_OBJECT
public:
    /*! Formats error
        \param[in] v a formatted entry
        \return error string
     */
    inline static sad::String format_error(const sad::String & v)
    {
	    const std::string error_string = "Tree with name \"{0}\" not found";
        return str(fmt::Format(error_string) << v);
    }

    /*! Constructs a error for specified tree name
        \param[in] name a name of tree key
     */
    inline TreeNotFound(const sad::String & name)
    : sad::resource::Error(sad::resource::TreeNotFound::format_error(name))
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~TreeNotFound()  override;
};

/*! \class ResourceCannotBeLoadedFromArchive

    A error, which determines a problem, that this resource cannot be loaded from archive.
 */
class ResourceCannotBeLoadedFromArchive : public sad::resource::Error
{
SAD_OBJECT
public:
    /*! Formats error
        \param[in] file name of file
        \return error string
     */
    inline static sad::String format_error(const sad::String & file)
    {
        return fmt::str(fmt::Format("Resource \"{0}\" cannot be loaded from archive") << file);
    }

    /*! Constructs a error
        \param[in] name a name of resource
     */
    inline ResourceCannotBeLoadedFromArchive(const sad::String & name)
    : sad::resource::Error(sad::resource::ResourceCannotBeLoadedFromArchive::format_error(name)), m_file_name(name)
    {
        
    }

    /*! This class can be inherited 
     */
    virtual ~ResourceCannotBeLoadedFromArchive()  override;
protected:
    /*! A name of file.
     */
    sad::String m_file_name;
};

/*! Formats an error to string. Remember, that you still need to free errors.
    \param[in] errors a list of errors
    \param[in] line_format a format of line (default is just a message)
    \param[in] separator a separator for lines
    \return list of errors as string
 */
sad::String format(
    const sad::Vector<sad::resource::Error *> & errors,
    const sad::String & line_format = "{0}",
    const sad::String & separator = "\n"
);

/*! Converts error list to string
    \param[in] errors list of errors
    \return string or empty
 */ 
sad::Maybe<sad::String> errorsToString(const sad::Vector<sad::resource::Error *>& errors);

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::Error)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::FileLoadError)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::ResourceLoadError)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::MissingResource)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::ResourceAlreadyExists)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::CannotDeleteReferencedResource)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::UnregisteredFileType)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::UnregisteredResourceType)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::AnonymousResource)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::FileLoadingNotImplemented)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::JSONParseError)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::MalformedResourceEntry)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::EmptyTextureAtlas)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::TreeNotFound)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::resource::ResourceCannotBeLoadedFromArchive)

