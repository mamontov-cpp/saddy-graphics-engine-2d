#include "resource/error.h"

DECLARE_SOBJ(sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::FileLoadError, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::ResourceLoadError, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::MissingResource, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::ResourceAlreadyExists, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::CannotDeleteReferencedResource, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::UnregisteredFileType, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::UnregisteredResourceType, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::AnonymousResource, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::FileLoadingNotImplemented, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::JSONParseError, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::MalformedResourceEntry, sad::resource::Error);

sad::resource::Error::~Error()
{

}

sad::resource::FileLoadError::~FileLoadError()
{
	
}

sad::resource::ResourceLoadError::~ResourceLoadError()
{
	
}

sad::resource::MissingResource::~MissingResource()
{

}

sad::resource::ResourceAlreadyExists::~ResourceAlreadyExists()
{
	
}

sad::resource::CannotDeleteReferencedResource::~CannotDeleteReferencedResource()
{
	
}

sad::resource::UnregisteredFileType::~UnregisteredFileType()
{
	
}

sad::resource::UnregisteredResourceType::~UnregisteredResourceType()
{
	
}

sad::resource::AnonymousResource::~AnonymousResource()
{
	
}

sad::resource::FileLoadingNotImplemented::~FileLoadingNotImplemented()
{
	
}

sad::resource::JSONParseError::~JSONParseError()
{
	
}

sad::resource::MalformedResourceEntry::~MalformedResourceEntry()
{

}
