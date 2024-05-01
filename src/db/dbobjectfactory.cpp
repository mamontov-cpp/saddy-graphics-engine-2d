#include "db/dbobjectfactory.h"

#include "sadscopedlock.h"

#include "3rdparty/picojson/getproperty.h"
#include "3rdparty/picojson/valuetotype.h"

#include "label.h"
#include "sprite2d.h"
#include "sprite3d.h"
#include "scene.h"
#include "db/custom/customobject.h"

#include "way.h"

#include "dialogue/dialogue.h"

#include "animations/animationsinstance.h"
#include "animations/animationswayinstance.h"
#include "animations/animationsgroup.h"
#include "animations/animationsblinking.h"
#include "animations/animationscamerarotation.h"
#include "animations/animationscamerashaking.h"
#include "animations/animationscolor.h"
#include "animations/animationsfontlist.h"
#include "animations/animationsfontsize.h"
#include "animations/animationsoptionlist.h"
#include "animations/animationsresize.h"
#include "animations/animationsrotate.h"
#include "animations/animationsparallel.h"
#include "animations/animationssequential.h"
#include "animations/animationssimplemovement.h"
#include "animations/animationstyping.h"
#include "animations/animationswaymoving.h"
#include "animations/animationstexturecoordinatescontinuous.h"
#include "animations/animationstexturecoordinateslist.h"

#include "layouts/grid.h"
#include "opticksupport.h"

// =========================================== sad::db::ObjectFactory::AbstractDelegate PUBLIC METHODS ===========================================

sad::db::ObjectFactory::AbstractDelegate::~AbstractDelegate() = default;

// =========================================== PUBLIC METHODS ===========================================

sad::db::ObjectFactory::ObjectFactory()
{
    PROFILER_EVENT;
    initWithDefaultCallbacks();
}

sad::db::ObjectFactory::ObjectFactory(bool empty)
{
    PROFILER_EVENT;
    if (!empty)
    {
        initWithDefaultCallbacks();        
    }
}

void sad::db::ObjectFactory::add(
    const sad::String & name,
    sad::db::schema::Schema* schema, 
    bool own, 
    sad::db::ObjectFactory::AbstractDelegate * d
)
{	
    sad::ScopedLock lock(&m_lock);
    if (m_metadata_container.contains(name) == false && d)
    {
        m_metadata_container.insert(name, new sad::db::ObjectFactory::Entry(own, schema, d) );
    }
}

sad::db::schema::Schema* sad::db::ObjectFactory::schema(const sad::String& name)
{
    PROFILER_EVENT;
    sad::ScopedLock lock(&m_lock);
    sad::db::schema::Schema* result = nullptr;
    if (m_metadata_container.contains(name))
    {
        result = m_metadata_container[name]->Schema;
    }
    return result;
}

sad::db::Object* sad::db::ObjectFactory::create(const sad::String& name)
{
    PROFILER_EVENT;
    sad::ScopedLock lock(&m_lock);
    sad::db::Object* result = nullptr;
    if (m_metadata_container.contains(name))
    {
        result = m_metadata_container[name]->Delegate->create();
    }
    return result;
}

sad::db::Object* sad::db::ObjectFactory::createFromEntry(const picojson::value & v)
{
    PROFILER_EVENT;
    sad::ScopedLock lock(&m_lock);
    const picojson::value * type = picojson::get_property(v, "type");
    const picojson::value * name = picojson::get_property(v, "name");
    sad::db::Object*  result = nullptr;
    if (type)
    {
        sad::Maybe<sad::String> maybetype = picojson::ValueToType<sad::String>::get(*type);
        sad::Maybe<sad::String> maybename;
        if (name)
        {
            maybename = picojson::ValueToType<sad::String>::get(*name);
        }
        if (maybetype.exists())
        {
            if (maybename.exists() 
                && maybetype.value() == "sad::db::custom::Object" 
               )
            {
                if (m_special_custom_handlers.contains(maybename.value()))
                {
                    result =  m_special_custom_handlers[maybename.value()]->create();
                }
            }
            if (result == nullptr)
            {
                result = this->create(maybetype.value());
            }
        }
    }
    return result;
}

sad::db::ObjectFactory::~ObjectFactory()
{
    PROFILER_EVENT;
    
}

// =========================================== PROTECTED METHODS ===========================================

void sad::db::ObjectFactory::initWithDefaultCallbacks()
{
    PROFILER_EVENT;
    // A scene objects
    add<sad::Label>("sad::Label", sad::Label::basicSchema(), false);
    add<sad::Scene>("sad::Scene", sad::Scene::basicSchema(), false);
    add<sad::Sprite2D>("sad::Sprite2D", sad::Sprite2D::basicSchema(), false);
    add<sad::Sprite3D>("sad::Sprite3D", sad::Sprite3D::basicSchema(), false);

    // Custom object has no schema at all
    add<sad::db::custom::Object>("sad::db::custom::Object", nullptr, false);

    add<sad::Way>("sad::Way", sad::Way::basicSchema(), false);

    add<sad::dialogue::Dialogue>("sad::dialogue::Dialogue", sad::dialogue::Dialogue::basicSchema(), false);

    // Animation instance primitives
    add<sad::animations::Instance>("sad::animations::Instance", sad::animations::Instance::basicSchema(), false);
    add<sad::animations::WayInstance>("sad::animations::WayInstance", sad::animations::WayInstance::basicSchema(), false);
    add<sad::animations::Group>("sad::animations::Group", sad::animations::Group::basicSchema(), false);

    // A types of animations
    add<sad::animations::Blinking>("sad::animations::Blinking", sad::animations::Blinking::basicSchema(), false);
    add<sad::animations::CameraRotation>("sad::animations::CameraRotation", sad::animations::CameraRotation::basicSchema(), false);
    add<sad::animations::CameraShaking>("sad::animations::CameraShaking", sad::animations::CameraShaking::basicSchema(), false);
    add<sad::animations::Color>("sad::animations::Color", sad::animations::Color::basicSchema(), false);
    add<sad::animations::FontList>("sad::animations::FontList", sad::animations::FontList::basicSchema(), false);
    add<sad::animations::FontSize>("sad::animations::FontSize", sad::animations::FontSize::basicSchema(), false);
    add<sad::animations::OptionList>("sad::animations::OptionList", sad::animations::OptionList::basicSchema(), false);
    add<sad::animations::Parallel>("sad::animations::Parallel", sad::animations::Parallel::basicSchema(), false);
    add<sad::animations::Resize>("sad::animations::Resize", sad::animations::Resize::basicSchema(), false);
    add<sad::animations::Rotate>("sad::animations::Rotate", sad::animations::Rotate::basicSchema(), false);
    add<sad::animations::SimpleMovement>("sad::animations::SimpleMovement", sad::animations::SimpleMovement::basicSchema(), false);
    add<sad::animations::Sequential>("sad::animations::Sequential", sad::animations::Sequential::basicSchema(), false);
    add<sad::animations::TextureCoordinatesContinuous>("sad::animations::TextureCoordinatesContinuous", sad::animations::TextureCoordinatesContinuous::basicSchema(), false);
    add<sad::animations::TextureCoordinatesList>("sad::animations::TextureCoordinatesList", sad::animations::TextureCoordinatesList::basicSchema(), false);
    add<sad::animations::Typing>("sad::animations::Typing", sad::animations::Typing::basicSchema(), false);
    add<sad::animations::WayMoving>("sad::animations::WayMoving", sad::animations::WayMoving::basicSchema(), false);
    
    // A layout schema
    add<sad::layouts::Grid>("sad::layouts::Grid", sad::layouts::Grid::basicSchema(), false);
}

// =========================================== sad::db::ObjectFactory::Entry PUBLIC METHODS ===========================================

sad::db::ObjectFactory::Entry::~Entry()
{
    PROFILER_EVENT;
    if (OwnsSchema)
    {
        delete Schema;
    }
    delete Delegate;
}
