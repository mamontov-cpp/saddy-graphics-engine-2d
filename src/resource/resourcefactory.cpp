#include "resource/resourcefactory.h"
#include "resource/resourcefile.h"
#include "resource/textureatlasfile.h"

#include "db/dbstoredpropertyfactory.h"
#include "db/custom/customschemafile.h"

#include "animations/animationsfile.h"

#include "texture.h"
#include "texturemappedfont.h"
#include "sprite2d.h"

#include <cassert>
#include "opticksupport.h"

sad::resource::Creator::~Creator()
{
    PROFILER_EVENT;

}

sad::resource::FileCreator::~FileCreator()
{
    PROFILER_EVENT;

}

sad::resource::Factory::Factory()
{
    PROFILER_EVENT;
    add(sad::Texture::globalMetaData()->name(), new resource::CreatorFor<sad::Texture>());
    add(sad::TextureMappedFont::globalMetaData()->name(), 
        new resource::CreatorFor<sad::TextureMappedFont>());
    add(sad::Sprite2D::Options::globalMetaData()->name(), 
        new resource::CreatorFor<sad::Sprite2D::Options>());

    m_factory = new sad::db::StoredPropertyFactory();
}

sad::resource::Factory::~Factory()
{
    PROFILER_EVENT;
    delete m_factory;
}

void sad::resource::Factory::add(const sad::String & name, resource::Creator * c)
{
    PROFILER_EVENT;
    if (c)
    {
        if (m_creators.contains(name))
        {
            delete m_creators[name]; //-V515
        }
        m_creators.insert(name, c);
    }
}

sad::resource::Resource* sad::resource::Factory::create(const sad::String& name)
{
    PROFILER_EVENT;
    sad::resource::Resource* result = nullptr;
    if (m_creators.contains(name))
    {
        resource::Creator * c = m_creators[name];
        result = c->create();	
    }
    return result;	
}

sad::resource::ResourceFile * sad::resource::Factory::fileByType(const sad::String & type_hint)
{
    PROFILER_EVENT;
    if (type_hint == "sad::resource::TextureAtlasFile")
    {
        return new sad::resource::TextureAtlasFile();
    }
    if (type_hint == "sad::animations::File")
    {
        return new sad::animations::File();
    }
    if (type_hint == "sad::db::custom::SchemaFile")
    {
        sad::db::custom::SchemaFile* file = new sad::db::custom::SchemaFile();
        file->setFactory(m_factory);
        return file;
    }
    if (type_hint == "sad::Texture" 
        || type_hint == "sad::TextureMappedFont"
        || type_hint == "sad::freetype::Font")
    {
        return new sad::resource::ResourceFile();
    }
    if (m_file_creators.contains(type_hint))
    {
        return m_file_creators[type_hint]->create();
    }
    return nullptr;
}

void sad::resource::Factory::setStoredPropertyFactory(sad::db::StoredPropertyFactory * factory)
{
    PROFILER_EVENT;
    assert(m_factory);
    delete m_factory;
    m_factory = factory;
}

sad::db::StoredPropertyFactory* sad::resource::Factory::storedPropertyFactory() const
{
    PROFILER_EVENT;
    return m_factory;
}
