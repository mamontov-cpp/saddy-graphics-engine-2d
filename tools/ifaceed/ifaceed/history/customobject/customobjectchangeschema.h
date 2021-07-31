/*! \file customobjectchangeschema.h
    

    A command, which is used to change scheme of custom object
 */
#pragma once
#include <sadptrhash.h>

#include <db/dbvariant.h>

#include <db/custom/customobject.h>

#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{
    
namespace customobject
{
    
/*! \class ChangeSchema

    Defines change of scheme name for an object
 */
class ChangeSchema: public history::scenenodes::ChangeProperty<sad::String>
{
public:
      /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeSchema(
        sad::db::custom::Object* d,
        const sad::String& old_value,
        const sad::String& new_value
    );
    /*! Erases link to a node
      */
    virtual ~ChangeSchema();
    /*! Restores old values of properties for scene  node
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Old area for scene node
     */
    sad::Rect2D m_old_area;
    /*! Old values for properties of object
     */
    sad::PtrHash<sad::String, sad::db::Variant> m_old_values;
    /*!
     * Update screen properties and values, according to new schema
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const sad::String& value) override;
};

}

}
