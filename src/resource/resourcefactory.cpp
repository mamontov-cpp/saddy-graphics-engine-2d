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

sad::resource::Creator::~Creator()
{

}

sad::resource::FileCreator::~FileCreator()
{

}

sad::resource::Factory::Factory()
{
    add(sad::Texture::globalMetaData()->name(), new resource::CreatorFor<sad::Texture>());
    add(sad::TextureMappedFont::globalMetaData()->name(), 
        new resource::CreatorFor<sad::TextureMappedFont>());
    add(sad::Sprite2D::Options::globalMetaData()->name(), 
        new resource::CreatorFor<sad::Sprite2D::Options>());

    m_factory = new sad::db::StoredPropertyFactory();
}

sad::resource::Factory::~Factory()
{
    delete m_factory;
}

void sad::resource::Factory::add(const sad::String & name, resource::Creator * c)
{
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
    sad::resource::Resource* result = nullptr;
    if (m_creators.contains(name))
    {
        resource::Creator * c = m_creators[name];
        result = c->create();	
    }
    return result;	
}

sad::resource::ResourceFile * sad::resource::Factory::fileByType(const sad::String & typehint)
{
    if (typehint == "sad::resource::TextureAtlasFile")
    {
        return new sad::resource::TextureAtlasFile();
    }
    if (typehint == "sad::animations::File")
    {
        return new sad::animations::File();
    }
    if (typehint == "sad::db::custom::SchemaFile")
    {
        sad::db::custom::SchemaFile* file = new sad::db::custom::SchemaFile();
        file->setFactory(m_factory);
        return file;
    }
    if (typehint == "sad::Texture" 
        || typehint == "sad::TextureMappedFont"
        || typehint == "sad::freetype::Font")
    {
        return new sad::resource::ResourceFile();
    }
    if (m_file_creators.contains(typehint))
    {
        return m_file_creators[typehint]->create();
    }
    return nullptr;
}

void sad::resource::Factory::setStoredPropertyFactory(sad::db::StoredPropertyFactory * factory)
{
    assert(m_factory);
    delete m_factory;
    m_factory = factory;
}

sad::db::StoredPropertyFactory* sad::resource::Factory::storedPropertyFactory() const
{
    return m_factory;
}

