#include "scenenodesbindings.h"

#include "../querytable.h"
#include "../scripting.h"

#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/scenenodes/scenenodesnew.h"

#include <renderer.h>
#include <label.h>

#include <resource/tree.h>
#include <resource/resource.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>


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
		engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name resource");
		return 0;
	}

	if (maybefont->metaData()->canBeCastedTo("sad::Font") == false 
		&& maybefont->metaData()->canBeCastedTo("sad::freetype::Font") == false)
	{
		engine->currentContext()->throwError(QScriptContext::TypeError, QString("_addLabel: ") + resource.c_str() + " does not name resource");
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

