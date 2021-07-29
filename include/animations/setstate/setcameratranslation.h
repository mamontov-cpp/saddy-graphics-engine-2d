/*! \file animations/setstate/setcameratranslation.h
    

    Defines a command, which translates camera around a pivot
 */
#pragma  once
#include "typedcommand.h"

#include "../../camera.h"
#include "../../sadpoint.h"

namespace sad
{

namespace animations
{

namespace setstate
{

class SetCameraTranslation: public sad::animations::setstate::TypedCommand<sad::Point3D>
{
public:
    /*! Sets a scene and pivot for translation
        \param[in] s scene
     */
    SetCameraTranslation(sad::Scene* s);
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const override;
    /*! Invokes a command with specified command
        \param[in] offset a specified offset
     */
    virtual void call(const sad::Point3D& offset) override;
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
