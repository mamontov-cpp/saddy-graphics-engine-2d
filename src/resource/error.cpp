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

sad::resource::Error::~Error() = default;

sad::resource::FileLoadError::~FileLoadError() = default;

sad::resource::ResourceLoadError::~ResourceLoadError() = default;

sad::resource::MissingResource::~MissingResource() = default;

sad::resource::ResourceAlreadyExists::~ResourceAlreadyExists() = default;

sad::resource::CannotDeleteReferencedResource::~CannotDeleteReferencedResource() = default;

sad::resource::UnregisteredFileType::~UnregisteredFileType() = default;

sad::resource::UnregisteredResourceType::~UnregisteredResourceType() = default;

sad::resource::AnonymousResource::~AnonymousResource() = default;

sad::resource::FileLoadingNotImplemented::~FileLoadingNotImplemented() = default;

sad::resource::JSONParseError::~JSONParseError() = default;

sad::resource::MalformedResourceEntry::~MalformedResourceEntry() = default;

sad::resource::EmptyTextureAtlas::~EmptyTextureAtlas() = default;

sad::resource::TreeNotFound::~TreeNotFound() = default;

sad::resource::ResourceCannotBeLoadedFromArchive::~ResourceCannotBeLoadedFromArchive() = default;



sad::String sad::resource::format(
    const sad::Vector<sad::resource::Error *> & errors,
    const sad::String & line_format,
    const sad::String & separator
)
{
    sad::Vector<sad::String> list;
    for(size_t i = 0; i < errors.size(); i++)
    {
        list << fmt::str(fmt::Format(line_format) << sad::String(errors[i]->what()));
    }
    return sad::join(list, separator);
}


sad::Maybe<sad::String> sad::resource::errorsToString(const sad::Vector<sad::resource::Error *>& errors)
{
    if (!errors.empty())
    {
	    const sad::String result = sad::resource::format(errors);
        sad::resource::free_vector(errors);
        return sad::Maybe<sad::String>(result);
    }
    return sad::Maybe<sad::String>();
}