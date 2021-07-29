/*! \file animations\animationssavedcameratranslation.h
    

    Defines saved camera translation settings as saved element
 */
#pragma once
#include "animations/animationssavedobjectstate.h"

#include "../scene.h"
#include "../camera.h"


namespace sad
{

namespace animations
{

/*! A saved camera translation settings for scene
 */
class  SavedCameraTranslation: public sad::animations::SavedObjectState
{
public:
    /*! Creates a new object and sets a scene
        \param[in] o scene, which settings will be saved
     */
    SavedCameraTranslation(sad::db::Object* o);
    /*! Can be inherited
     */
    virtual ~SavedCameraTranslation() override;
    /*! Restores last object state
     */
    virtual void restore() override;
protected:
    /*! An old pivot for translation vector
     */
    sad::Vector3D m_translation;
};

}

}
