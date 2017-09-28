#include "animationsbindings.h"

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../scripting.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationactions.h"

#include "../../history/animations/animationsnew.h"
#include "../../history/animations/animationsswapincomposite.h"

#include "../abstractgetter.h"
#include "../abstractsetter.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiconsoleblock.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../gui/textedit/textedit.h"

#include "../../history/animations/animationschangename.h"
#include "../../history/animations/animationschangetime.h"
#include "../../history/animations/animationschangelooped.h"
#include "../../history/animations/animationschangeeasingfunctiontype.h"
#include "../../history/animations/animationschangeeasingovershootamplitude.h"
#include "../../history/animations/animationschangeeasingperiod.h"
#include "../../history/animations/animationschangeblinkingfrequency.h"
#include "../../history/animations/animationschangecameraoffset.h"
#include "../../history/animations/animationschangeshakingfrequency.h"
#include "../../history/animations/animationschangecameraangle.h"
#include "../../history/animations/animationschangecamerapivot.h"
#include "../../history/animations/animationschangecolorcolor.h"
#include "../../history/animations/animationschangefontlistfonts.h"
#include "../../history/animations/animationschangefontsizesize.h"
#include "../../history/animations/animationschangelist.h"
#include "../../history/animations/animationschangeresizestartingsize.h"
#include "../../history/animations/animationschangeresizeendingsize.h"
#include "../../history/animations/animationschangerotateangle.h"
#include "../../history/animations/animationschangerect.h"
#include "../../history/animations/animationschangepropertyaspoint2displayedintwospinboxes.h"

#include "animationspoint2dsetter.h"
#include "animationssetter.h"
#include "animationswidgetsetter.h"
#include "animationswaysetter.h"
#include "easinggetter.h"
#include "easingsetter.h"


Q_DECLARE_METATYPE(QVector<sad::Point2D>)
Q_DECLARE_METATYPE(sad::Vector<unsigned long long>) //-V566

QVector<unsigned long long> scripting::animations::list()
{
    return scripting::query_table("animations", "sad::animations::Animation");
}


unsigned long long scripting::animations::_add(
    scripting::Scripting* scripting, 
    sad::String type,
    sad::String name,
    double time,
    bool looped
)
{
    core::Editor* e = scripting->editor();
    QString	animationtypename = QString("sad::animations::") + STD2QSTRING(type);

    sad::animations::Animation* a = e->animationFactory()->create(animationtypename.toStdString());
    unsigned long long result  = 0;
    if (a)
    {
        a->setObjectName(name);
        a->setTime(time);
        a->setLooped(looped);

        sad::Renderer::ref()->database("")->table("animations")->add(a);		
        
        history::animations::New* c = new history::animations::New(a);
        c->commit(e);
        e->currentBatchCommand()->add(c);

        result = a->MajorId;
    }
    else
    {
        QString error = QString("_add(): cannot created animation of following type - ") + type.c_str();
        scripting->context()->throwError(error.toStdString());
        throw dukpp03::ArgumentException();
    }
    return result;
}

void scripting::animations::remove(
    scripting::Scripting* scripting, 
    sad::animations::Animation* a
)
{
    scripting->editor()->actions()->animationActions()->removeAnimationFromDatabase(a, false);
}

bool scripting::animations::addToComposite(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    sad::animations::Animation* a
)
{
    return scripting->editor()->actions()->animationActions()->addAnimationToCompositeList(list, a, false);
}

bool scripting::animations::removeFromComposite(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    sad::animations::Animation* a
)
{
    return scripting->editor()->actions()->animationActions()->removeAnimationFromCompositeList(list, a, false, -1);
}

int scripting::animations::compositeLength(
    scripting::Scripting*, 
    sad::animations::Composite* list
)
{
    return list->animationMajorIds().size();
}

unsigned long long scripting::animations::getAnimation(
    scripting::Scripting*, 
    sad::animations::Composite* list,
    unsigned int pos
)
{
    sad::Vector<unsigned long long> v = list->animationMajorIds();
    unsigned long long result = 0;
    if (pos < v.size())
    {
        result = v[pos];
    }
    return result;
}


bool scripting::animations::moveBackInCompositeList(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    unsigned int pos
)
{
    sad::Vector<unsigned long long> v = list->animationMajorIds();
    if (pos < v.count() - 1)
    {
        core::Editor* e = scripting->editor();
        history::Command* c = new history::animations::SwapInComposite(list, pos, pos + 1);
        c->commit(e);
        e->currentBatchCommand()->add(c);
        return true;
    }
    return false;
}

bool scripting::animations::moveFrontInCompositeList(
    scripting::Scripting* scripting, 
    sad::animations::Composite* list,
    unsigned int pos
)
{
    sad::Vector<unsigned long long> v = list->animationMajorIds();
    if (pos > 0 && pos < v.count() - 1)
    {
        core::Editor* e = scripting->editor();
        history::Command* c = new history::animations::SwapInComposite(list, pos - 1, pos);
        c->commit(e);
        e->currentBatchCommand()->add(c);
        return true;
    }
    return false;
}


void scripting::animations::checkPropertiesForAnimations(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool readable
)
{
    if (obj.value()->isInstanceOf("sad::animations::Animation"))
    {
        list << "time";
        list << "looped";
        list << "easing_type";
        list << "easing_overshoot_amplitude";
        list << "easing_period";
    }

    if (obj.value()->isInstanceOf("sad::animations::Blinking"))
    {
        list << "frequency";
    }

    if (obj.value()->isInstanceOf("sad::animations::CameraShaking"))
    {
        list << "offset";
        list << "frequency";
    }

    if (obj.value()->isInstanceOf("sad::animations::CameraRotation"))
    {
        list << "pivot";
        list << "min_angle";
        list << "max_angle";
    }

    if (obj.value()->isInstanceOf("sad::animations::Color"))
    {
        list << "min_color";
        list << "max_color";
    }

    if (obj.value()->isInstanceOf("sad::animations::FontList"))
    {
        list << "fonts";
    }

    if (obj.value()->isInstanceOf("sad::animations::FontSize"))
    {
        list << "min_size";
        list << "max_size";
    }

    if (obj.value()->isInstanceOf("sad::animations::Resize"))
    {
        list << "start_size";
        list << "end_size";
    }

    if (obj.value()->isInstanceOf("sad::animations::SimpleMovement"))
    {
        list << "start_point";
        list << "end_point";
    }

    if (obj.value()->isInstanceOf("sad::animations::Rotate"))
    {
        list << "min_angle";
        list << "max_angle";
    }

    if (obj.value()->isInstanceOf("sad::animations::OptionList") 
        || obj.value()->isInstanceOf("sad::animations::TextureCoordinatesList"))
    {
        list << "list";
    }

    if (obj.value()->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
    {
        list << "start_rect";
        list << "end_rect";
    }

    if (obj.value()->isInstanceOf("sad::animations::WayMoving"))
    {
        list << "way";
    }

    if (readable)
    {
        if (obj.value()->isInstanceOf("sad::animations::Composite"))
        {
            list << "list";
        }	
    }
}

dukpp03::qt::JSObject* scripting::animations::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::JSObject*  animations_value = new dukpp03::qt::JSObject();
    animations_value->setProperty("list", dukpp03::qt::make_function::from(scripting::animations::list)); // E.animations.list
    animations_value->setProperty("_add", dukpp03::qt::curried1::from(s, scripting::animations::_add)); // E.animations._add
    animations_value->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::animations::remove)); // E.animations.remove
    animations_value->setProperty("addToComposite", dukpp03::qt::curried1::from(s, scripting::animations::addToComposite)); // E.animations.addToComposite
    animations_value->setProperty("removeFromComposite", dukpp03::qt::curried1::from(s, scripting::animations::removeFromComposite)); // E.animations.removeFromComposite
    animations_value->setProperty("compositeLength", dukpp03::qt::curried1::from(s, scripting::animations::compositeLength)); // E.animations.compositeLength
    animations_value->setProperty("getAnimation", dukpp03::qt::curried1::from(s, scripting::animations::getAnimation)); // E.animations.getAnimation
    animations_value->setProperty("moveBackInCompositeList", dukpp03::qt::curried1::from(s, scripting::animations::moveBackInCompositeList)); // E.animations.moveBackInCompositeList
    animations_value->setProperty("moveFrontInCompositeList", dukpp03::qt::curried1::from(s, scripting::animations::moveFrontInCompositeList)); // E.animations.moveFrontInCompositeList



    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
    {
        set->add(new scripting::animations::Setter<sad::animations::Animation, sad::String, history::animations::ChangeName>(s, "name"));
        set->add(new scripting::animations::Setter<sad::animations::Animation, double, history::animations::ChangeTime>(s, "time"));
        set->add(new scripting::animations::Setter<sad::animations::Animation, bool, history::animations::ChangeLooped>(s, "looped"));
        set->add(new scripting::animations::Setter<sad::animations::Blinking, unsigned int, history::animations::ChangeBlinkingFrequency>(s, "frequency"));
        set->add(new scripting::animations::Setter<sad::animations::CameraShaking, sad::Point2D, history::animations::ChangeCameraOffset>(s, "offset"));
        set->add(new scripting::animations::Setter<sad::animations::CameraShaking, int, history::animations::ChangeShakingFrequency>(s, "frequency"));
        set->add(new scripting::animations::Setter<sad::animations::CameraRotation, sad::Point3D, history::animations::ChangeCameraPivot>(s, "pivot"));

        gui::uiblocks::UIAnimationBlock* ablk = s->editor()->uiBlocks()->uiAnimationBlock();
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::CameraRotation,
            QDoubleSpinBox*,
            double,
            history::animations::ChangeCameraAngle
         >(s, ablk->dsbCameraRotationStartingAngle, "min_angle")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::CameraRotation,
            QDoubleSpinBox*,
            double,
            history::animations::ChangeCameraAngle
         >(s, ablk->dsbCameraRotationEndingAngle, "max_angle")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::Color,
            gui::colorview::ColorView*,
            sad::AColor,
            history::animations::ChangeColorColor
         >(s, ablk->cwColorStartingColor, "min_color")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::Color,
            gui::colorview::ColorView*,
            sad::AColor,
            history::animations::ChangeColorColor
         >(s, ablk->cwColorEndingColor, "max_color")
        );
        set->add(new scripting::animations::Setter<sad::animations::FontList, sad::Vector<sad::String>, history::animations::ChangeFontListFonts>(s, "fonts"));
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::FontSize,
            QSpinBox*,
            unsigned int,
            history::animations::ChangeFontSizeSize
         >(s, ablk->sbFontSizeStartingSize, "min_size")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::FontSize,
            QSpinBox*,
            unsigned int,
            history::animations::ChangeFontSizeSize
         >(s, ablk->sbFontSizeEndingSize, "max_size")
        );
        set->add(new scripting::animations::Setter<sad::animations::Resize, sad::Point2D, history::animations::ChangeResizeStartingSize>(s, "start_size"));
        set->add(new scripting::animations::Setter<sad::animations::Resize, sad::Point2D, history::animations::ChangeResizeEndingSize>(s, "end_size"));
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::Rotate,
            QDoubleSpinBox*,
            double,
            history::animations::ChangeRotateAngle
         >(s, ablk->dsbRotateStartingAngle, "min_angle")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::Rotate,
            QDoubleSpinBox*,
            double,
            history::animations::ChangeRotateAngle
         >(s, ablk->dsbRotateEndingAngle, "max_angle")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::OptionList,
            QTextEdit*,
            sad::Vector<sad::String>,
            history::animations::ChangeList
         >(s, ablk->txtOptionListList, "list")
        );
        set->add(new scripting::animations::WidgetSetter<
            sad::animations::TextureCoordinatesList,
            QTextEdit*,
            sad::Vector<sad::String>,
            history::animations::ChangeList
         >(s, ablk->txtTextureCoordinatesList, "list")
        );
        set->add(new scripting::animations::Point2DSetter<
            sad::animations::SimpleMovement,
            history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
         >(s, "start_point", ablk->dabSimpleMovementStartingPointX, ablk->dabSimpleMovementStartingPointY)
        );
        set->add(new scripting::animations::Point2DSetter<
            sad::animations::SimpleMovement,
            history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
         >(s, "end_point", ablk->dabSimpleMovementEndingPointX, ablk->dabSimpleMovementEndingPointY)
        );

        set->add(new scripting::animations::EasingSetter<
            unsigned int,
            history::animations::ChangeEasingFunctionType
         >(s, "easing_type", &sad::animations::easing::Function::functionTypeAsUnsignedInt)
        );
        set->add(new scripting::animations::EasingSetter<
            double,
            history::animations::ChangeEasingOvershootAmplitude
         >(s, "easing_overshoot_amplitude", &sad::animations::easing::Function::overshootAmplitude)
        );
        set->add(new scripting::animations::EasingSetter<
            double,
            history::animations::ChangeEasingPeriod
         >(s, "easing_period", &sad::animations::easing::Function::period)
        );

        std::function<dukpp03::Maybe<sad::String>(const sad::Rect2D&)> is_aabb = [](const sad::Rect2D& val) {
            dukpp03::Maybe<sad::String> result;
            if (sad::isAABB(val) == false)
            {
                result.setValue("Rectangle must be axis-aligned");
            }
            return result;
        };

        scripting::animations::WidgetSetter<
            sad::animations::TextureCoordinatesContinuous,
            gui::rectwidget::RectWidget*,
            sad::Rect2D,
            history::animations::ChangeRect
        >* rect1 = new scripting::animations::WidgetSetter<
            sad::animations::TextureCoordinatesContinuous,
            gui::rectwidget::RectWidget*,
            sad::Rect2D,
            history::animations::ChangeRect
        >(s, ablk->rctTCCStartingRect, "start_rect");
        rect1->addCondition(is_aabb);
        set->add(rect1);

        scripting::animations::WidgetSetter<
            sad::animations::TextureCoordinatesContinuous,
            gui::rectwidget::RectWidget*,
            sad::Rect2D,
            history::animations::ChangeRect
        >* rect2 = new scripting::animations::WidgetSetter<
            sad::animations::TextureCoordinatesContinuous,
            gui::rectwidget::RectWidget*,
            sad::Rect2D,
            history::animations::ChangeRect
        >(s, ablk->rctTCCEndingRect, "end_rect");
        rect2->addCondition(is_aabb);
        set->add(rect2);

        set->add(new scripting::animations::WaySetter(s));
    }

    animations_value->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.animations.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        get->add(new scripting::AbstractGetter<sad::animations::Animation*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Animation*, double>("time"));
        get->add(new scripting::AbstractGetter<sad::animations::Animation*, bool>("looped"));
        get->add(new scripting::animations::EasingGetter<unsigned int>("easing_type", &sad::animations::easing::Function::functionTypeAsUnsignedInt));
        get->add(new scripting::animations::EasingGetter<double>("easing_overshoot_amplitude", &sad::animations::easing::Function::overshootAmplitude));
        get->add(new scripting::animations::EasingGetter<double>("easing_period", &sad::animations::easing::Function::period));
        get->add(new scripting::AbstractGetter<sad::animations::Blinking*, unsigned int>("frequency"));
        get->add(new scripting::AbstractGetter<sad::animations::CameraShaking*, sad::Point2D>("offset"));
        get->add(new scripting::AbstractGetter<sad::animations::CameraShaking*, int>("frequency"));
        get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, sad::Point3D>("pivot"));
        get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, double>("min_angle"));
        get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, double>("max_angle"));
        get->add(new scripting::AbstractGetter<sad::animations::Color*, sad::AColor>("min_color"));
        get->add(new scripting::AbstractGetter<sad::animations::Color*, sad::AColor>("max_color"));
        get->add(new scripting::AbstractGetter<sad::animations::FontList*, sad::Vector<sad::String> >("fonts"));
        get->add(new scripting::AbstractGetter<sad::animations::FontSize*, unsigned int >("min_size"));
        get->add(new scripting::AbstractGetter<sad::animations::FontSize*, unsigned int >("max_size"));
        get->add(new scripting::AbstractGetter<sad::animations::Resize*, sad::Point2D >("start_size"));
        get->add(new scripting::AbstractGetter<sad::animations::Resize*, sad::Point2D >("end_size"));
        get->add(new scripting::AbstractGetter<sad::animations::Rotate*, double >("min_angle"));
        get->add(new scripting::AbstractGetter<sad::animations::Rotate*, double >("max_angle"));
        get->add(new scripting::AbstractGetter<sad::animations::SimpleMovement*, sad::Point2D>("start_point"));
        get->add(new scripting::AbstractGetter<sad::animations::SimpleMovement*, sad::Point2D>("end_point"));
        get->add(new scripting::AbstractGetter<sad::animations::OptionList*, sad::Vector<sad::String> >("list"));
        get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesContinuous*, sad::Rect2D >("start_rect"));
        get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesContinuous*, sad::Rect2D >("end_rect"));
        get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesList*, sad::Vector<sad::String> >("list"));
        get->add(new scripting::AbstractGetter<sad::animations::WayMoving*, unsigned long long >("way"));
        get->add(new scripting::AbstractGetter<sad::animations::Composite*, sad::Vector<unsigned long long> >("list"));
    }
    animations_value->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.animations.get

    e->setProperty("animations", animations_value);

    QString templateanimationadd(
        "E.animations.add{CLASSNAME} = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"time\" in o == false)    "
        "   {                              "
        "      o[\"time\"] = 0;            "
        "   }                              "
        "   if (\"looped\" in o == false)  "
        "   {                              "
        "      o[\"looped\"] = false;      "
        "   }                              "
        "   return E.animations._add(\"{CLASSNAME}\", o[\"name\"], o[\"time\"], o[\"looped\"]);"
        "};"
    );

    const char* names[] = {
        "Blinking",
        "CameraRotation",
        "CameraShaking",
        "Color",
        "FontList",
        "FontSize",
        "OptionList",
        "Parallel",
        "Resize",
        "Rotate",
        "SimpleMovement",
        "Sequential",
        "TextureCoordinatesList",
        "TextureCoordinatesContinuous",
        "Typing",
        "WayMoving",
        NULL
    };
    int i = 0;
    while (names[i] != NULL)
    {
        QString kadd = templateanimationadd;
        kadd.replace("{CLASSNAME}", names[i]);
        bool b  = s->context()->eval(kadd.toStdString());
        assert(b);
        ++i;
    }

    bool b  = s->context()->eval(
        "E.animations.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.animations.set(arguments[0], arguments[1], arguments[2]); return E.animations;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert( b );

     b = s->context()->eval(
        "E.animations.easingsToNames = ["
        "\"Linear\","
        "\"InSine\","
        "\"OutSine\","
        "\"InOutSine\","
        "\"InQuad\","
        "\"OutQuad\","
        "\"InOutQuad\","
        "\"InCubic\","
        "\"OutCubic\","
        "\"InOutCubic\","
        "\"InQuart\","
        "\"OutQuart\","
        "\"InOutQuart\","
        "\"InQuint\","
        "\"OutQuint\","
        "\"InOutQuint\","
        "\"InExpo\","
        "\"OutExpo\","
        "\"InOutExpo\","
        "\"InCirc\","
        "\"OutCirc\","
        "\"InOutCirc\","
        "\"InElastic\","
        "\"OutElastic\","
        "\"InOutElastic\","
        "\"InBack\","
        "\"OutBack\","
        "\"InOutBack\","
        "\"InBounce\","
        "\"OutBounce\","
        "\"InOutBounce\""
        "];"
        "E.animations.findKey = function(o, val) {"
        "    for(var key in o) {"
        "       if (o.hasOwnProperty(key)) {"
        "           if (o[key] == val) return parseInt(key);"
        "       } "
        "    }"
        "};"
        "E.animations.easing = function(a)"
        "{"
        "  if (typeof a == \"string\") return E.animations.findKey(E.animations.easingsToNames, a); else return  E.animations.easingsToNames[a]; "
        "};"
    );
    assert(b);
    return animations_value;
}

