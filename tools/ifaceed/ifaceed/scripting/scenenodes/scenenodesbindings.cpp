#include "scenenodesbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../core/editor.h"

#include "../../history/scenenodes/scenenodesnew.h"
#include "../../history/scenenodes/scenenodesspan.h"
#include "../../history/scenenodes/scenenodeschangename.h"
#include "../../history/scenenodes/scenenodeschangeangle.h"
#include "../../history/scenenodes/scenenodeschangecolor.h"
#include "../../history/scenenodes/scenenodeschangearea.h"
#include "../../history/scenenodes/scenenodeschangevisibility.h"

#include "../../history/label/changefontsize.h"
#include "../../history/label/changetext.h"
#include "../../history/label/changefontname.h"
#include "../../history/label/changelinespacing.h"
#include "../../history/label/changemaximallinewidth.h"
#include "../../history/label/changebreaktext.h"
#include "../../history/label/changeoverflowstrategy.h"
#include "../../history/label/changetextellipsis.h"
#include "../../history/label/changemaximallinescount.h"
#include "../../history/label/changeoverflowstrategyforlines.h"
#include "../../history/label/changetextellipsisforlines.h"
#include "../../history/label/changehasformatting.h"

#include "../../history/sprite2d/changeflipx.h"
#include "../../history/sprite2d/changeflipy.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"
#include "../../gui/actions/sprite2dactions.h"
#include "../../gui/actions/gridactions.h"

#include "scenenodessetter.h"
#include "scenenodesflagsetter.h"
#include "scenenodesoptionssetter.h"
#include "scenenodesschemasetter.h"
#include "scenenodescustomsetter.h"
#include "scenenodescustomgetter.h"

#include "../abstractgetter.h"

#include <renderer.h>
#include <label.h>
#include <sprite2d.h>

#include <resource/tree.h>
#include <resource/resource.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

#include <db/custom/customobject.h>

QVector<unsigned long long>  scripting::scenenodes::list()
{
    return scripting::query_table("scenenodes", "sad::SceneNode");
}

QVector<unsigned long long> scripting::scenenodes::listScene(sad::Scene* scene)
{
    QVector<unsigned long long> result;
    const sad::Vector<sad::SceneNode*>& scenenodes = scene->objects();
    for(size_t i = 0; i < scenenodes.size(); i++)
    {
        result << scenenodes[i]->MajorId;
    }
    return result;
}


unsigned long long scripting::scenenodes::_addLabel(
    scripting::Scripting* scripting,
    sad::Scene* scene,
    sad::String resource,
    unsigned int fontsize,
    sad::String text,
    sad::String name,
    sad::Point2D topleftpoint,
    sad::AColor clr
)
{
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybefont = tree->root()->resource(resource);
    bool error = true;
    if (maybefont)
    {
        if (maybefont->metaData()->canBeCastedTo("sad::Font")
            || maybefont->metaData()->canBeCastedTo("sad::freetype::Font"))
        {
            error = false;
        }
    }
    if (error)
    {
        std::string resource_not_found_error = std::string("_addLabel: ") + resource.c_str() + " does not name font resource";
        scripting->context()->throwError(resource_not_found_error);
        throw new dukpp03::ArgumentException();
        return 0;
    }

    sad::Renderer::ref()->lockRendering();

    sad::Label* label = new sad::Label(resource, topleftpoint, text, "");
    label->setObjectName(name);
    label->setColor(clr);
    label->setSize(fontsize);

    scene->add(label);
    sad::Renderer::ref()->database("")->table("scenenodes")->add(label);

    sad::Renderer::ref()->unlockRendering();

    history::scenenodes::New* cmd = new history::scenenodes::New(label);
    core::Editor* c = scripting->editor();
    cmd->commit(c);
    c->currentBatchCommand()->add(cmd);

    return label->MajorId;
}


unsigned long long scripting::scenenodes::_addSprite2D(
    scripting::Scripting* scripting,
    sad::Scene* scene,
    sad::String resource,
    sad::String name,
    sad::Rect2D rect,
    sad::AColor clr
)
{
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybeopts = tree->root()->resource(resource);
    bool error = true;
    if (maybeopts)
    {
        if (maybeopts->metaData()->canBeCastedTo("sad::Sprite2D::Options"))
        {
            error = false;
        }
    }

    if (error)
    {
        std::string resource_not_found_error = std::string("_addSprite2D: ") + resource.c_str() + " does not name sprite options resource (sad::Sprite2D::Options type)";
        scripting->context()->throwError(resource_not_found_error);
        throw new dukpp03::ArgumentException();
        return 0;
    }

    if (sad::isAABB(rect) == false)
    {
        std::string rectangle_argument_error = "_addSprite2D: passed rectangle must be axis-aligned bounding box";
        scripting->context()->throwError(rectangle_argument_error);
        throw new dukpp03::ArgumentException();
        return 0;
    }

    sad::Renderer::ref()->lockRendering();

    sad::Sprite2D* sprite = new sad::Sprite2D();
    sprite->setTreeName(sad::Renderer::ref(), "");
    sprite->set(resource);
    sprite->setArea(rect);
    sprite->setObjectName(name);
    sprite->setColor(clr);

    scene->add(sprite);
    sad::Renderer::ref()->database("")->table("scenenodes")->add(sprite);

    sad::Renderer::ref()->unlockRendering();

    history::scenenodes::New* cmd = new history::scenenodes::New(sprite);
    core::Editor* c = scripting->editor();
    cmd->commit(c);
    c->currentBatchCommand()->add(cmd);

    return sprite->MajorId;
}

unsigned long long scripting::scenenodes::_addCustomObject(
    scripting::Scripting* scripting,
    sad::Scene* scene,
    sad::String resource,
    sad::String name,
    unsigned int fontsize,
    sad::String text,
    sad::Rect2D rect,
    sad::AColor clr
)
{
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybeschema = tree->root()->resource(resource);
    bool error = true;
    if (maybeschema)
    {
        if (maybeschema->metaData()->canBeCastedTo("sad::db::custom::Schema"))
        {
            error = false;
        }
    }

    if (error)
    {
        std::string resource_not_found_error = std::string("_addCustomObject: ") + resource.c_str() + " does not name schema resource (sad::db::custom::Schema type)";
        scripting->context()->throwError(resource_not_found_error);
        throw new dukpp03::ArgumentException();
        return 0;
    }

    if (sad::isAABB(rect) == false)
    {
        std::string rectangle_argument_error = "_addCustomObject: passed rectangle must be axis-aligned bounding box";
        scripting->context()->throwError(rectangle_argument_error);
        throw new dukpp03::ArgumentException();
        return 0;
    }

    sad::Renderer::ref()->lockRendering();

    sad::db::custom::Object* obj = new sad::db::custom::Object();

    obj->setTreeName(sad::Renderer::ref(), "");

    obj->setSchemaName(resource);
    obj->setObjectName(name);
    obj->setFontSize(fontsize);
    obj->setString(text);
    obj->setArea(rect);
    obj->setColor(clr);

    scene->add(obj);
    sad::Renderer::ref()->database("")->table("scenenodes")->add(obj);

    sad::Renderer::ref()->unlockRendering();

    history::scenenodes::New* cmd = new history::scenenodes::New(obj);
    core::Editor* c = scripting->editor();
    cmd->commit(c);
    c->currentBatchCommand()->add(cmd);

    return obj->MajorId;
}

void scripting::scenenodes::makeBackground(
    scripting::Scripting* scripting,
    sad::SceneNode* node
)
{
    scripting->editor()->actions()->sprite2DActions()->setSceneNodeAsBackground(node, false);
}

void scripting::scenenodes::remove(scripting::Scripting* scripting,  sad::SceneNode* node)
{
    scripting->editor()->actions()->sceneNodeActions()->removeSceneNode(node, false);
}


void scripting::scenenodes::spanBetweenTwoPoints(
    scripting::Scripting* scripting,
    sad::SceneNode* node,
    sad::Point2D p1,
    sad::Point2D p2
)
{
    if (node)
    {

        sad::Maybe<sad::Rect2D> old_area_maybe = node->getProperty<sad::Rect2D>("area");
        sad::Maybe<double> old_angle_maybe = node->getProperty<double>("angle");
        if (old_angle_maybe.exists() && old_area_maybe.exists())
        {
            core::Editor* e = scripting->editor(); 
            // Do not allow usr to span node in case it's within a grid
            if (e->actions()->gridActions()->isInGrid(node))
            {
                return;
            }
            gui::actions::SceneNodeActions* scene_node_actions = e->actions()->sceneNodeActions();
            scene_node_actions->spanObjectBetweenTwoPoints(node, p1, p2);
            sad::Maybe<sad::Rect2D> new_area_maybe = node->getProperty<sad::Rect2D>("area");
            sad::Maybe<double> new_angle_maybe = node->getProperty<double>("angle");
            e->currentBatchCommand()->add(new history::scenenodes::Span(
                node,
                old_area_maybe.value(),
                old_angle_maybe.value(),
                new_area_maybe.value(),
                new_angle_maybe.value()
            ));
            if (e->shared()->selectedObject() == node)
            {
                scene_node_actions->updateRegionForNode();
                scene_node_actions->updateAngleForNode();
            }
        }
    }
}

dukpp03::qt::JSObject* scripting::scenenodes::init(scripting::Scripting* s, dukpp03::qt::JSObject* e)
{
    dukpp03::qt::JSObject* scenenodes = new dukpp03::qt::JSObject();
    scenenodes->setProperty("list", dukpp03::qt::make_function::from(scripting::scenenodes::list));
    scenenodes->setProperty("listScene", dukpp03::qt::make_function::from(scripting::scenenodes::listScene));
    scenenodes->setProperty("_addLabel", dukpp03::qt::curried1::from(s, scripting::scenenodes::_addLabel));
    scenenodes->setProperty("_addSprite2D", dukpp03::qt::curried1::from(s, scripting::scenenodes::_addSprite2D));
    scenenodes->setProperty("_addCustomObject", dukpp03::qt::curried1::from(s, scripting::scenenodes::_addCustomObject));
    scenenodes->setProperty("makeBackground", dukpp03::qt::curried1::from(s, scripting::scenenodes::makeBackground));
    scenenodes->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::scenenodes::remove));
    scenenodes->setProperty("spanBetweenTwoPoints", dukpp03::qt::curried1::from(s, scripting::scenenodes::spanBetweenTwoPoints));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();

    {
        // All props
        set->add(new scripting::scenenodes::FlagSetter(s, "visible", history::scenenodes::changeVisibility));
        set->add(new scripting::scenenodes::Setter<sad::String, history::scenenodes::ChangeName>(s, "name"));

        scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>* area_setter = new scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>(s, "area");
        std::function<dukpp03::Maybe<sad::String>(const sad::Rect2D&)> is_aabb = [](const sad::Rect2D& val) {
            dukpp03::Maybe<sad::String> result;
            if (sad::isAABB(val) == false)
            {
                result.setValue("Rectangle must be axis-aligned");
            }
            return result;
        };
        area_setter->addCondition(is_aabb);
        set->add(area_setter);

        set->add(new scripting::scenenodes::Setter<double, history::scenenodes::ChangeAngle>(s, "angle"));
        set->add(new scripting::scenenodes::Setter<sad::AColor, history::scenenodes::ChangeColor>(s, "color"));

        // sad::Label props
        scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>* font_size_setter = new scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>(s, "fontsize");
        std::function<dukpp03::Maybe<sad::String>(const unsigned long&)> is_greater_than_zero = [](const unsigned long& val) {
            dukpp03::Maybe<sad::String> result;
            if (val == 0)
            {
                result.setValue("Value must be greater than zero");
            }
            return result;
        };
        font_size_setter->addCondition(is_greater_than_zero);
        set->add(font_size_setter);
        set->add(new scripting::scenenodes::Setter<sad::String, history::label::ChangeText>(s, "text"));
        set->add(new scripting::scenenodes::Setter<float, history::label::ChangeLineSpacing>(s, "linespacing"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLineWidth>(s, "maximallinewidth"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategy>(s, "overflowstrategy"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeBreakText>(s, "breaktext"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsis>(s, "textellipsisposition"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLinesCount>(s, "maximallinescount"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategyForLines>(s, "overflowstrategyforlines"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsisForLines>(s, "textellipsispositionforlines"));
        set->add(new scripting::scenenodes::Setter<bool, history::label::ChangeHasFormatting>(s, "hasformatting"));

        scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>* font_setter = new scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>(s, "font");
        std::function<dukpp03::Maybe<sad::String>(const sad::String&)> font_exists = [](const sad::String& resource_name) {
            dukpp03::Maybe<sad::String> result;
            sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resource_name);
            bool valid = false;
            if (resource) {
                if (resource->metaData()->canBeCastedTo("sad::freetype::Font")
                    || resource->metaData()->canBeCastedTo("sad::TextureMappedFont")
                    || resource->metaData()->canBeCastedTo("sad::Font"))
                {
                    valid = true;
                }
            }
            if (!valid)
            {
                result.setValue(resource_name + " is not a font resource");
            }
            return result;
        };
        font_setter->addCondition(font_exists);
        set->add(font_setter);
        // sad::Sprite2D props
        set->add(new scripting::scenenodes::FlagSetter(s, "flipx", history::sprite2d::changeFlipX));
        set->add(new scripting::scenenodes::FlagSetter(s, "flipy", history::sprite2d::changeFlipY));
        set->add(new scripting::scenenodes::OptionsSetter(s));
        // sad::db::CustomObject props
        set->add(new scripting::scenenodes::SchemaSetter(s));
#define PUSH_SETTER(TYPE) set->add(new scripting::scenenodes::CustomSetter< TYPE >(s));
        PUSH_SETTER(double)
            PUSH_SETTER(float)
            PUSH_SETTER(int)
            PUSH_SETTER(long)
            PUSH_SETTER(long long)
            PUSH_SETTER(sad::AColor)
            PUSH_SETTER(sad::Color)
            PUSH_SETTER(sad::Point2D)
            PUSH_SETTER(sad::Point2I)
            PUSH_SETTER(sad::Point3D)
            PUSH_SETTER(sad::Point3I)
            PUSH_SETTER(sad::Size2D)
            PUSH_SETTER(sad::Size2I)
            PUSH_SETTER(sad::Rect2D)
            PUSH_SETTER(sad::Rect2I)
            PUSH_SETTER(sad::String)
            PUSH_SETTER(std::string)
            PUSH_SETTER(QString)
            PUSH_SETTER(short)
            PUSH_SETTER(bool)
            PUSH_SETTER(char)
            PUSH_SETTER(signed char)
            PUSH_SETTER(unsigned char)
            PUSH_SETTER(unsigned int)
            PUSH_SETTER(unsigned long)
            PUSH_SETTER(unsigned long long)
            PUSH_SETTER(unsigned short)
#undef PUSH_SETTER
    }

    scenenodes->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.scenenodes.set
    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        // All
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("layer"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("scene"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("visible"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::Rect2D>("area"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, double>("angle"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::AColor>("color"));
        // sad::Label props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("fontsize"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("text"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, float>("linespacing"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("font"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("maximallinewidth"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("overflowstrategy"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("breaktext"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("textellipsisposition"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("maximallinescount"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("overflowstrategyforlines"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("textellipsispositionforlines"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("hasformatting"));

        // sad::Sprite2D props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("flipx"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("flipy"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("options"));
        // sad::db::CustomObject props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("schema"));
#define PUSH_GETTER(TYPE) get->add(scripting::scenenodes::custom_getter< TYPE >());
            PUSH_GETTER(double)
            PUSH_GETTER(float)
            PUSH_GETTER(int)
            PUSH_GETTER(long)
            PUSH_GETTER(long long)
            PUSH_GETTER(sad::AColor)
            PUSH_GETTER(sad::Color)
            PUSH_GETTER(sad::Point2D)
            PUSH_GETTER(sad::Point2I)
            PUSH_GETTER(sad::Point3D)
            PUSH_GETTER(sad::Point3I)
            PUSH_GETTER(sad::Size2D)
            PUSH_GETTER(sad::Size2I)
            PUSH_GETTER(sad::Rect2D)
            PUSH_GETTER(sad::Rect2I)
            PUSH_GETTER(sad::String)
            PUSH_GETTER(std::string)
            PUSH_GETTER(QString)
            PUSH_GETTER(short)
            PUSH_GETTER(bool)
            PUSH_GETTER(char)
            PUSH_GETTER(signed char)
            PUSH_GETTER(unsigned char)
            PUSH_GETTER(unsigned int)
            PUSH_GETTER(unsigned long)
            PUSH_GETTER(unsigned long long)
            PUSH_GETTER(unsigned short)
#undef PUSH_GETTER
    }
    scenenodes->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.scenenodes.get

    e->setProperty("scenenodes", scenenodes);

    bool b = s->context()->eval(
        "E.OverflowStrategy = { \"Visible\": 0, \"Hidden\": 1, \"Ellipsis\": 2 };"
        "E.BreakText = { \"Normal\": 0, \"BreakWord\": 1};"
        "E.TextEllipsisPosition = { \"Begin\": 0, \"Middle\": 1, \"End\": 2};"
    );
    assert(b);

    b = s->context()->eval(
        "E.scenenodes.addLabel = function(o) {"
        "   if (\"fontsize\" in o == false)"
        "   {                              "
        "     o[\"fontsize\"] = 16;        "
        "   }                              "
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addLabel(o[\"scene\"], o[\"font\"], o[\"fontsize\"], o[\"text\"], o[\"name\"], o[\"point\"], o[\"color\"]);"
        "};"
        "E.scenenodes.addSprite2D = function(o) {"
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addSprite2D(o[\"scene\"], o[\"sprite\"], o[\"name\"], o[\"area\"], o[\"color\"]);"
        "};"
        "E.scenenodes.addCustomObject = function(o) {"
        "   if (\"fontsize\" in o == false)"
        "   {                              "
        "     o[\"fontsize\"] = 16;        "
        "   }                              "
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addCustomObject(o[\"scene\"], o[\"schema\"], o[\"name\"], o[\"fontsize\"], o[\"text\"],  o[\"area\"], o[\"color\"]);"
        "};"
        "E.scenenodes.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.scenenodes.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.scenenodes.set(arguments[0], arguments[1], arguments[2]); return E.scenenodes;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert(b);
    return scenenodes;
}