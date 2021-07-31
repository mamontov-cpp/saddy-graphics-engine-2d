/*! \file updateelement.h
    

    Defines smart update for element of UI from object property
 */
#pragma once
#include "../blockedclosuremethodcall.h"

#include <sadstring.h>
#include <scenenode.h>

namespace gui
{

/*! \class UpdateElement
    Updates ui, using properties from specified object
 */
template<
    typename _PropertyType
>
class UpdateElement
{
   public:
    /*! Updates  element of UI
        \param[in] o an object, which has a property
        \param[in] property_name a property name
        \param[in] w widget, which must be updated
        \param[in] f a method to update widget
     */
    template<
        typename _WidgetType,
        typename _Method
    >
    static void with(
       sad::SceneNode* o,
       const sad::String& property_name,
       _WidgetType* w,
       _Method f
    )
    {
        sad::Maybe<_PropertyType> val = o->getProperty<_PropertyType>(property_name);
        if (val.exists())
        {
            invoke_blocked(w, f, val.value());
        }
    }
};

}
