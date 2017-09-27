/*! \file databasebindings.h
    

    Describes bindings for editing database properties
 */
#pragma once
#include <QSet>

#include <sadstring.h>

#include <db/dbobject.h>
#include "../dukqtcontext.h"

namespace scripting
{

class Scripting;

namespace database
{
/*! Initializes invisible properties list
 */
void initializeInvisiblePropertiesList();
/*! Returns list of invisible properties
    \return reference for invisible properties
 */
const QSet<QString>& getInvisibleProperties();
/*! Adds new property
    \param[in] s scripting part
    \param[in] type a type
    \param[in] name name of property
 */
bool addProperty(scripting::Scripting* s, sad::String type, sad::String name);
/*! Removes a property from database by it's name
    \param[in] s scripting part
    \param[in] name a name for a property
 */
bool removeProperty(scripting::Scripting* s, sad::String name);
/*! Lists database properties
    \return list of strings with propeties of database
 */
QStringList list();
/*! Returns type of object
    \param[in] o object
    \return an object type
 */
sad::String type(sad::db::Object* o);
/*! Lists readableProperties for a type
    \param[in] o object 
    \return list of strings with propeties of database
 */
QStringList readableProperties(sad::db::Object* o);
/*! Lists writableProperties for a type
    \param[in] o object
    \return list of strings with propeties of database
 */
QStringList writableProperties(sad::db::Object* o);

/*! Initializes bindings for database
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created database handling object
*/
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}

