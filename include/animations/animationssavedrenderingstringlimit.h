/*! \file animations\animationssavedrenderingstringlimit.h
    

    Defines saved rendering string limit settings as element, that can be restored, when animation is cancelled.
    Note that it does not actually stores anything and just used to define, how this setting should be restored.
 */
#pragma once
#include "animations/animationssavedobjectstate.h"

#include "../scene.h"


namespace sad
{

namespace animations
{

/*! A saved camera rotation settings for scene
 */
class  SavedRenderingStringLimit: public sad::animations::SavedObjectState
{
public:
    /*! Creates a new object and sets a scene
        \param[in] o scene, which settings will be saved
     */
    SavedRenderingStringLimit(sad::db::Object* o);
    /*! Can be inherited
     */
    virtual ~SavedRenderingStringLimit() override;
    /*! Restores last object state
     */
    virtual void restore() override;
protected:
    /*! Whether saved object is label
     */
    bool m_is_label;
};

}

}
