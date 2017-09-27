#include "groupsbindings.h"

#include <db/dbdatabase.h>

#include "../abstractgetter.h"
#include "../abstractsetter.h"

#include "../querytable.h"
#include "../queryobject.h"
#include "../scripting.h"

#include "../../qstdstring.h"

#include "../../core/editor.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/animationgroupactions.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"
#include "../../gui/uiblocks/uianimationinstanceblock.h"

#include "../../history/groups/groupsnew.h"
#include "../../history/groups/groupsaddinstance.h"
#include "../../history/groups/groupsremoveinstance.h"
#include "../../history/groups/groupschangename.h"
#include "../../history/groups/groupschangelooped.h"
#include "../../history/groups/groupschangesequential.h"

Q_DECLARE_METATYPE(sad::animations::Group*); //-V566

QVector<unsigned long long> scripting::groups::list()
{
    return scripting::query_table("animationgroups", "sad::animations::Group");
}


unsigned long long scripting::groups::_add(
    scripting::Scripting* scripting, 
    sad::String name,
    bool looped
)
{
    core::Editor* e = scripting->editor();

    sad::animations::Group* d = new sad::animations::Group();
    d->setObjectName(name);
    d->setLooped(looped);


    sad::Renderer::ref()->database("")->table("animationgroups")->add(d);
    history::groups::New* c = new history::groups::New(d);
    c->commit(e);
    e->currentBatchCommand()->add(c);

    return d->MajorId;
}


void scripting::groups::remove(
    scripting::Scripting* scripting,
    sad::animations::Group* group
)
{
    scripting->editor()->actions()->groupActions()->removeFromDatabase(group, false);
}

unsigned int scripting::groups::length(
    scripting::Scripting*,
    sad::animations::Group* group
)
{
    return group->instances().size();
}

unsigned long long scripting::groups::entry(
    scripting::Scripting*,
    sad::animations::Group* group,
    unsigned int pos
)
{
    sad::Vector<unsigned long long> v = group->instances();
    unsigned long long result = 0;
    if (pos < v.size())
    {
        result = v[pos];
    }
    return result;
}

bool scripting::groups::addInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,	
    sad::animations::Instance* minstance
)
{
    
    bool alreadyinlist = false;
    int instanceposition = -1;
    bool result = false;

    core::Editor* e = scripting->editor();
    gui::actions::AnimationGroupActions* ag_actions = e->actions()->groupActions();
    gui::uiblocks::UIAnimationsGroupBlock* blk = e->uiBlocks()->uiAnimationsGroupBlock();
    gui::uiblocks::UIAnimationInstanceBlock* ai_blk = e->uiBlocks()->uiAnimationInstanceBlock();
    
    int groupposition =  ag_actions->findInList<sad::animations::Group*>(blk->lstAnimationsGroup, group);
    if (groupposition > -1)
    {
        sad::Vector<unsigned long long> ids = group->instances();
        QListWidget* list = ai_blk->lstAnimationInstances;
        
        if (std::find(ids.begin(), ids.end(), minstance->MajorId) != ids.end())
        {
            alreadyinlist = true;
        }
        if (!alreadyinlist)
        {
            bool positionfound = false;
            for(int i = 0; i < list->count() && !positionfound; i++)
            {
                sad::animations::Instance* instance = list->item(i)->data(Qt::UserRole).value<sad::animations::Instance*>();

                if (std::find(ids.begin(), ids.end(), instance->MajorId) == ids.end())
                {
                    ++instanceposition;
                }

                if (instance == minstance)
                {
                    positionfound = true;
                }
            }

            if (positionfound)
            {
                result = true;

                history::groups::AddInstance* c = new history::groups::AddInstance(group, minstance, instanceposition);
                c->commit(e);

                e->currentBatchCommand()->add(c);
            }
        }
    }

    return !alreadyinlist && result;
}

bool scripting::groups::removeInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,
    unsigned int pos
)
{
    sad::Vector<unsigned long long> instances = group->instances();
    bool result = false;
    if (pos < instances.size())
    {
        unsigned long long id = instances[pos];
        sad::db::Object* d = sad::Renderer::ref()->database("")->queryByMajorId(id);
        if (d)
        {
            if (d->isInstanceOf("sad::animations::Instance"))
            {
                sad::animations::Instance* i = static_cast<sad::animations::Instance*>(d);
                result = true;

                core::Editor* editor = scripting->editor();

                history::groups::RemoveInstance* c = new history::groups::RemoveInstance(group, i, pos);
                c->commit(editor);

                editor->currentBatchCommand()->add(c);
            }
        }
    }
    return result;
}

void scripting::groups::checkProperties(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool readable
)
{
    if (obj.value()->isInstanceOf("sad::animations::Group"))
    {
        list << "looped";
        list << "sequential";
        if (readable)
        {
            list << "instances";
        }
    }
}

dukpp03::qt::JSObject* scripting::groups::init(scripting::Scripting* s, dukpp03::qt::JSObject* animations)
{
    dukpp03::qt::JSObject*  groups_value = new dukpp03::qt::JSObject();
    groups_value->setProperty("list", dukpp03::qt::make_function::from(scripting::groups::list));   // E.animations.groups.list
    groups_value->setProperty("_add", dukpp03::qt::curried1::from(s, scripting::groups::_add));     // E.animations.groups._add
    groups_value->setProperty("remove", dukpp03::qt::curried1::from(s, scripting::groups::remove)); // E.animations.groups.remove
    groups_value->setProperty("length", dukpp03::qt::curried1::from(s, scripting::groups::length)); // E.animations.groups.length
    groups_value->setProperty("entry", dukpp03::qt::curried1::from(s, scripting::groups::entry)); // E.animations.groups.entry
    groups_value->setProperty("addInstance", dukpp03::qt::curried1::from(s, scripting::groups::addInstance)); // E.animations.groups.addInstance
    groups_value->setProperty("removeInstance", dukpp03::qt::curried1::from(s, scripting::groups::removeInstance)); // E.animations.groups.removeInstance

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
    {
        // Setter for "name" property
        scripting::AbstractSetter<sad::animations::Group*, sad::String>* name_setter = scripting::setterForProperty<sad::animations::Group*, sad::String>(s, "name");
        std::function<
            void(scripting::Scripting*, sad::animations::Group*, const sad::String&, sad::String, sad::String)
        > name_change_action = [](scripting::Scripting* sc, sad::animations::Group* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor = sc->editor();

            gui::actions::AnimationGroupActions* ag_actions = editor->actions()->groupActions();
            gui::uiblocks::UIAnimationsGroupBlock* blk = editor->uiBlocks()->uiAnimationsGroupBlock();

            int row = ag_actions->findInList<sad::animations::Group*>(blk->lstAnimationsGroup, obj);

            if (row > -1)
            {
                history::Command* c = new history::groups::ChangeName(obj, row, oldvalue, newvalue);
                editor->currentBatchCommand()->add(c);
                c->commit(editor);
            }
        };
        name_setter->addAction(name_change_action);
        set->add(name_setter);
        // Setter for "looped" property
        scripting::AbstractSetter<sad::animations::Group*, bool>* looped_setter = scripting::setterForProperty<sad::animations::Group*, bool>(s, "looped");
        std::function<
            void(scripting::Scripting*, sad::animations::Group*, const sad::String&, bool, bool)
        > looped_change_action = [](scripting::Scripting* sc, sad::animations::Group* obj, const sad::String& propertyname, bool oldvalue, bool newvalue) {
            core::Editor* editor = sc->editor();

            history::Command* c = new history::groups::ChangeLooped(obj, oldvalue, newvalue);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        looped_setter->addAction(looped_change_action);
        set->add(looped_setter);
        // Setter for "sequential" property
        scripting::AbstractSetter<sad::animations::Group*, bool>* sequential_setter = scripting::setterForProperty<sad::animations::Group*, bool>(s, "sequential");
        std::function<
            void(scripting::Scripting*, sad::animations::Group*, const sad::String&, bool, bool)
        > sequential_change_action = [](scripting::Scripting* sc, sad::animations::Group* obj, const sad::String& propertyname, bool oldvalue, bool newvalue) {
            core::Editor* editor = sc->editor();

            history::Command* c = new history::groups::ChangeSequential(obj, oldvalue, newvalue);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        sequential_setter->addAction(sequential_change_action);
        set->add(sequential_setter);
    }
    groups_value->setProperty("set", static_cast<dukpp03::qt::Callable*>(set)); // E.animations.groups.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        get->add(new scripting::AbstractGetter<sad::animations::Group*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::animations::Group*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Group*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::animations::Group*, bool>("looped"));
        get->add(new scripting::AbstractGetter<sad::animations::Group*, bool>("sequential"));
        get->add(new scripting::AbstractGetter<sad::animations::Group*, sad::Vector<unsigned long long> >("instances"));
    }
    groups_value->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.animations.groups.get

    animations->setProperty("groups", groups_value);

    bool b  = s->context()->eval(
        "E.animations.groups.add = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"looped\" in o == false)  "
        "   {                              "
        "      o[\"looped\"] = false;      "
        "   }                              "
        "   return E.animations.groups._add(o[\"name\"], o[\"looped\"]);"
        "};"
        "E.animations.groups.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.groups.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       E.animations.groups.set(arguments[0], arguments[1], arguments[2]); return E.animations.groups;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert( b );

    return groups_value;
}