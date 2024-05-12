#include "resource/tree.h"
#include "resource/resource.h"
#include "resource/resourcefile.h"

#include "renderer.h"

#include "3rdparty/picojson/picojson.h"
#include "3rdparty/picojson/valuetotype.h"

#include "util/free.h"
#include "util/fs.h"
#include "opticksupport.h"

// ReSharper disable once CppInconsistentNaming
#define TAR7Z_SADDY  // NOLINT(clang-diagnostic-unused-macros)

// ReSharper disable once CppUnusedIncludeDirective
#include "3rdparty/tar7z/include/tar.h"
#include "3rdparty/tar7z/include/reader.h"

#include <fstream>

sad::resource::Tree::Tree(sad::Renderer * r)
: 
m_root(new sad::resource::Folder()), 
m_temporary_root_folder(nullptr), 
m_factory(new sad::resource::Factory()),
m_renderer(r),
m_store_links(false)
{
    PROFILER_EVENT;
    if (r == nullptr)
    {
        m_renderer = sad::Renderer::ref();
    }

    const std::function<void(const sad::String&)> default_on_load = [](const sad::String&) -> void {};
    m_on_load_started = default_on_load;
    m_on_load_finished = default_on_load;
    const std::function<void(size_t)> default_on_count = [](size_t) -> void {};
    m_on_count = default_on_count;
    const std::function<void(sad::resource::Error*)> default_on_error = [](sad::resource::Error*) -> void {};
    m_on_error = default_on_error;
}

sad::resource::Tree::~Tree()
{
    PROFILER_EVENT;
    delete m_root;
    delete m_factory;
    for (size_t i = 0; i < m_archive_list.size(); i++)
    {
        delete m_archive_list[i];
    }
    for (sad::resource::Tree* tree  :m_subtrees)
    {
        if (tree)
        {
            tree->delRef();
        }
    }
}

void sad::resource::Tree::setOnLoad(const std::function<void(size_t)>& on_count, const std::function<void(const sad::String&)>& on_load_started, const std::function<void(const sad::String&)>& on_load_finished)
{
    PROFILER_EVENT;
    m_on_count = on_count;
    m_on_load_started = on_load_started;
    m_on_load_finished = on_load_finished;
}

void sad::resource::Tree::setOnError(const std::function<void(sad::resource::Error*)>& on_error)
{
    PROFILER_EVENT;
    m_on_error = on_error;
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::loadFromString(const sad::String & string)
{
    PROFILER_EVENT;
    clearCache();
    m_temporary_root_folder = nullptr;
    sad::Vector<sad::resource::Error*> errors;
    
    // Check string for emptiness - should we do anything
    if (string.consistsOfWhitespaceCharacters())
    {
        return errors;
    }

    picojson::value v = picojson::parse_string(string);
    if (picojson::get_last_error().empty() && v.is<picojson::array>())
    {
        // Check new root errors
        sad::resource::Folder * new_root = new sad::resource::Folder();
        m_temporary_root_folder = new_root;
        sad::Vector<sad::resource::ResourceFile *> new_files;

        // Try load data to temporary containers
        picojson::array & resource_list = v.get<picojson::array>();
        m_on_count(resource_list.size());
        for(size_t i = 0 ; i < resource_list.size() && errors.empty(); i++)
        {
            sad::Maybe<sad::String>  maybe_type = picojson::to_type<sad::String>(
                picojson::get_property(resource_list[i], "type")
            );
            sad::Maybe<sad::String>  maybe_name = picojson::to_type<sad::String>(
                picojson::get_property(resource_list[i], "filename")
            );
            if (maybe_type.exists() && maybe_name.exists())
            {
                sad::Maybe<sad::String>  maybe_resource_name = picojson::to_type<sad::String>(
                            picojson::get_property(resource_list[i], "name")
                        );

                m_on_load_started(maybe_name.value());
                const sad::Vector<sad::resource::Error*> loading_errors = load(
                    maybe_type.value(), 
                    maybe_name.value(), 
                    maybe_resource_name, 
                    new_root, 
                    resource_list[i], 
                    new_files
                );
                fireOnError(loading_errors);
                errors << loading_errors;
                m_on_load_finished(maybe_name.value());
            } 
            else
            {
                errors << new sad::resource::MalformedResourceEntry(resource_list[i]);
                m_on_error(errors[errors.size() - 1]);
            }
        }

        // Try to copy resources to a core
        if (errors.empty())
        {
            const sad::resource::ResourceEntryList list = new_root->copyAndClear();
            const sad::Vector<sad::resource::Error*> duplicate_errors = duplicatesToErrors(m_root->duplicatesBetween(list));
            fireOnError(duplicate_errors);
            errors << duplicate_errors;
            if (errors.empty())
            {
                m_root->addResources(list, false);
                m_files << new_files;
                delete new_root;
            }
            else
            {
                sad::resource::free(list);
            }
        }
        
        if (!errors.empty())
        {
            delete new_root;
            util::free(new_files);
        }
    }
    else
    {
        if (picojson::get_last_error().empty())
        {
            errors << new  sad::resource::MalformedResourceEntry(v);
        }
        else
        {
            errors << new sad::resource::JSONParseError();
        }
        m_on_error(errors[errors.size() - 1]);
    }
    if (errors.empty())
    {
        m_current_root = m_temporary_root;
    }
    m_temporary_root_folder = nullptr; //-V519
    return errors;
}


sad::Maybe<sad::String> sad::resource::Tree::tryLoadFromString(const sad::String & string)
{
    PROFILER_EVENT;
    return sad::resource::errorsToString(this->loadFromString(string));
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::loadFromFile(const sad::String& string)
{
    PROFILER_EVENT;
    std::ifstream stream(string.c_str());
    if (sad::util::isAbsolutePath(string))
    {
        m_temporary_root = sad::util::folder(string);
    } 
    else
    {
        m_temporary_root = "";
    }
    if (stream.good())
    {
        const std::string all_data(
            (std::istreambuf_iterator<char>(stream)), 
            std::istreambuf_iterator<char>()
        );
        return loadFromString(all_data);
    }
    else
    {
        if (util::isAbsolutePath(string) == false)
        {
            const sad::String path = util::concatPaths(m_renderer->executablePath(), string);
            stream.clear();
            stream.open(path.c_str());
            if (stream.good())
            {
                const std::string all_data(
                    (std::istreambuf_iterator<char>(stream)), 
                     std::istreambuf_iterator<char>()
                    );
                return loadFromString(all_data);
            }
        }
    }
    sad::Vector<sad::resource::Error*> result;
    sad::resource::Error* error = new sad::resource::FileLoadError(string);
    result << error;
    m_on_error(error);
    return result;
}

sad::Maybe<sad::String> sad::resource::Tree::tryLoadFromFile(const sad::String & string)
{
    PROFILER_EVENT;
    return sad::resource::errorsToString(this->loadFromFile(string));
}


sad::Vector<sad::resource::Error*> sad::resource::Tree::load(
        const sad::String& type_hint, 
        const sad::String& filename, 
        const sad::Maybe<sad::String>& resource_name
)
{
    PROFILER_EVENT;
    return load(
        type_hint, 
        filename, 
        resource_name, 
        m_root, 
        picojson::value(picojson::null_type, false), 
        m_files
    );
}

sad::Vector<sad::resource::Error*> sad::resource::Tree::load(
    const sad::String& type_hint, 
    const sad::String& filename, 
    const sad::Maybe<sad::String>& resource_name,
    sad::resource::Folder * store,
    const picojson::value & v,
    sad::Vector<sad::resource::ResourceFile *> & files
)
{
    PROFILER_EVENT;
    sad::resource::Folder * temporary = new sad::resource::Folder();
    sad::Vector<sad::resource::Error*> errors;
    
    // Fill picojson value if needed
    picojson::value resource_description = v;
    sad::String new_filename = filename;
    if (m_temporary_root.length() != 0)
    {
        new_filename = sad::util::concatPaths(m_temporary_root, filename);
    }
    if (resource_description.is<picojson::object>() == false)
    {
        resource_description = picojson::object();
        resource_description.insert("type", picojson::value(type_hint));
        resource_description.insert("filename", picojson::value(new_filename));
        if (resource_name.exists())
        {
            resource_description.insert("name", picojson::value(resource_name.value()));
        }
    }

    // Try create physical file
    sad::resource::ResourceFile * file = m_factory->fileByType(type_hint);
    // First of all file could not be created sometimes
    const int tar7z_length = 6;
    const bool is_tar7z = new_filename.startsWith("tar7z:", tar7z_length);
    if (file)
    {
        file->setTree(this);
        file->setName(new_filename);
        sad::resource::Resource  * resource = m_factory->create(type_hint);
        // Sometimes a resource takes care of loading itself, otherwise it could be done
        // via file
        if (resource)
        {
            resource->setFactoryName(type_hint);
            bool ok = false;
            if (!is_tar7z || resource->supportsLoadingFromTar7z())
            {
                ok = resource->tryLoad(*file, m_renderer, resource_description , m_store_links);
            }
            // Sometimes loading of resource could fail
            if (ok)
            {
                file->add(resource);
                resource->setPhysicalFile(file);
                // When appending a resource, resource could be anonymous
                if (resource_name.exists())
                {
                    // Sometimes resource could be duplicated
                    if (temporary->resource(resource_name.value()) != nullptr)
                    {
                        errors << new sad::resource::ResourceAlreadyExists(resource_name.value());
                        delete resource;
                    }
                    else
                    {
                        resource->setName(resource_name.value());
                        temporary->addResource(resource_name.value(), resource);
                    }
                }
                else
                {
                    errors << new sad::resource::AnonymousResource(filename);
                    delete resource;
                }
            }
            else
            {
                bool unsupported = false;
                if ((resource->supportsLoadingFromTar7z() == false) && is_tar7z)
                {
                    unsupported = true;
                }
                delete resource;
                sad::String file_error_name = filename;
                if (resource_name.exists())
                {
                    file_error_name = resource_name.value();
                }
                if (unsupported)
                {
                    errors << new sad::resource::ResourceCannotBeLoadedFromArchive(file_error_name);                    
                }
                else
                {
                    errors << new sad::resource::ResourceLoadError(file_error_name);
                }
            }

        }
        else
        {
            if (file->supportsLoadingFromTar7z() || !is_tar7z)
            {
                errors << file->load(temporary);
            }
            else
            {
                errors << new sad::resource::ResourceCannotBeLoadedFromArchive(file->name());
            }
        }
    }
    else
    {
        errors << new sad::resource::UnregisteredFileType(type_hint);
    }

    // Cleanup all state or try to copy all data, if errors not found 
    // Errors still can persist due to duplication of resources in store and temporary
    if (errors.empty())
    {
        // Checks whether resources already exists in stored place
        const sad::resource::ResourceEntryList list = temporary->copyAndClear();
        errors << duplicatesToErrors( store->duplicatesBetween(list) );

        // If everything is ok, append all elements, otherwise free lists
        if (errors.empty())
        {
            files << file;
            store->addResources(list, false);
            clearCache();
            delete temporary;
        }
        else
        {
            sad::resource::free(list);
        }
    }

    // If some errors exists, destroy temporary
    if (!errors.empty())
    {
        delete file;
        delete temporary;
    }
    return errors;
}

bool sad::resource::Tree::unload(const sad::String& file)
{
    PROFILER_EVENT;
    sad::resource::ResourceFile * f = this->file(file);
    if (f)
    {
        sad::Vector<sad::resource::Resource*>  list = f->resources();
        bool referenced = false;
        for(size_t i = 0; i < list.size(); i++)
        {
            referenced = referenced || list[i]->referenced();
        }
        if (referenced)
            return false;
        for(size_t i = 0; i < list.size(); i++)
        {
            sad::Maybe<sad::String> path = this->root()->find(list[i]);
            if (path.exists())
            {
                this->root()->removeResource(path.value(), true);
            } 
            else
            {
                delete list[i];
            }
        }
        clearCache();
        m_files.removeAll(f);
        delete f;
        return true;
    }
    return false;
}

bool sad::resource::Tree::unload(sad::resource::ResourceFile * file)
{
    PROFILER_EVENT;
    sad::resource::ResourceFile * f = file;
    if (f)
    {
        sad::Vector<sad::resource::Resource*>  list = f->resources();
        bool referenced = false;
        for(size_t i = 0; i < list.size(); i++)
        {
            referenced = referenced || list[i]->referenced();
        }
        if (referenced)
            return false;
        for(size_t i = 0; i < list.size(); i++)
        {
            sad::Maybe<sad::String> path = this->root()->find(list[i]);
            if (path.exists())
            {
                this->root()->removeResource(path.value(), true);
            } 
            else
            {
                delete list[i];
            }
        }
        clearCache();
        m_files.removeAll(f);
        delete f;
        return true;
    }
    return false;
}

sad::resource::Folder* sad::resource::Tree::root() const
{
    PROFILER_EVENT;
    return m_root;
}

sad::resource::ResourceFile* sad::resource::Tree::file(const sad::String& name)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_files.size(); i++)
    {
        if (m_files[i]->name() == name)
            return m_files[i];
    }
    return nullptr;
}

const sad::Vector<sad::resource::ResourceFile*>& sad::resource::Tree::files() const
{
    PROFILER_EVENT;
    return m_files;
}

sad::resource::Factory* sad::resource::Tree::factory() const
{
    PROFILER_EVENT;
    return m_factory;
}

void sad::resource::Tree::setFactory(sad::resource::Factory* factory)
{
    PROFILER_EVENT;
    delete m_factory;
    m_factory = factory;
}

void sad::resource::Tree::setRenderer(sad::Renderer * renderer)
{
    PROFILER_EVENT;
    m_renderer = renderer;
}

sad::Renderer * sad::resource::Tree::renderer() const
{
    PROFILER_EVENT;
    return m_renderer;
}

bool sad::resource::Tree::shouldStoreLinks() const
{
    PROFILER_EVENT;
    return m_store_links;
}

void sad::resource::Tree::setStoreLinks(bool store)
{
    PROFILER_EVENT;
    m_store_links = store;
}

sad::Vector<sad::resource::Error *> sad::resource::Tree::duplicatesToErrors(
        const sad::Vector<sad::String> & l
)
{
    PROFILER_EVENT
    sad::Vector<sad::resource::Error *> result;
    for (size_t i = 0; i < l.size(); i++)
    {
        result << new sad::resource::ResourceAlreadyExists(l[i]);
    }
    return result;
}

sad::resource::Resource* sad::resource::Tree::resource(const sad::String& name)
{
    PROFILER_EVENT
    const auto iterator = m_resource_cache.find(name);
    if (iterator != m_resource_cache.end())
    {
        return iterator->second;
    }
    sad::resource::Folder* root = this->root();
    sad::resource::Resource* result = nullptr;
    if (root)
    {
        result = root->resource(name);
    }
    if (!result)
    {
        for (size_t i = 0; (i < m_subtrees.size()) && (!result); ++i)
        {
            result = m_subtrees[i]->resource(name);
        }
    }
    m_resource_cache.insert(name, result);
    return result;
}

sad::resource::Resource* sad::resource::Tree::getResourceOfClass(const sad::String& name, const sad::String& class_name)
{
    PROFILER_EVENT
    auto iterator = m_class_resource_cache.find(class_name);
    if (iterator == m_class_resource_cache.end())
    {
        m_class_resource_cache.insert(class_name, sad::Hash<sad::String, sad::resource::Resource*>());
        iterator = m_class_resource_cache.find(class_name);
    }
    const auto inner_iterator = iterator->second.find(name);
    if (inner_iterator != iterator->second.end())
    {
        return inner_iterator->second;
    }
    sad::resource::Folder* folder = this->root();
    sad::resource::Resource* res = nullptr;
    if (folder)
    {
        res = folder->resource(name);
    }
    if (res)
    {
        if (res->metaData()->canBeCastedTo(class_name))
        {
            // We cannot use checked cast because type stored as string, so we store 
            // only resource type as string
            sad::Object * tmp = res->metaData()->castTo(res, class_name);
            res = static_cast<sad::resource::Resource*>(tmp);
        }
    }
    if (!res)
    {
        for (size_t i = 0; (i < m_subtrees.size()) && (!res); ++i)
        {
            res = m_subtrees[i]->getResourceOfClass(name, class_name);
        }
    }
    iterator->second.insert(name, res);
    return res;
}

sad::resource::Folder * sad::resource::Tree::temporaryRoot() const
{
    PROFILER_EVENT
    return m_temporary_root_folder;
}

void sad::resource::Tree::unloadResourcesFromGPU() const
{
    PROFILER_EVENT
    this->root()->unloadResourcesFromGPU();
}

tar7z::Entry* sad::resource::Tree::archiveEntry(const sad::String& archive, const sad::String& filename, bool load_new)
{
    PROFILER_EVENT
    if (m_archives.contains(archive) && !load_new)
    {
        return m_archive_list[m_archives[archive]]->file(filename);
    }
    tar7z::Archive* ar = new tar7z::Archive();
    tar7z::Reader r;
    bool ok = true;
    if (r.read(archive, *ar) != tar7z::Error::T7ZE_OK)
    {
        ok = false;
        if (sad::util::isAbsolutePath(archive) == false)
        {
            if (m_temporary_root.length() != 0)
            {
                const sad::String path = sad::util::concatPaths(m_temporary_root, archive);
                if (r.read(path, *ar) == tar7z::Error::T7ZE_OK)
                {
                    ok = true;
                }
            }

            if (!ok)
            {
                const sad::String path = sad::util::concatPaths(m_renderer->executablePath(), archive);
                if (r.read(path, *ar) == tar7z::Error::T7ZE_OK)
                {
                    ok = true;
                }
            }
        }        
    }
    if (ok)
    {
        m_archive_list << ar;
        m_archives.insert(archive, static_cast<unsigned int>(m_archive_list.size()) - 1u);
        return ar->file(filename);
    }
    
    delete ar;
    return nullptr;
}

const sad::Vector<sad::resource::Tree*>& sad::resource::Tree::subtrees() const
{
    PROFILER_EVENT
    return m_subtrees;
}

void sad::resource::Tree::addSubtree(sad::resource::Tree* tree)
{
    PROFILER_EVENT
    if (tree)
    {
        if (std::find(m_subtrees.begin(), m_subtrees.end(), tree) == m_subtrees.end())
        {
            m_subtrees << tree;
            tree->addRef();
            clearCache();
        }
    }
}

void sad::resource::Tree::removeSubtree(sad::resource::Tree* tree)
{
    PROFILER_EVENT
    if (tree)
    {
        if (std::find(m_subtrees.begin(), m_subtrees.end(), tree) != m_subtrees.end())
        {
            m_subtrees.removeFirst(tree);
            tree->delRef();
            clearCache();
        }
    }
}

DECLARE_COMMON_TYPE(sad::resource::Tree)


// ==================================== PROTECTED METHODS ==================================

void sad::resource::Tree::fireOnError(const sad::Vector<sad::resource::Error*>& errors) const
{
    PROFILER_EVENT
    if (errors.empty())
    {
        return;
    }
    for (sad::resource::Error* error : errors)
    {
        m_on_error(error);
    }
}


void sad::resource::Tree::clearCache()
{
    PROFILER_EVENT
    m_resource_cache.clear();
    m_class_resource_cache.clear();
}