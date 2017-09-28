#include "instancesbindings.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <db/dbdatabase.h>

#include "../querytable.h"
#include "../queryobject.h"
#include "../abstractgetter.h"
#include "../abstractsetter.h"
#include "../scripting.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationinstanceactions.h"

#include "../../history/instances/instancesnew.h"
#include "../../history/instances/instancesremove.h"
#include "../../history/instances/instanceschangename.h"
#include "../../history/instances/instanceschangeanimation.h"
#include "../../history/instances/instanceschangeobject.h"
#include "../../history/instances/instanceschangeway.h"
#include "../../history/instances/instanceschangestarttime.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiconsoleblock.h"
#include "../../gui/uiblocks/uianimationblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include <animations/animationswayinstance.h>

QVector<unsigned long long> scripting::instances::list()
{
    return scripting::query_table("animationinstances", "sad::animations::Instance");
}


unsigned long long scripting::instances::_addWayInstance(
    scripting::Scripting* scripting,
    sad::String name,
    const QVariant& way,
    const QVariant& object,
    double starttime
)
{

    sad::Maybe<sad::p2d::app::Way*> oway = scripting::query<sad::p2d::app::Way*>(way);
    if (oway.exists() == false)
    {
        bool converted = false;
        std::string error = "_addWayInstance(): argument 3 must be valid way";
        if (!converted && (way.canConvert(QVariant::ULongLong)))
        {
            converted = true;
            unsigned long long id = way.value<unsigned long long>();
            if (id != 0)
            {
                scripting->context()->throwError(error);
                throw new dukpp03::ArgumentException();
            }
        }
        if (!converted && way.canConvert(QVariant::String))
        {
            converted = true;
            sad::String name = way.value<QString>().toStdString();
            if (name.length() != 0)
            {
                scripting->context()->throwError(error);
                throw new dukpp03::ArgumentException();
            }
        }
        
    }
    sad::Maybe<sad::db::Object*> o = scripting::query<sad::db::Object*>(object);
    if (o.exists())
    {
        if (!(o.value()->isInstanceOf("sad::Scene") || o.value()->isInstanceOf("sad::SceneNode")))
        {
            o.clear();
        }
    }
    if (o.exists() == false)
    {
        bool converted = false;
        std::string error = "_addWayInstance(): argument 4 must be valid scene object";
        if (!converted && (object.canConvert(QVariant::ULongLong)))
        {
            converted = true;
            unsigned long long id = object.value<unsigned long long>();
            if (id != 0)
            {
                scripting->context()->throwError(error);
                throw new dukpp03::ArgumentException();
            }
        }
        if (!converted && object.canConvert(QVariant::String))
        {
            converted = true;
            sad::String name = object.value<QString>().toStdString();
            if (name.length() != 0)
            {
                scripting->context()->throwError(error);
                throw new dukpp03::ArgumentException();
            }
        }        
    }
    sad::animations::WayInstance* instance = new sad::animations::WayInstance();
    if (oway.exists())
    {
        instance->setProperty("way", oway.value()->MajorId);
    }
    else
    {
        instance->setProperty("way", 0ull);
    }

    if (o.exists())
    {
        instance->setProperty("object", o.value()->MajorId);
    }
    else
    {
        instance->setProperty("object", 0ull);
    }


    instance->setProperty("starttime", starttime);
    instance->setObjectName(name);

    sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

    unsigned long long result = instance->MajorId;
    core::Editor* e = scripting->editor();
    history::instances::New* c = new history::instances::New(instance);
    c->commit(e);
    e->currentBatchCommand()->add(c);

    return result;
}

void scripting::instances::remove(
    scripting::Scripting* scripting,
    sad::animations::Instance* instance
)
{
    scripting->editor()->actions()->instanceActions()->removeInstanceFromDatabase(instance, false);
}


void scripting::instances::checkProperties(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool
)
{
    if (obj.value()->isInstanceOf("sad::animations::Instance"))
    {
        list << "animation";
        list << "animationmajorid";
        list << "object";
        list << "starttime";
    }

    if (obj.value()->isInstanceOf("sad::animations::WayInstance"))
    {
        list << "way";
    }
}


dukpp03::qt::JSObject* scripting::instances::init(scripting::Scripting* s, dukpp03::qt::JSObject* animations)
{
    dukpp03::qt::JSObject*  instances_value = new dukpp03::qt::JSObject();
    instances_value->setProperty("list", dukpp03::qt::make_function::from(scripting::instances::list)); // E.animations.instances.list

    dukpp03::qt::MultiMethod* _add_instance = new dukpp03::qt::MultiMethod();
    {
        std::function<unsigned long long(scripting::Scripting*, sad::animations::Instance*, const sad::String& name, double start_time)>  add_instance_to_db = [](scripting::Scripting* sc, sad::animations::Instance* instance, const sad::String& name, double start_time) {
            instance->setProperty("starttime", start_time);
            instance->setObjectName(name);

            sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

            core::Editor* e = sc->editor();
            history::instances::New* c = new history::instances::New(instance);
            c->commit(e);
            e->currentBatchCommand()->add(c);
            return instance->MajorId;
        };
        // animation as sad::animations::Animation*
        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, sad::db::Object* o, double)> _add_instance_1 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, sad::db::Object* o, double start_time) -> unsigned long long {
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, unsigned long o, double)> _add_instance_2 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, unsigned long o, double start_time) {
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::animations::Animation*, sad::String o, double)> _add_instance_3 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::animations::Animation* animation, sad::String o, double start_time) {
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", sad::String(""));
            instance->setProperty("animationmajorid", animation->MajorId);

            return add_instance_to_db(sc, instance, name, start_time);
        };
        // animation as unsigned long long
        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long, sad::db::Object* o, double)> _add_instance_4 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, sad::db::Object* o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long, unsigned long o, double)> _add_instance_5 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, unsigned long o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, unsigned long long,  sad::String o, double)> _add_instance_6 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, unsigned long long animation, sad::String o, double start_time) {
            if (animation != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
            }
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();

            return add_instance_to_db(sc, instance, name, start_time);
        };

        // animation as sad::String
        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, sad::db::Object* o, double)> _add_instance_7 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, sad::db::Object* o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            if (o->isInstanceOf("sad::Scene") || o->isInstanceOf("sad::SceneNode"))
            {
                instance->setProperty("object", o->MajorId);
            }

            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, unsigned long o, double)> _add_instance_8 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, unsigned long o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            if (o != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must be valid object");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            return add_instance_to_db(sc, instance, name, start_time);
        };

        std::function<unsigned long long(scripting::Scripting*, sad::String, sad::String, sad::String o, double)> _add_instance_9 = [add_instance_to_db](scripting::Scripting* sc, sad::String name, sad::String animation, sad::String o, double start_time) {
            if (animation.length() != 0)
            {
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(animation);
                if (r)
                {
                    if (r->isInstanceOf("sad::animations::Animation") == false)
                    {
                        r = NULL;
                    }
                }
                if (!r)
                {
                    sc->context()->throwError("_addInstance(): Argument 2 must be valid animation");
                }
            }
            if (o.length() != 0)
            {
                sc->context()->throwError("_addInstance(): Argument 4 must have type sad::db::Object*");
            }
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setProperty("animation", animation);
            instance->setProperty("animationmajorid", 0ull);
            return add_instance_to_db(sc, instance, name, start_time);
        };

        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_1));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_2));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_3));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_4));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_5));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_6));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_7));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_8));
        _add_instance->add(dukpp03::qt::curried1::from(s, _add_instance_9));
    }
    instances_value->setProperty("_addInstance", static_cast<dukpp03::qt::Callable*>(_add_instance));                        // E.animations.instances._addInstance
    instances_value->setProperty("_addWayInstance", dukpp03::qt::curried1::from(s, scripting::instances::_addWayInstance));  // E.animations.instances._addWayInstance
    instances_value->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::instances::remove));                    // E.animations.instances.remove

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
    {
        // Setter for "name" property
        scripting::AbstractSetter<sad::animations::Instance*, sad::String>* name_setter = scripting::setterForProperty<sad::animations::Instance*, sad::String>(s, "name");
        std::function<
            void(scripting::Scripting*, sad::animations::Instance*, const sad::String&, sad::String, sad::String)
        > name_change_action = [](scripting::Scripting* sc, sad::animations::Instance* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor = sc->editor();

            gui::actions::AnimationInstanceActions* ai_actions = editor->actions()->instanceActions();
            gui::uiblocks::UIAnimationInstanceBlock* ai_blk = editor->uiBlocks()->uiAnimationInstanceBlock();


            int row = ai_actions->findInList<sad::animations::Instance*>(ai_blk->lstAnimationInstances, obj);

            if (row > -1)
            {
                history::Command* c = new history::instances::ChangeName(obj, row, oldvalue, newvalue);
                editor->currentBatchCommand()->add(c);
                c->commit(editor);
            }
        };
        name_setter->addAction(name_change_action);
        set->add(name_setter);
        // Setter for "animation" property
        scripting::AbstractSetter<sad::animations::Instance*, sad::String>* animation_setter = scripting::setterForProperty<sad::animations::Instance*, sad::String>(s, "animation");
        std::function<dukpp03::Maybe<sad::String>(const sad::String&)>  is_animation_resource = [](const sad::String& resource) {
            dukpp03::Maybe<sad::String> result;
            if (resource.length())
            {
                bool valid = false;
                sad::resource::Resource* r = sad::Renderer::ref()->tree("")->root()->resource(resource);
                if (r)
                {
                    valid = r->metaData()->canBeCastedTo("sad::animations::Animation");
                }
                if (!valid)
                {
                    result.setValue(resource + " : is not a reference to an animation from resources");
                }
            }
            return result;
        };
        std::function<
            void(scripting::Scripting*, sad::animations::Instance*, const sad::String&, sad::String, sad::String)
        > animation_change_action = [](scripting::Scripting* sc, sad::animations::Instance* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor = sc->editor();

            gui::actions::AnimationInstanceActions* ai_actions = editor->actions()->instanceActions();
            gui::uiblocks::UIAnimationInstanceBlock* ai_blk = editor->uiBlocks()->uiAnimationInstanceBlock();

            int row = ai_actions->findInList<sad::animations::Instance*>(ai_blk->lstAnimationInstances, obj);
            unsigned long long majorid = obj->getProperty<unsigned long long>("animationmajorid").value();
            if (row > -1)
            {
                history::Command* c = NULL;
                QRadioButton* treebutton = ai_blk->rbAnimationInstanceFromTree;
                QRadioButton* dbbutton = ai_blk->rbAnimationInstanceFromDatabase;
                if (newvalue.length() == 0)
                {
                    c = new history::instances::ChangeAnimation(
                        obj,
                        dbbutton,
                        treebutton,
                        oldvalue,
                        majorid,
                        newvalue,
                        majorid
                    );

                }
                else
                {
                    if (majorid > 0)
                    {
                        c = new history::instances::ChangeAnimation(
                            obj,
                            treebutton,
                            dbbutton,
                            oldvalue,
                            majorid,
                            newvalue,
                            majorid
                        );
                    }
                    else
                    {
                        c = new history::instances::ChangeAnimation(
                            obj,
                            oldvalue,
                            newvalue
                        );
                    }
                }
                editor->currentBatchCommand()->add(c);
                c->commit(editor);
            };
        };
        animation_setter->addCondition(is_animation_resource);
        animation_setter->addAction(animation_change_action);
        set->add(animation_setter);
        // Setter for animationmajorid
        scripting::AbstractSetter<sad::animations::Instance*, unsigned long long>* animation_major_id_setter = scripting::setterForProperty<sad::animations::Instance*, unsigned long long>(s, "animationmajorid");
        std::function<dukpp03::Maybe<unsigned long long>(dukpp03::qt::BasicContext* ctx, duk_idx_t pos)> get_animation = [](dukpp03::qt::BasicContext* ctx, duk_idx_t pos) {
            dukpp03::Maybe<unsigned long long> result;
            dukpp03::Maybe<sad::animations::Animation*> maybe_animation = dukpp03::GetValue<sad::animations::Animation*, dukpp03::qt::BasicContext>::perform(ctx, pos);
            if (maybe_animation.exists())
            {
                result.setValue(maybe_animation.value()->MajorId);
            }
            else
            {
                dukpp03::Maybe<sad::String> maybe_text = dukpp03::GetValue<sad::String, dukpp03::qt::BasicContext>::perform(ctx, pos);
                if (maybe_text.exists())
                {
                    if (maybe_text.value().length() == 0)
                    {
                        result.setValue(0);
                    }
                }
            }
            return result;
        };
        std::function<dukpp03::Maybe<sad::String>(unsigned long long)>  is_animation = [](unsigned long long id) {
            dukpp03::Maybe<sad::String> result;
            if (id > 0)
            {
                bool valid = false;
                sad::db::Object* r = sad::Renderer::ref()->database("")->queryByMajorId(id);
                if (r)
                {
                    valid = r->isInstanceOf("sad::animations::Animation");
                }
                if (!valid)
                {
                    result.setValue("Argument is not a reference to an animation from database");
                }
            }
            return result;
        };
        
        std::function<
            void(scripting::Scripting*, sad::animations::Instance*, const sad::String&, unsigned long long, unsigned long long)
        > animation_major_id_change_action = [](scripting::Scripting* sc, sad::animations::Instance* obj, const sad::String& propertyname, unsigned long long oldid, unsigned long long newid) {
            core::Editor* editor = sc->editor();

            gui::actions::AnimationInstanceActions* ai_actions = editor->actions()->instanceActions();
            gui::uiblocks::UIAnimationInstanceBlock* ai_blk = editor->uiBlocks()->uiAnimationInstanceBlock();

            int row = ai_actions->findInList<sad::animations::Instance*>(ai_blk->lstAnimationInstances, obj);
            sad::String name = obj->getProperty<sad::String>("animation").value();
            if (row > -1)
            {
                history::Command* c = NULL;
                QRadioButton* treebutton = ai_blk->rbAnimationInstanceFromTree;
                QRadioButton* dbbutton = ai_blk->rbAnimationInstanceFromDatabase;
                if (newid > 0)
                {
                    if (oldid > 0)
                    {
                        c = new history::instances::ChangeAnimation(
                            obj,
                            oldid,
                            newid
                        );
                    }
                    else
                    {
                        c = new history::instances::ChangeAnimation(
                            obj,
                            dbbutton,
                            treebutton,
                            name,
                            oldid,
                            name,
                            newid
                        );
                    }

                }
                else
                {
                    c = new history::instances::ChangeAnimation(
                        obj,
                        treebutton,
                        dbbutton,
                        name,
                        oldid,
                        name,
                        newid
                    );
                }
                editor->currentBatchCommand()->add(c);
                c->commit(editor);
            }
        };
        animation_major_id_setter->addConverter(get_animation);
        animation_major_id_setter->addCondition(is_animation);
        animation_major_id_setter->addAction(animation_major_id_change_action);
        set->add(animation_major_id_setter);
        // Setter for object
        scripting::AbstractSetter<sad::animations::Instance*, unsigned long long>* object_setter = scripting::setterForProperty<sad::animations::Instance*, unsigned long long>(s, "object");
        
        std::function<dukpp03::Maybe<unsigned long long>(dukpp03::qt::BasicContext* ctx, duk_idx_t pos)> get_object = [](dukpp03::qt::BasicContext* ctx, duk_idx_t pos) {
            dukpp03::Maybe<unsigned long long> result;
            dukpp03::Maybe<sad::db::Object*> maybe_object = dukpp03::GetValue<sad::db::Object*, dukpp03::qt::BasicContext>::perform(ctx, pos);
            if (maybe_object.exists())
            {
                result.setValue(maybe_object.value()->MajorId);
            }
            else
            {
                dukpp03::Maybe<sad::String> maybe_text = dukpp03::GetValue<sad::String, dukpp03::qt::BasicContext>::perform(ctx, pos);
                if (maybe_text.exists())
                {
                    if (maybe_text.value().length() == 0)
                    {
                        result.setValue(0);
                    }
                }
            }
            return result;
        };

        std::function<dukpp03::Maybe<sad::String>(unsigned long long)>  is_object = [](unsigned long long id) {
            dukpp03::Maybe<sad::String> result;
            if (id > 0)
            {
                bool valid = false;
                sad::db::Object* r = sad::Renderer::ref()->database("")->queryByMajorId(id);
                if (r)
                {
                    valid = r->isInstanceOf("sad::Scene") || r->isInstanceOf("sad::SceneNode");
                }
                if (!valid)
                {
                    result.setValue("Argument is not a reference to an object from database");
                }
            }
            return result;
        };

        std::function<
            void(scripting::Scripting*, sad::animations::Instance*, const sad::String&, unsigned long long, unsigned long long)
        > object_change_action = [](scripting::Scripting* sc, sad::animations::Instance* obj, const sad::String& propertyname, unsigned long long oldid, unsigned long long newid) {
            core::Editor* editor = sc->editor();

            history::Command* c = new history::instances::ChangeObject(obj, oldid, newid);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        object_setter->addConverter(get_object);
        object_setter->addCondition(is_object);
        object_setter->addAction(object_change_action);
        set->add(object_setter);
        // Setter for way
        scripting::AbstractSetter<sad::animations::WayInstance*, unsigned long long>* way_setter = scripting::setterForProperty<sad::animations::WayInstance*, unsigned long long>(s, "way");
        std::function<dukpp03::Maybe<sad::String>(unsigned long long)>  is_way = [](unsigned long long id) {
            dukpp03::Maybe<sad::String> result;
            if (id > 0)
            {
                bool valid = false;
                sad::db::Object* r = sad::Renderer::ref()->database("")->queryByMajorId(id);
                if (r)
                {
                    valid = r->isInstanceOf("sad::p2d::app::Way");
                }
                if (!valid)
                {
                    result.setValue("Argument is not a reference to a way from database");
                }
            }
            return result;
        };

        std::function<
            void(scripting::Scripting*, sad::animations::WayInstance*, const sad::String&, unsigned long long, unsigned long long)
        > way_change_action = [](scripting::Scripting* sc, sad::animations::WayInstance* obj, const sad::String& propertyname, unsigned long long oldid, unsigned long long newid) {
            core::Editor* editor = sc->editor();

            history::Command* c = new history::instances::ChangeWay(obj, oldid, newid);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        way_setter->addConverter(get_object);
        way_setter->addCondition(is_way);
        way_setter->addAction(way_change_action);
        set->add(way_setter);

        scripting::AbstractSetter<sad::animations::Instance*, double>* start_time_setter = scripting::setterForProperty<sad::animations::Instance*, double>(s, "starttime");
        std::function<
            void(scripting::Scripting*, sad::animations::Instance*, const sad::String&, double, double)
        > start_time_change_action = [](scripting::Scripting* sc, sad::animations::Instance* obj, const sad::String& propertyname, double old_value, double new_value) {
            core::Editor* editor = sc->editor();

            history::Command* c = new history::instances::ChangeStartTime(obj, old_value, new_value);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        start_time_setter->addAction(start_time_change_action);
        set->add(start_time_setter);
    }
    instances_value->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.animations.instances.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, sad::String>("animation"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>("animationmajorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>("object"));
        get->add(new scripting::AbstractGetter<sad::animations::Instance*, double>("starttime"));
        get->add(new scripting::AbstractGetter<sad::animations::WayInstance*, unsigned long long>("way"));
    }
    instances_value->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.animations.instances.get

    animations->setProperty("instances", instances_value);

    bool b = s->context()->eval(
        "E.animations.instances.addInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"animation\" in o == false)"
        "   {                              "
        "     o[\"animation\"] = 0;        "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addInstance(o[\"name\"], o[\"animation\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.addWayInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"way\" in o == false)     "
        "   {                              "
        "      o[\"way\"] = 0;             "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addWayInstance(o[\"name\"], o[\"way\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.instances.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       E.animations.instances.set(arguments[0], arguments[1], arguments[2]); return E.animations.instances;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert( b );
    return instances_value;
}