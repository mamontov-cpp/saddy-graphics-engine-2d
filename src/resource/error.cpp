#include "resource/error.h"

DECLARE_SOBJ(sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::FileLoadError, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::ResourceLoadError, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::MissingResource, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::ResourceAlreadyExists, sad::resource::Error);

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
