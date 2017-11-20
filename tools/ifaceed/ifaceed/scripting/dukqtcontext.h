/*! \file dukqtcontext.h
    
    A helper for qt context
 */
#pragma once
#include <dukqt.h>
#include <curry1.h>
#include <object.h>
#include <thislambda.h>
#include <lambda.h>

#include <QString>
#include <QStringList>
#include <sadvector.h>
#include <sadstring.h>
#include <sadpoint.h>
#include <sadrect.h>
#include <sadcolor.h>
#include <scene.h>
#include <scenenode.h>
#include <layouts/grid.h>
#include <p2d/app/way.h>
#include <dialogue/dialogue.h>
#include <animations/animationsanimation.h>
#include <animations/animationscomposite.h>
#include <animations/animationsblinking.h>
#include <animations/animationscamerashaking.h>
#include <animations/animationscamerarotation.h>
#include <animations/animationscolor.h>
#include <animations/animationsfontlist.h>
#include <animations/animationsfontsize.h>
#include <animations/animationsresize.h>
#include <animations/animationsrotate.h>
#include <animations/animationsoptionlist.h>
#include <animations/animationstexturecoordinateslist.h>
#include <animations/animationstexturecoordinatescontinuous.h>
#include <animations/animationssimplemovement.h>
#include <animations/animationswaymoving.h>
#include <animations/animationsinstance.h>
#include <animations/animationswayinstance.h>
#include <animations/animationsgroup.h>

namespace dukpp03
{

namespace qt
{
/*! Makes lambda, that could be registered as method
 */
typedef dukpp03::bind_lambda<dukpp03::qt::BasicContext> bind_lambda;
/*! Makes lambda, that could be registered as function
 */
typedef dukpp03::make_lambda<dukpp03::qt::BasicContext> make_lambda;
/*! A lambda with curried first argument
 */
typedef dukpp03::curried1<dukpp03::qt::BasicContext> curried1;

}

}

/*! Makes binding types easier by declaring  meta types for T, T* and T** respectively
 */
#define Q_DUKPP03_DECLARE_METATYPE(T) Q_DECLARE_METATYPE(T) Q_DECLARE_METATYPE(T*) Q_DECLARE_METATYPE(T**)

Q_DECLARE_METATYPE(QString*)
Q_DECLARE_METATYPE(QStringList*)
Q_DUKPP03_DECLARE_METATYPE(sad::db::Object*)
Q_DUKPP03_DECLARE_METATYPE(sad::Scene*)
Q_DUKPP03_DECLARE_METATYPE(sad::SceneNode*)
Q_DUKPP03_DECLARE_METATYPE(sad::layouts::Grid*)
Q_DUKPP03_DECLARE_METATYPE(sad::p2d::app::Way*)
Q_DUKPP03_DECLARE_METATYPE(sad::dialogue::Dialogue*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Animation*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Composite*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Blinking*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::CameraShaking*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::CameraRotation*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Color*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::FontList*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::FontSize*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Resize*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Rotate*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::OptionList*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::TextureCoordinatesList*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::SimpleMovement*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::TextureCoordinatesContinuous*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::WayMoving*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::Instance*)
Q_DUKPP03_DECLARE_METATYPE(sad::animations::WayInstance*)
Q_DUKPP03_DECLARE_METATYPE(sad::String)
Q_DUKPP03_DECLARE_METATYPE(sad::Vector<sad::String>)
Q_DUKPP03_DECLARE_METATYPE(sad::Point2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Point2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Point3D)
Q_DUKPP03_DECLARE_METATYPE(sad::Point3I)
Q_DUKPP03_DECLARE_METATYPE(sad::Size2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Size2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Rect2D)
Q_DUKPP03_DECLARE_METATYPE(sad::Rect2I)
Q_DUKPP03_DECLARE_METATYPE(sad::Color)
Q_DUKPP03_DECLARE_METATYPE(sad::AColor)


/*! A typedef for currying first part
 */
typedef dukpp03::curried1<dukpp03::qt::BasicContext> curried1;


namespace dukpp03
{

/*! Declared get value instantiation for TYPE
 */
#define DEFINE_GET_VALUE_INSTANTIATION( TYPE )      \
template<>                                          \
class GetValue< TYPE , dukpp03::qt::BasicContext>   \
{                                                   \
public:                                             \
    static dukpp03::Maybe< TYPE > perform(          \
        dukpp03::qt::BasicContext* ctx,             \
        duk_idx_t pos                               \
    );                                              \
};

DEFINE_GET_VALUE_INSTANTIATION(sad::String)
DEFINE_GET_VALUE_INSTANTIATION(QStringList)
DEFINE_GET_VALUE_INSTANTIATION(sad::Vector<sad::String> )
DEFINE_GET_VALUE_INSTANTIATION(sad::db::Object* )
DEFINE_GET_VALUE_INSTANTIATION(sad::Scene* )
DEFINE_GET_VALUE_INSTANTIATION(sad::SceneNode* )
DEFINE_GET_VALUE_INSTANTIATION(sad::layouts::Grid*)
DEFINE_GET_VALUE_INSTANTIATION(sad::p2d::app::Way*)
DEFINE_GET_VALUE_INSTANTIATION(sad::dialogue::Dialogue*)
DEFINE_GET_VALUE_INSTANTIATION(sad::dialogue::Phrase)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Animation*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Composite*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Blinking*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::CameraShaking*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::CameraRotation*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Color*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::FontList*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::FontSize*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Resize*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Rotate*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::OptionList*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::TextureCoordinatesList*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::SimpleMovement*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::TextureCoordinatesContinuous*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::WayMoving*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Instance*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::WayInstance*)
DEFINE_GET_VALUE_INSTANTIATION(sad::animations::Group*)

#undef DEFINE_GET_VALUE_INSTANTIATION

/*! An instantiation for pushing sad::String on stack
 */
template<>
class PushValue<sad::String, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::String& v);
};

/*! An instantiation for pushing QStringList on stack
 */
template<>
class PushValue<QStringList, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
        \param[in] ctx context
        \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const QStringList& v);
};

/*! An instantiation for pushing sad::Vector<sad::String> on stack
 */
template<>
class PushValue<sad::Vector<sad::String>, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::Vector<sad::String>& v);
};

/*! An instantiation for pushing sad::Vector<unsigned long long> on stack
*/
template<>
class PushValue<sad::Vector<unsigned long long>, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::Vector<unsigned long long>& v);
};



/*! An instantiation for pushing sad::dialogue::Phrase on stack
*/
template<>
class PushValue<sad::dialogue::Phrase, dukpp03::qt::BasicContext>
{
public:
    /*! Performs pushing value
    \param[in] ctx context
    \param[in] v value
    */
    static void perform(dukpp03::qt::BasicContext* ctx, const sad::dialogue::Phrase& v);
};


}
