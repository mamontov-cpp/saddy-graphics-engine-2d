/*! \file   hfsmshared.h
    

    Describes a shared data for state of hierarchical finite state machine
 */
#pragma once
#include "../object.h"

#include "../db/dbvariant.h"
#include "../maybe.h"

namespace sad
{

namespace hfsm
{
/*! Shared data for hierarchical finite state machine
 */
class Shared: public sad::Object
{
SAD_OBJECT
public:
    /*! Creates empty shared data
     */
    Shared();
    /*! Destroys shared data
     */
    virtual ~Shared();
    /*! Sets state variable
        \param[in] name a name for variable
        \param[in] v a value for variable
     */
    virtual void setVariable(const sad::String& name, const sad::db::Variant& v);
    /*! Returns state variable
        \param[in] name a name for variable
        \return a variable
     */
    virtual sad::Maybe<sad::db::Variant> getVariable(const sad::String& name);
protected:
    /*! A global variables for a shared state
     */
    sad::Hash<sad::String, sad::db::Variant> m_variables;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::hfsm::Shared)
