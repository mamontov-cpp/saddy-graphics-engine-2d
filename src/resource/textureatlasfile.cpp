#include <resource/textureatlasfile.h>
#include <resource/tree.h>
#include <resource/error.h>

#include <renderer.h>
#include <texture.h>

#include <util/fs.h>
#include <util/free.h>

#include <3rdparty/picojson/getproperty.h>
#include <3rdparty/picojson/valuetotype.h>

#include <fstream>
#include "opticksupport.h"

sad::resource::TextureAtlasFile::TextureAtlasFile(const sad::String& name) 
: sad::resource::ResourceFile(name), m_my_texture(nullptr)
{
    PROFILER_EVENT;
    
}

sad::resource::TextureAtlasFile::~TextureAtlasFile()
{
    PROFILER_EVENT;
    if (m_my_texture)
    {
        delete m_my_texture;
    }
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::load(
    sad::resource::Folder * parent
)
{
    PROFILER_EVENT;
    sad::Vector<sad::resource::Error*> errors;
    sad::resource::TextureAtlasFile::parse_result result;
    this->tryParsePartial(result, errors);
    if (errors.size() == 0)
    {
        if (m_my_texture)
        {
            delete m_my_texture;
        }
        sad::resource::Resource * linkedresource = parent->resource(result.ResourceName);
        if (linkedresource)
        {
            if (linkedresource->metaData()->name() != "sad::Texture")
            {
                errors << new sad::resource::ResourceLoadError(m_name);
                return errors;
            }
        }
        else
        {
            m_my_texture = new sad::resource::ResourceFile(result.FileName);
            m_my_texture->setTree(this->tree());
            sad::Texture* tex = new sad::Texture();
            tex->BuildMipMaps = !result.NoMipMaps;
            linkedresource  = tex;
            bool ok = linkedresource->tryLoad(
                *m_my_texture, 
                this->tree()->renderer(), 
                picojson::value(picojson::object()), 
                this->tree()->shouldStoreLinks()
            );
            if (!ok && !sad::util::isAbsolutePath(result.FileName))
            {
                sad::String my_folder = sad::util::folder(this->name()); 
                sad::String new_path = sad::util::concatPaths(my_folder, result.FileName) ;
                m_my_texture->setName(new_path);
                ok = linkedresource->tryLoad(
                    *m_my_texture, 
                    this->tree()->renderer(), 
                    picojson::value(picojson::object()), 
                    this->tree()->shouldStoreLinks()
                );
            }
            if (!ok)
            {
                delete linkedresource;
                delete m_my_texture;
                errors << new sad::resource::ResourceLoadError(result.ResourceName);
                m_my_texture = nullptr;
                return errors;
            }
        }

        const picojson::array & list = result.EntryList;
        sad::resource::ResourceEntryList resourcelist;
        fillOptionsList(result, resourcelist, linkedresource);
        errors << this->tree()->duplicatesToErrors(parent->duplicatesBetween(resourcelist));

        if (errors.size() != 0)
        {
            if (m_my_texture)
            {
                delete m_my_texture;
                delete linkedresource;
            }
        }
        else
        {
            if (m_my_texture)
            {
                parent->addResource(result.ResourceName, linkedresource);
            }
            this->replaceResources(resourcelist);
            parent->addResources(resourcelist);
        }
    }
    return errors;
}

sad::Vector<sad::resource::Error*> sad::resource::TextureAtlasFile::reload()
{
    PROFILER_EVENT;
    sad::Vector<sad::resource::Error*> errors;
    sad::resource::TextureAtlasFile::parse_result result;
    this->tryParsePartial(result, errors, true);
    if (errors.size() == 0)
    {
        sad::resource::TextureAtlasFile::TextureLoadResult textureloadresult;
        sad::resource::ResourceEntryList resourcelist;
        sad::resource::ResourceEntryList tobeadded, tobereplaced, toberemoved;
        this->tryLoadNewTexture(result, textureloadresult, errors, true);
        if (errors.size() == 0)
        {           
            this->fillOptionsList(
                result, 
                resourcelist,
                textureloadresult.NewTexture
            );
            sad::resource::ResourceEntryList oldresourcelist;
            this->createOldResourceList(oldresourcelist);
            this->diffResourcesLists(oldresourcelist, resourcelist, tobeadded, tobereplaced, toberemoved);
            errors << this->tree()->duplicatesToErrors(this->tree()->root()->duplicatesBetween(tobeadded));
            convertReferencedOptionsToBeRemovedToErrors(toberemoved, errors);
        }

        if (errors.size() == 0)
        {
            sad::resource::Folder * root = this->tree()->root();
            // Add an added resources
            root->addResources(tobeadded);
            // Replace replaced resources
            root->replaceResources(tobereplaced);
            // Remove removable resources
            root->removeResources(toberemoved, true);
            // Replace own resource list, setting correct reference to this
            this->replaceResources(resourcelist);
            // Commit linked texture, storing it in tree
            this->commit(result, textureloadresult);
        }
        else
        {
            sad::resource::free(resourcelist);
            this->free(textureloadresult);
        }

    }
    return errors;  
}

bool sad::resource::TextureAtlasFile::supportsLoadingFromTar7z() const
{
    PROFILER_EVENT;
    return true;
}


void sad::resource::TextureAtlasFile::tryParsePartial(
        sad::resource::TextureAtlasFile::parse_result & result,
        sad::Vector<sad::resource::Error *> & errors,
        bool force_reload
) const
{
    PROFILER_EVENT;
    sad::Maybe<sad::String> maybecontent = this->tryReadToString(force_reload);
    if (maybecontent.exists())
    {
        if (maybecontent.value().consistsOfWhitespaceCharacters())
        {
            errors << new sad::resource::EmptyTextureAtlas(this->name());
            return;
        }
        picojson::value rootvalue = picojson::parse_string(maybecontent.value());
        if (picojson::get_last_error().size() == 0)
        {
            sad::Maybe<sad::String> mayberesource = picojson::to_type<sad::String>(
                picojson::get_property(rootvalue, "resource")
            );
            sad::Maybe<sad::String> maybefile = picojson::to_type<sad::String>(
                picojson::get_property(rootvalue, "file")
            );
            sad::Maybe<bool> maybenomips = picojson::to_type<bool>(
                picojson::get_property(rootvalue, "no-mipmaps")
            );
            picojson::value const * atlas = picojson::get_property(rootvalue, "atlas");
            if (atlas && mayberesource.exists() && maybefile.exists())
            {
                if (atlas->is<picojson::array>())
                {
                    bool schemeok = true;
                    const picojson::array & list = atlas->get<picojson::array>();
                    for (size_t i = 0;  (i < list.size()) && schemeok; i++)
                    {
                        sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
                            picojson::get_property(list[i], "name")
                        );
                        sad::Maybe<sad::Size2D> maybesize = picojson::to_type<sad::Size2D>(
                            picojson::get_property(list[i], "size")
                        );
                        sad::Maybe<sad::Rect2D> maybetexrect = picojson::to_type<sad::Rect2D>(
                            picojson::get_property(list[i], "texrect")
                        );
                        bool entryisvalid = maybename.exists() 
                                && maybesize.exists()
                                && maybetexrect.exists();
                        schemeok = entryisvalid;
                        if (!entryisvalid)
                        {
                            errors << new sad::resource::MalformedResourceEntry(list[i]);
                        }
                        else
                        {
                            if (mayberesource.exists() && maybename.exists())
                            {
                                if (mayberesource.value() == maybename.value())
                                {
                                    errors << new sad::resource::MalformedResourceEntry(list[i]);
                                }
                            }
                        }
                    }
                    if (schemeok)
                    {
                        result.ResourceName = mayberesource.value();
                        result.FileName = maybefile.value();
                        result.NoMipMaps = false;
                        if (maybenomips.exists())
                        {
                            result.NoMipMaps = maybenomips.value();
                        }
                        result.EntryList = list;
                    }
                }
                else
                {
                    errors << new sad::resource::MalformedResourceEntry(*atlas);
                }
            }
            else
            {
                errors << new sad::resource::MalformedResourceEntry(rootvalue);
            }
        }
        else
        {
            errors << new sad::resource::JSONParseError();
        }
    }
    else
    {
        errors << new sad::resource::FileLoadError(m_name);
    }
}

void sad::resource::TextureAtlasFile::tryLoadNewTexture(
    sad::resource::TextureAtlasFile::parse_result & parsed,
    sad::resource::TextureAtlasFile::TextureLoadResult & result,
    sad::Vector<sad::resource::Error *> & errors,
    bool force_reload
) const
{
    PROFILER_EVENT;
    result.OldTexture = this->tree()->root()->resource(parsed.ResourceName);
    result.NewTexture = new sad::Texture();
    sad::resource::ResourceFile * file  = nullptr;
    bool force_reload_texture = false;
    // Try load texture
    if (result.OldTexture)
    {
        if (result.OldTexture->metaData()->canBeCastedTo("sad::Texture") == false)
        {
            errors << new sad::resource::ResourceAlreadyExists(parsed.ResourceName);
            return;
        }
        if (m_my_texture)
        {
            result.NewTextureFile = new sad::resource::ResourceFile(parsed.FileName);
            result.NewTextureFile->setTree(this->tree());
            file = result.NewTextureFile;
        }
        else
        {
            result.NewTextureFile = result.OldTexture->file();
        }
    }
    else
    {
        result.NewTextureFile  = new sad::resource::ResourceFile(parsed.FileName);
        file = result.NewTextureFile;
    }
    sad::Renderer * r = this->tree()->renderer();
    picojson::value v(picojson::object_type, false);
    v.insert("type",     picojson::value(sad::Texture::globalMetaData()->name()));
    v.insert("filename", picojson::value(result.NewTextureFile->name()));
    v.insert("name", picojson::value(parsed.ResourceName));
    if (result.NewTextureFile->rfi().Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE)
    {
        if (this->rfi().Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE)
        {
            force_reload_texture = !(result.NewTextureFile->rfi().ArchiveName == this->rfi().ArchiveName);
        }
        else
        {
            force_reload_texture = true;    
        }
    }
    if (force_reload_texture)
    {
        v.insert("force_archive_reload", true);
    }
    bool ok = result.NewTexture->tryLoad(*file, r, v, this->tree()->shouldStoreLinks());
    if (!ok)
    {
        errors << new sad::resource::ResourceLoadError(parsed.ResourceName);
    }
}

void sad::resource::TextureAtlasFile::commit(
    sad::resource::TextureAtlasFile::parse_result & parsed,
    const sad::resource::TextureAtlasFile::TextureLoadResult & result
)
{
    PROFILER_EVENT;
    if (result.OldTexture)
    {
        sad::resource::ResourceFile* oldfile = result.OldTexture->file();
        this->tree()->root()->replaceResource(parsed.ResourceName, result.NewTexture);
        if (m_my_texture == nullptr)
        {
            oldfile->replace(result.OldTexture, result.NewTexture);
            result.NewTexture->setPhysicalFile(oldfile);
        }
        else
        {
            delete m_my_texture;
            m_my_texture = result.NewTextureFile;
            m_my_texture->setTree(this->tree());
            result.NewTexture->setPhysicalFile(m_my_texture);
        }
    }
    else
    {
        this->tree()->root()->addResource(parsed.ResourceName, result.NewTexture);

        delete m_my_texture;
        m_my_texture = result.NewTextureFile;
        m_my_texture->setTree(this->tree());
        result.NewTexture->setPhysicalFile(m_my_texture);
    }
}

void sad::resource::TextureAtlasFile::free(
    const sad::resource::TextureAtlasFile::TextureLoadResult & result
)
{
    PROFILER_EVENT;
    if (result.NewTexture)
    {
        delete result.NewTexture;
    }
    if (result.NewTextureFile)
    {
        delete result.NewTextureFile;
    }
}

void sad::resource::TextureAtlasFile::fillOptionsList(
    sad::resource::TextureAtlasFile::parse_result & parsed,
    sad::resource::ResourceEntryList & resources,
    sad::resource::Resource * texture
) const
{
    PROFILER_EVENT;
    const picojson::array & list = parsed.EntryList;
    for(size_t i = 0; i < list.size(); i++)
    {
        sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
            picojson::get_property(list[i], "name")
            );
        sad::Maybe<sad::Size2D> maybesize = picojson::to_type<sad::Size2D>(
            picojson::get_property(list[i], "size")
            );
        sad::Maybe<sad::Rect2D> maybetexrect = picojson::to_type<sad::Rect2D>(
            picojson::get_property(list[i], "texrect")
            );
        sad::Maybe<sad::Color> maybetransparent = picojson::to_type<sad::Color>(
            picojson::get_property(list[i], "transparent")
            );
        sad::Sprite2D::Options * opts = new sad::Sprite2D::Options();
        opts->Rectangle = sad::Rect2D(0, 0,  maybesize.value().Width, maybesize.value().Height);
        opts->TextureRectangle = maybetexrect.value();
        opts->Texture = parsed.ResourceName;
        if (this->tree()->shouldStoreLinks())
        {
            opts->enableStoringLinks();
        }
        if (maybetransparent.exists())
        {
            opts->Transparent = true;
            opts->TransparentColor = maybetransparent.value();
            static_cast<sad::Texture*>(texture)->setAlpha(
                255, 
                opts->TransparentColor,
                opts->TextureRectangle
            );
        }
        resources << sad::resource::ResourceEntry(maybename.value(), opts);
    }
}
