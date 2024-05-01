#include "animations/setstate/setcameratranslation.h"
#include "scene.h"
#include "opticksupport.h"

sad::animations::setstate::SetCameraTranslation::SetCameraTranslation(sad::Scene* s) : m_scene(s)
{
    PROFILER_EVENT;

}


sad::animations::setstate::AbstractSetStateCommand* 
sad::animations::setstate::SetCameraTranslation::clone() const
{
    PROFILER_EVENT;
    return new sad::animations::setstate::SetCameraTranslation(m_scene);
}

void sad::animations::setstate::SetCameraTranslation::call(const sad::Point3D& offset)
{
    PROFILER_EVENT;
    sad::Camera& c = m_scene->camera();
    c.setTranslationOffset(offset);
}

sad::animations::setstate::SetCameraTranslation::~SetCameraTranslation()
{
    PROFILER_EVENT;

}
