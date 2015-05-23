#include "scenenodesbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"
#include "../../qstdstring.h"


#include "../../core/editor.h"

#include "../../history/scenenodes/scenenodesnew.h"

#include "../../gui/scenenodeactions.h"
#include "../../gui/sprite2dactions.h"

#include <renderer.h>
#include <label.h>
#include <sprite2d.h>

#include <resource/tree.h>
#include <resource/resource.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

#include <db/custom/customobject.h>

QScriptValue scripting::scenenodes::list(
    QScriptContext* ctx,
    QScriptEngine* engine
)
{
	return scripting::query_table("scenenodes", "sad::SceneNode", ctx, engine);
}

QVector<unsigned long long> scripting::scenenodes::listScene(scripting::Scripting* scripting, sad::Scene* scene)
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
    QScriptEngine* engine = scripting->engine();
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybefont = tree->root()->resource(resource);
    if (!maybefont)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name font resource");
        return 0;
    }

    if (maybefont->metaData()->canBeCastedTo("sad::Font") == false
        && maybefont->metaData()->canBeCastedTo("sad::freetype::Font") == false)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name font resource");
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
    core::Editor* c = scripting->panel()->editor();
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
    QScriptEngine* engine = scripting->engine();
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybeopts = tree->root()->resource(resource);
    if (!maybeopts)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name sprite options resource (sad::Sprite2D::Options type)");
        return 0;
    }

    if (maybeopts->metaData()->canBeCastedTo("sad::Sprite2D::Options") == false)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name sprite options resource (sad::Sprite2D::Options type)");
        return 0;
    }

    if (sad::isAABB(rect) == false)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " rectangle must be axis aligned");
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
    core::Editor* c = scripting->panel()->editor();
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
    QScriptEngine* engine = scripting->engine();
    sad::resource::Tree* tree = sad::Renderer::ref()->tree("");
    sad::resource::Resource* maybeschema = tree->root()->resource(resource);
    if (!maybeschema)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name schema resource (sad::db::custom::Schema type)");
        return 0;
    }

    if (maybeschema->metaData()->canBeCastedTo("sad::db::custom::Schema") == false)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name schema resource (sad::db::custom::Schema type)");
        return 0;
    }

    if (sad::isAABB(rect) == false)
    {
        engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " rectangle must be axis aligned");
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
    core::Editor* c = scripting->panel()->editor();
    cmd->commit(c);
    c->currentBatchCommand()->add(cmd);

    return obj->MajorId;
}

void scripting::scenenodes::makeBackground(
    scripting::Scripting* scripting,
	sad::SceneNode* node
)
{
	scripting->panel()->sprite2DActions()->setSceneNodeAsBackground(node, false);
}

void scripting::scenenodes::remove(scripting::Scripting* scripting,  sad::SceneNode* node)
{
	scripting->panel()->sceneNodeActions()->removeSceneNode(node, false);
}
