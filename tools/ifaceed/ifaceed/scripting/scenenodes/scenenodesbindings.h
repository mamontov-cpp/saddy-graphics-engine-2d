/*! \file scenenodesbindings.h
    

    A scene nodes bindings and data
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>
#include <sadrect.h>
#include <sadcolor.h>
#include <scene.h>

#include <QVector>

namespace scripting
{

class Scripting;

namespace scenenodes
{

/*! Lists all major ids for scene nodes from all scenes
    \return a list of all major ids for all scene nodes of current window
 */
QVector<unsigned long long> list();

/*! Lists all major ids for scene nodes, contained in specified scene
    \param[in] scene a scene
    \return all major ids for scene nodes
 */
QVector<unsigned long long> listScene(sad::Scene* scene);


/*! Adds label. Prefixed by underscore, since it will be mapped to _addLabel function and addLabel
    is reserved for call, which will take object, preprocess it's fields and call _addLabel using fields of this object.
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

/*! Adds sprite. Prefixed by underscore, since it will be mapped to _addSprite2D function and addSprite2D
    is reserved for call, which will take object, preprocess it's fields and call _addSprite2D using fields of this object.
    \param[in] scripting a scripting part
    \param[in] scene a scene
    \param[in] resource a resource part
    \param[in] name a name part
    \param[in] rect a bounding rectangle
    \param[in] clr a color
    \return major id a major id for label
 */
unsigned long long _addSprite2D(
    scripting::Scripting* scripting,
    sad::Scene* scene,
    sad::String resource,
    sad::String name,
    sad::Rect2D rect,
    sad::AColor clr
);

/*! Adds custom object. Prefixed by underscore, since it will be mapped to _addCustomObject function and addCustomObject
    is reserved for call, which will take object, preprocess it's fields and call _addCustomObject using fields of this object.
    \param[in] scripting a scripting part
    \param[in] scene a scene
    \param[in] resource a resource part
    \param[in] name a name part
    \param[in] fontsize a size of font of added object, if it'll be shown as label
    \param[in] text a text of object, if it'll be shown as label
    \param[in] rect a bounding rectangle
    \param[in] clr a color
    \return major id a major id for label
 */
unsigned long long _addCustomObject(
    scripting::Scripting* scripting,
    sad::Scene* scene,
    sad::String resource,
    sad::String name,
    unsigned int fontsize,
    sad::String text,
    sad::Rect2D rect,
    sad::AColor clr
);

/*! Makes an object a background objects
    \param[in] scripting a scripting part
    \param[in] node a node
 */
void makeBackground(
    scripting::Scripting* scripting,
    sad::SceneNode* node
);

/*! Removes a scene node
    \param[in] scripting a scripting part
    \param[in] node node to be removed
 */
void remove(
    scripting::Scripting* scripting,
    sad::SceneNode* node
);

/*! Spans an object between two points
    \param[in] scripting a scripting part
    \param[in] node node to be spanned
    \param[in] p1  first point
    \param[in] p2  second point
 */
void spanBetweenTwoPoints(
    scripting::Scripting* scripting,
    sad::SceneNode* node,
    sad::Point2D p1,
    sad::Point2D p2
);

/*! Initializes bindings for scene nodes
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created scene nodes object
*/
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
