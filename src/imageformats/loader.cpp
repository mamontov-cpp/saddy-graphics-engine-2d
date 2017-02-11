#include "imageformats/loader.h"

#include "db/dbtypename.h"

bool sad::imageformats::Loader::load(tar7z::Entry* entry, sad::Texture* texture)
{
    return false;
}

sad::imageformats::Loader::~Loader()
{

}

DECLARE_COMMON_TYPE(sad::imageformats::Loader)
