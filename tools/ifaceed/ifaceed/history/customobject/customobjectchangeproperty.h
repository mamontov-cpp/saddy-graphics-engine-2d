/*! \file customobjectchangeproperty.h
    

    Describes a command, which defines changing property of a custom object
 */
#pragma once
#include "../history/scenenodes/scenenodeschangeproperty.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/customobjectactions.h"

namespace history
{
    
namespace customobject
{

/*! Describes a command, which defines changing property of a custom object
 */
template<typename T>
class ChangeProperty: public history::scenenodes::ChangeProperty<T>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] property a property name
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeProperty(
        sad::SceneNode* d,
        const sad::String& property,
        const T& old_value,
        const T& new_value
    )
    :  history::scenenodes::ChangeProperty<T>(d, property, old_value, new_value)
    {
        
    }
    /*! Can be inherited
     */
    virtual ~ChangeProperty()
    {
        
    }
protected:
     /*! Updates a main panel to current value
         \param[in] e editor
         \param[in] value a value for element
      */
     void updateUI(core::Editor* e, const T& value) override
     {
         e->actions()->customObjectActions()->updateCustomObjectPropertyValue(
             this->history::scenenodes::ChangeProperty<T>::m_node,
             this->history::scenenodes::ChangeProperty<T>::m_property,
             sad::db::Variant(value)
        );
     }
};

}

}
