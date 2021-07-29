#include "animations/animationstexturecoordinateslist.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"

#include "sprite2d.h"
#include "sadmutex.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"
#include "db/dbdatabase.h"


#include <sprite2d.h>

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>

DECLARE_SOBJ_INHERITANCE(sad::animations::TextureCoordinatesList, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::TextureCoordinatesList::TextureCoordinatesList() 
: m_cache_folder(nullptr), 
m_cache_root_folder(nullptr),
m_renderer(nullptr)
{
    m_creators.pushProperty<sad::Rect2D>("texturecoordinates", "texturecoordinates");
}

sad::animations::TextureCoordinatesList::~TextureCoordinatesList()
{
    
}

static sad::db::schema::Schema* AnimationTextureCoordinatesListSchema = nullptr;

static sad::Mutex AnimationTextureCoordinatesListSchemaInit;
sad::db::schema::Schema* sad::animations::TextureCoordinatesList::basicSchema()
{
    if (AnimationTextureCoordinatesListSchema == nullptr)
    {
        AnimationTextureCoordinatesListSchemaInit.lock();
        if (AnimationTextureCoordinatesListSchema == nullptr)
        {
            AnimationTextureCoordinatesListSchema = new sad::db::schema::Schema();
            AnimationTextureCoordinatesListSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationTextureCoordinatesListSchema->add(
                "list",
                new sad::db::MethodPair<sad::animations::TextureCoordinatesList, sad::Vector<sad::String> >(
                    &sad::animations::TextureCoordinatesList::list,
                    &sad::animations::TextureCoordinatesList::setList
                )
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationTextureCoordinatesListSchema);
        }
        AnimationTextureCoordinatesListSchemaInit.unlock();
    }
    return AnimationTextureCoordinatesListSchema;
}

sad::db::schema::Schema* sad::animations::TextureCoordinatesList::schema() const
{
    return sad::animations::TextureCoordinatesList::basicSchema();
}

void sad::animations::TextureCoordinatesList::setTreeName(
    sad::Renderer* renderer,
    const sad::String& tree_name
)
{
    m_renderer = renderer;
    m_tree_name = tree_name;
}

bool sad::animations::TextureCoordinatesList::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<sad::Vector<sad::String> > list = picojson::to_type<sad::Vector<sad::String> >(
                                                        picojson::get_property(v, "list")
                                                      );
        bool result = list.exists();
        if (result)
        {
            setList(list.value());			
        }

        flag = flag && result;
    }
    return flag;
}

void sad::animations::TextureCoordinatesList::setList(const sad::Vector<sad::String>& list)
{
    m_list = list;
    m_inner_valid = m_list.size() != 0;
    this->updateValidFlag();
}

const sad::Vector<sad::String> & sad::animations::TextureCoordinatesList::list() const
{
    return m_list;
}


void sad::animations::TextureCoordinatesList::setState(sad::animations::Instance* i, double time)
{
    double time_position = m_easing->evalBounded(time, m_time);
    double value = static_cast<double>(m_list.size()) * time_position;
    unsigned int kvalue = static_cast<unsigned int>(value);
    if (kvalue < m_list.size())
    {
        sad::Rect2D* r = this->coordinates(m_list[kvalue]);
        if (r)
        {
            i->stateCommandAs<sad::Rect2D>()->call(*r);
        }
    }
}


sad::animations::setstate::AbstractSetStateCommand* sad::animations::TextureCoordinatesList::stateCommand(sad::db::Object* o)
{
    if (this->applicableTo(o))
    {
        sad::animations::setstate::AbstractSetStateCommand* c;
        if (o->isInstanceOf("sad::Sprite2D"))
        {
            c = sad::animations::setstate::make(
                    o,
                     &sad::Sprite2D::setTextureCoordinates
                );
        }
        else
        {
            if (o->isInstanceOf("sad::db::custom::Object"))
            {
                c = sad::animations::setstate::make(
                        o,
                        &sad::db::custom::Object::setTextureCoordinates
                    );
            }
            else
            {
                c = new sad::animations::setstate::SetProperty<sad::Rect2D>(o, "texturecoordinates");
            }
        }
        return c;
    }
    return new sad::animations::setstate::DummyCommand<sad::Rect2D>();
}

bool sad::animations::TextureCoordinatesList::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::Rect2D>("texturecoordinates").exists();
    }
    return result;
}

// =============================== PROTECTED METHODS ==========================

sad::Rect2D* sad::animations::TextureCoordinatesList::coordinates(const sad::String& c)
{
    if (m_folder == nullptr)
    {
        if (m_table && m_renderer == nullptr)
        {
            if (m_table->database())
            {
                m_renderer = m_table->database()->renderer();
            }
        }
        if (m_renderer != nullptr)
        {
            m_folder = m_renderer->tree(m_tree_name)->root();
        }
        if (m_folder == nullptr)
        {
            return nullptr;
        }
    }

    if (m_folder != m_cache_folder)
    {
        m_cache.clear();
        m_cache_folder = m_folder;
        m_cache_root_folder = m_folder;
        while(m_cache_root_folder->parent() != nullptr)
        {
            m_cache_root_folder = m_cache_root_folder->parent();
        }
    }

    sad::Rect2D* result;
    if (m_cache.contains(c))
    {
        result = &(m_cache[c]);
    }
    else
    {
        result = nullptr;
        sad::resource::Resource* r = m_cache_root_folder->resource(c);
        if (r)
        {
            if (r->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
            {
                sad::Sprite2D::Options* opts = static_cast<sad::Sprite2D::Options*>(r);

                m_cache.insert(c, opts->TextureRectangle);
                result = &(m_cache[c]);
            }
        }
    }
    return result;
}
