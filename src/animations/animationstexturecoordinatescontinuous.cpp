#include "animations/animationstexturecoordinatescontinuous.h"
#include "animations/animationsinstance.h"

#include "animations/easing/easingfunction.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"

#include "sprite2d.h"
#include "db/custom/customobject.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <sprite2d.h>

#include <util/fs.h>

#include <resource/resourcefile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>
#include "sadmutex.h"

DECLARE_SOBJ_INHERITANCE(sad::animations::TextureCoordinatesContinuous, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::TextureCoordinatesContinuous::TextureCoordinatesContinuous() 
{
    m_creators.pushProperty<sad::Rect2D>("texturecoordinates", "texturecoordinates");	
}

sad::animations::TextureCoordinatesContinuous::~TextureCoordinatesContinuous()
{
    
}

static sad::db::schema::Schema* AnimationTextureCoordinatesContinuousSchema = nullptr;

static sad::Mutex AnimationTextureCoordinatesContinuousSchemaInit;

sad::db::schema::Schema* sad::animations::TextureCoordinatesContinuous::basicSchema()
{
    if (AnimationTextureCoordinatesContinuousSchema == nullptr)
    {
        AnimationTextureCoordinatesContinuousSchemaInit.lock();
        if (AnimationTextureCoordinatesContinuousSchema == nullptr)
        {
            AnimationTextureCoordinatesContinuousSchema = new sad::db::schema::Schema();
            AnimationTextureCoordinatesContinuousSchema->addParent(sad::animations::Animation::basicSchema());

            AnimationTextureCoordinatesContinuousSchema->add(
                "start_rect",
                new sad::db::MethodPair<sad::animations::TextureCoordinatesContinuous, sad::Rect2D>(
                    &sad::animations::TextureCoordinatesContinuous::startRect,
                    &sad::animations::TextureCoordinatesContinuous::setStartRect
                )
            );
            AnimationTextureCoordinatesContinuousSchema->add(
                "end_rect",
                new sad::db::MethodPair<sad::animations::TextureCoordinatesContinuous, sad::Rect2D>(
                    &sad::animations::TextureCoordinatesContinuous::endRect,
                    &sad::animations::TextureCoordinatesContinuous::setEndRect
                )
            );
                
            sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationTextureCoordinatesContinuousSchema);
        }
        AnimationTextureCoordinatesContinuousSchemaInit.unlock();
    }
    return AnimationTextureCoordinatesContinuousSchema;
}

sad::db::schema::Schema* sad::animations::TextureCoordinatesContinuous::schema() const
{
    return sad::animations::TextureCoordinatesContinuous::basicSchema();
}

bool sad::animations::TextureCoordinatesContinuous::loadFromValue(const picojson::value& v)
{
    bool flag = this->sad::animations::Animation::loadFromValue(v);
    if (flag)
    {
        sad::Maybe<sad::Rect2D> startrect = picojson::to_type<sad::Rect2D>(
                                                picojson::get_property(v, "start_rect")
                                            );
        sad::Maybe<sad::Rect2D> endrect = picojson::to_type<sad::Rect2D>(
                                                picojson::get_property(v, "end_rect")
                                            );
        bool result = startrect.exists() && endrect.exists();
        if (result)
        {
            setStartRect(startrect.value());
            setEndRect(endrect.value());
        }

        flag = flag && result;
    }
    return flag;
}

void sad::animations::TextureCoordinatesContinuous::setStartRect(const sad::Rect2D& rect)
{
    m_start_rect = rect;
}

const sad::Rect2D& sad::animations::TextureCoordinatesContinuous::startRect() const
{
    return m_start_rect;
}

void sad::animations::TextureCoordinatesContinuous::setEndRect(const sad::Rect2D& rect)
{
    m_end_rect = rect;
}

const sad::Rect2D& sad::animations::TextureCoordinatesContinuous::endRect() const
{
    return m_end_rect;
}


void sad::animations::TextureCoordinatesContinuous::setState(sad::animations::Instance* i, double time)
{
    double time_position = m_easing->eval(time, m_time);
    sad::Rect2D r;
    for(size_t ii = 0; ii < 4; ii++)
    {
        r[ii] = m_start_rect[ii] + (m_end_rect[ii] - m_start_rect[ii]) * time_position;
    }
    i->stateCommandAs<sad::Rect2D>()->call(r);
}


sad::animations::setstate::AbstractSetStateCommand* sad::animations::TextureCoordinatesContinuous::stateCommand(sad::db::Object* o)
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

bool sad::animations::TextureCoordinatesContinuous::applicableTo(sad::db::Object* o)
{
    bool result = false;
    if (o && m_valid)
    {
        result = o->getProperty<sad::Rect2D>("texturecoordinates").exists();
    }
    return result;
}
