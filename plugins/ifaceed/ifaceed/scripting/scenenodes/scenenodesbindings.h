/*! \file scenenodesbindings.h
	\author HiddenSeeker

	A scene nodes bindings and data
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>
#include <sadcolor.h>
#include <scene.h>

#include <QScriptEngine>
#include <QVector>

namespace scripting
{

class Scripting;

namespace scenenodes
{

/*! Lists a scene node
	\param[in] ctx context
	\param[in] engine an enginge
	\return a scene nodes list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

/*! Lists a scene node from a scene
	\param[in] scripting a scripting part
	\param[in] scene a scene
	\return a scene nodes list
 */
QVector<unsigned long long> listScene(scripting::Scripting* scripting, sad::Scene* scene);


/*! Adds label. Prefixed by underscore, since it will be mapped to _addLabel function and addLabel
	is reserved for more hard object
	\param[in] scripting a scripting part
	\param[in] scene a scene
	\param[in] resource a resource part
	\param[in] fontsize a font size
	\param[in] text a text part
	\param[in] name a name part
	\param[in] topleftpoint a topleft point point
	\param[in] clr a color
	\return major id a major id for label
 */
unsigned long long _addLabel(
	scripting::Scripting* scripting, 
	sad::Scene* scene,
	sad::String resource, 
	unsigned int fontsize,
	sad::String text, 
	sad::String name,
	sad::Point2D topleftpoint,
	sad::AColor clr
);

}

}
