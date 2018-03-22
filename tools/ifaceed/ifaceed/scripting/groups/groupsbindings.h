/*! \file groupsbindings.h
    

    A bindings for animation instances are listed here
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>
#include <maybe.h>

#include <QVector>


namespace scripting
{

class Scripting;

namespace groups
{

/*! Lists of major ids for all animation groups in database
    \return major ids for all animation groups in database
 */
QVector<unsigned long long> list();

/*! Adds group. Prefixed by underscore, since it will be mapped to _add function and add
    is reserved for call, which will take object, preprocess it's fields and call _add using fields of this object.
    \param[in] scripting a scripting part
    \param[in] name a name for way
    \param[in] looped whether group is looped
    \return id of list
 */ 
unsigned long long _add(
    scripting::Scripting* scripting, 
    sad::String name,
    bool looped
);


/*! Removes animation group
    \param scripting a scripting part
    \param group a group to be removed
 */
void remove(
    scripting::Scripting* scripting,
    sad::animations::Group* group
);

/*! Returns count of animation instances in group
    \param[in]  scripting a scripting part
    \param[in]  group a group item
 */
unsigned int length(
    scripting::Scripting* scripting,
    sad::animations::Group* group	
);

/*! Returns instance from list (0 if not found)
    \param[in]  scripting a scripting part
    \param[in]  group a group item
    \param[in]  pos position
    \return major id of instance
 */
unsigned long long entry(
    scripting::Scripting* scripting,
    sad::animations::Group* group,
    unsigned int pos
);

/*! Tries to add instance to a group
    of group. Returns true on success, false
    if animation is already in group
    \param[in]  scripting a scripting part
    \param[in] group a group
    \param[in] minstance an instance to be added
    \return true on success
 */
bool addInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,
    sad::animations::Instance* minstance
);

/*! Tries to remove instance from a group
    of group. Returns true on success, false
    if position is not valid
    \param[in]  scripting a scripting part
    \param[in] group a group
    \param[in] pos position of instace in group
    \return true on success
 */
bool removeInstance(
    scripting::Scripting* scripting,
    sad::animations::Group* group,
    unsigned int pos
);

/*! Sets readable or writable properties for animation groups
    \param[in] obj object
    \param[out] list a list of properties
    \param[in] readable a readable or writable properties
 */
void checkProperties(
    const sad::Maybe<sad::db::Object*>& obj,
    QStringList& list,
    bool readable
);

/*! Initializes bindings for animation instances
    \param[in] s scripting object
    \param[in] animations a global binding object for animations
    \return created animations object
 */
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* animations);

}

}
