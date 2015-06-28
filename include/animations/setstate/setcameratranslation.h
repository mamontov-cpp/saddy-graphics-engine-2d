/*! \file animations/setstate/setcameratranslation.h
    \author HiddenSeeker

    Defines a comand, which translates camera around a pivot
 */
#pragma  once
#include "typedcommand.h"

#include "../../camera.h"
#include "../../scene.h"
#include "../../sadpoint.h"

namespace sad
{

namespace animations
{

namespace setstate
{

class SetCameraTranslation: public sad::animations::setstate::TypedCommmand<sad::Point3D>
{
public:
    /*! Sets a scene and pivot for translation
        \param[in] s scene
     */
    SetCameraTranslation(sad::Scene* s);
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const;
    /*! Invokes a command with specified command
        \param[in] offset a specified offset
     */
    virtual void call(const sad::Point3D& offset);
    /*! Must be inherited
     */
    virtual ~SetCameraTranslation();
protected:
    /*! A scene, which should be used
     */
    sad::Scene* m_scene;
};

}

}

}
