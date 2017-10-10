#include "db/dbtypename.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbvariant.h"

#include "sadstring.h"
#include "sadpoint.h"
#include "sadrect.h"
#include "sadcolor.h"
#include "sadsize.h"
#include "object.h"
#include "sadcolor.h"
#include "sadrect.h"
#include "sadvector.h"
#include "sadptrvector.h"
#include "sadhash.h"
#include "sadmutex.h"
#include "font.h"
#include "texturemappedfont.h"
#include "scenenode.h"
#include "label.h"
#include "formattedlabel.h"
#include "sprite2d.h"
#include "sprite3d.h"
#include "scene.h"
#include "timer.h"
#include "sadthread.h"

#include "dialogue/dialogue.h"
#include "dialogue/phrase.h"

#include "p2d/app/way.h"
#include "p2d/bounds.h"
#include "p2d/body.h"
#include "p2d/line.h"
#include "p2d/rectangle.h"
#include "p2d/circle.h"
#include "p2d/infiniteline.h"
#include "p2d/movement.h"
#include "p2d/weight.h"
#include "p2d/collisionevent.h"
#include "p2d/collisionhandler.h"
#include "p2d/collisiontest.h"

#include "animations/animationsgroup.h"
#include "animations/animationsfactory.h"
#include "animations/animationsanimations.h"

#include "animations/easing/easingfunction.h"

#include "layouts/lengthvalue.h"
#include "layouts/horizontalalignment.h"
#include "layouts/verticalalignment.h"
#include "layouts/stackingtype.h"
#include "layouts/serializablecell.h"
#include "layouts/cell.h"
#include "layouts/grid.h"

#include "clipboard.h"

static sad::Mutex typename_cache_lock;
static sad::Hash<sad::String, sad::String> typename_cache;

const sad::String&  sad::db::internal::fetchTypeNameForPointer(const sad::String& base_name)
{
    typename_cache_lock.lock();
    if (typename_cache.contains(base_name) == false)
    {
        typename_cache.insert(base_name, base_name + " *");
    }
    const sad::String& result =  typename_cache[base_name];
    typename_cache_lock.unlock();
    return result;
}

static sad::Mutex templates_dependent_on_1_args_lock;
static sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > templates_dependent_on_1_args_cache;

const sad::String&  sad::db::internal::fetchTypeNameForTemplate1(
    const sad::String& type_name,
    const sad::String& arg1
)
{
    templates_dependent_on_1_args_lock.lock();
    if (templates_dependent_on_1_args_cache.contains(type_name) == false)
    {
        templates_dependent_on_1_args_cache.insert(type_name, sad::Hash<sad::String, sad::String>());
    }
    sad::Hash<sad::String, sad::String>& slice1 = templates_dependent_on_1_args_cache[type_name];
    if (slice1.contains(arg1) == false)
    {
        slice1.insert(arg1, type_name + "<" + arg1 + ">");
    }
    const sad::String& result =  slice1[arg1];
    templates_dependent_on_1_args_lock.unlock();
    return result;
}

static sad::Mutex templates_dependent_on_2_args_lock;
static sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > > templates_dependent_on_2_args_cache;

const sad::String&  sad::db::internal::fetchTypeNameForTemplate2(
    const sad::String& type_name,
    const sad::String& arg1,
    const sad::String& arg2
)
{
    templates_dependent_on_2_args_lock.lock();
    if (templates_dependent_on_2_args_cache.contains(type_name) == false)
    {
        templates_dependent_on_2_args_cache.insert(type_name, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >& slice1 = templates_dependent_on_2_args_cache[type_name];
    if (slice1.contains(arg1) == false)
    {
        slice1.insert(arg1, sad::Hash<sad::String, sad::String>());
    }
    sad::Hash<sad::String, sad::String>& slice2 = slice1[arg1];
    if (slice2.contains(arg2) == false)
    {
        slice2.insert(arg2, type_name + "<" + arg1 + "," + arg2 + ">");
    }
    const sad::String& result =  slice2[arg2];
    templates_dependent_on_2_args_lock.unlock();
    return result;
}

static sad::Mutex templates_dependent_on_3_args_lock;
static sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > > > templates_dependent_on_3_args_cache;


const sad::String&  sad::db::internal::fetchTypeNameForTemplate3(
    const sad::String& type_name,
    const sad::String& arg1,
    const sad::String& arg2,
    const sad::String& arg3
)
{
    templates_dependent_on_3_args_lock.lock();
    if (templates_dependent_on_3_args_cache.contains(type_name) == false)
    {
        templates_dependent_on_3_args_cache.insert(type_name, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > >& slice1 = templates_dependent_on_3_args_cache[type_name];
    if (slice1.contains(arg1) == false)
    {
        slice1.insert(arg1, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >& slice2 = slice1[arg1];
    if (slice2.contains(arg2) == false)
    {
        slice2.insert(arg2,  sad::Hash<sad::String, sad::String>());
    }
     sad::Hash<sad::String, sad::String>& slice3 = slice2[arg2];
    if (slice3.contains(arg3) == false)
    {
        slice3.insert(arg3, type_name + "<" + arg1 + "," + arg2 + "," + arg3 + ">");
    }
    const sad::String& result =  slice3[arg3];
    templates_dependent_on_3_args_lock.unlock();
    return result;
}

static sad::Mutex templates_dependent_on_4_args_lock;
static sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > > > > templates_dependent_on_4_args_cache;


const sad::String&  sad::db::internal::fetchTypeNameForTemplate4(
    const sad::String& type_name,
    const sad::String& arg1,
    const sad::String& arg2,
    const sad::String& arg3,
    const sad::String& arg4
)
{
    templates_dependent_on_4_args_lock.lock();
    if (templates_dependent_on_4_args_cache.contains(type_name) == false)
    {
        templates_dependent_on_4_args_cache.insert(type_name, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > > >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > > >& slice1 = templates_dependent_on_4_args_cache[type_name];
    if (slice1.contains(arg1) == false)
    {
        slice1.insert(arg1, sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::Hash<sad::String, sad::String> > >& slice2 = slice1[arg1];
    if (slice2.contains(arg2) == false)
    {
        slice2.insert(arg2,   sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >());
    }
    sad::Hash<sad::String, sad::Hash<sad::String, sad::String> >& slice3 = slice2[arg2];
    if (slice3.contains(arg3) == false)
    {
        slice3.insert(arg3, sad::Hash<sad::String, sad::String>());
    }
    sad::Hash<sad::String, sad::String>& slice4 = slice3[arg3];
    if (slice4.contains(arg4) == false)
    {
        slice4.insert(arg4, type_name + "<" + arg1 + "," + arg2 + "," + arg3 + "," + arg4 + ">");
    }
    const sad::String& result =  slice4[arg4];
    templates_dependent_on_4_args_lock.unlock();
    return result;
}

DECLARE_COMMON_TYPE(bool)
DECLARE_COMMON_TYPE(char)
DECLARE_COMMON_TYPE(const char)
DECLARE_COMMON_TYPE(signed char)
DECLARE_COMMON_TYPE(unsigned char)
DECLARE_COMMON_TYPE(short)
DECLARE_COMMON_TYPE(unsigned short)
DECLARE_COMMON_TYPE(int)
DECLARE_COMMON_TYPE(unsigned int)
DECLARE_COMMON_TYPE(long)
DECLARE_COMMON_TYPE(unsigned long)
DECLARE_COMMON_TYPE(long long)
DECLARE_COMMON_TYPE(unsigned long long)
DECLARE_COMMON_TYPE(float)
DECLARE_COMMON_TYPE(double)
DECLARE_COMMON_TYPE(long double)
DECLARE_COMMON_TYPE(sad::Clipboard)
DECLARE_COMMON_TYPE(std::string)
DECLARE_COMMON_TYPE(sad::String)
DECLARE_COMMON_TYPE(sad::Point2D)
DECLARE_COMMON_TYPE(sad::Point2I)
DECLARE_COMMON_TYPE(sad::Point3D)
DECLARE_COMMON_TYPE(sad::Point3I)
DECLARE_COMMON_TYPE(sad::Rect<sad::Point3D>)
DECLARE_COMMON_TYPE(sad::Rect2D)
DECLARE_COMMON_TYPE(sad::Rect2I)
DECLARE_COMMON_TYPE(sad::Color)
DECLARE_COMMON_TYPE(sad::AColor)
DECLARE_COMMON_TYPE(sad::Size2D)
DECLARE_COMMON_TYPE(sad::Size2I)
DECLARE_COMMON_TYPE(sad::Thread)
DECLARE_COMMON_TYPE(sad::Timer)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::Vector<sad::AColor> >)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::Point2D>)
DECLARE_COMMON_TYPE(sad::dialogue::Phrase)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::dialogue::Phrase*>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::dialogue::Phrase>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<unsigned long long>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::String>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::p2d::Body*>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::p2d::BasicCollisionHandler*>)
DECLARE_INSTANTIATED_COMMON_TYPE(sad::Vector<sad::layouts::SerializableCell>)

DECLARE_COMMON_TYPE(sad::p2d::Bound)
DECLARE_COMMON_TYPE(sad::p2d::Line)
DECLARE_COMMON_TYPE(sad::p2d::Rectangle)
DECLARE_COMMON_TYPE(sad::p2d::Circle)
DECLARE_COMMON_TYPE(sad::p2d::InfiniteLine)
DECLARE_COMMON_TYPE(sad::p2d::CollisionShape)
DECLARE_COMMON_TYPE(sad::p2d::AbstractMovementDeltaListener<sad::p2d::Vector>)
DECLARE_COMMON_TYPE(sad::p2d::AbstractMovementDeltaListener<double>)
DECLARE_COMMON_TYPE(sad::p2d::Weight)
DECLARE_COMMON_TYPE(sad::p2d::BasicCollisionEvent)
DECLARE_COMMON_TYPE(sad::p2d::BasicCollisionHandler)
DECLARE_COMMON_TYPE(sad::p2d::CollisionTest)

DECLARE_COMMON_TYPE(sad::animations::Factory)
DECLARE_COMMON_TYPE(sad::animations::Animations)

DECLARE_COMMON_TYPE(sad::layouts::LengthValue)
DECLARE_COMMON_TYPE(sad::layouts::HorizontalAlignment)
DECLARE_COMMON_TYPE(sad::layouts::VerticalAlignment)
DECLARE_COMMON_TYPE(sad::layouts::StackingType)
DECLARE_COMMON_TYPE(sad::layouts::Unit)
DECLARE_COMMON_TYPE(sad::layouts::Cell)


DECLARE_TYPE_AS_SAD_OBJECT(sad::db::Object)
DECLARE_TYPE_AS_SAD_OBJECT(sad::Object)
DECLARE_TYPE_AS_SAD_OBJECT(sad::Scene)
DECLARE_TYPE_AS_SAD_OBJECT(sad::p2d::app::Way)
DECLARE_TYPE_AS_SAD_OBJECT(sad::dialogue::Dialogue)

DECLARE_COMMON_TYPE(sad::db::Variant)


DECLARE_COMMON_TYPE(sad::Maybe<sad::String>)
