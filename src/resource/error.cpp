#include "resource/error.h"
#include "3rdparty/format/format.h"
#include "resource/resourcefile.h"

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
DECLARE_SOBJ_INHERITANCE(sad::resource::EmptyTextureAtlas, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::TreeNotFound, sad::resource::Error);
DECLARE_SOBJ_INHERITANCE(sad::resource::ResourceCannotBeLoadedFromArchive, sad::resource::Error);

sad::resource::Error::~Error() throw()
{

}

sad::resource::FileLoadError::~FileLoadError() throw()
{
    
}

sad::resource::ResourceLoadError::~ResourceLoadError() throw()
{
    
}

sad::resource::MissingResource::~MissingResource() throw()
{

}

sad::resource::ResourceAlreadyExists::~ResourceAlreadyExists() throw()
{
    
}

sad::resource::CannotDeleteReferencedResource::~CannotDeleteReferencedResource() throw()
{
    
}

sad::resource::UnregisteredFileType::~UnregisteredFileType() throw()
{
    
}

sad::resource::UnregisteredResourceType::~UnregisteredResourceType() throw()
{
    
}

sad::resource::AnonymousResource::~AnonymousResource() throw()
{
    
}

sad::resource::FileLoadingNotImplemented::~FileLoadingNotImplemented() throw()
{
    
}

sad::resource::JSONParseError::~JSONParseError() throw()
{
    
}

sad::resource::MalformedResourceEntry::~MalformedResourceEntry() throw()
{

}

sad::resource::EmptyTextureAtlas::~EmptyTextureAtlas() throw()
{

}

sad::resource::TreeNotFound::~TreeNotFound() throw()
{
    
}

sad::resource::ResourceCannotBeLoadedFromArchive::~ResourceCannotBeLoadedFromArchive() throw()
{

}


sad::String sad::resource::format(
    const sad::Vector<sad::resource::Error *> & errors,
    const sad::String & lineformat,
    const sad::String & separator
)
{
    sad::Vector<sad::String> list;
    for(size_t i = 0; i < errors.size(); i++)
    {
        list << fmt::str(fmt::Format(lineformat) << sad::String(errors[i]->what()));
    }
    return sad::join(list, separator);
}


sad::Maybe<sad::String> sad::resource::errorsToString(const sad::Vector<sad::resource::Error *>& errors)
{
    if (errors.size())
    {
        sad::String result = sad::resource::format(errors);
        sad::resource::free_vector(errors);
        return sad::Maybe<sad::String>(result);
    }
    return sad::Maybe<sad::String>();
}