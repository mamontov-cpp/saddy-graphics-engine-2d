#include "animations/setstate/setcameratranslation.h"

sad::animations::setstate::SetCameraTranslation::SetCameraTranslation(sad::Scene* s) : m_scene(s)
{

}


sad::animations::setstate::AbstractSetStateCommand* 
sad::animations::setstate::SetCameraTranslation::clone() const
{
    return new sad::animations::setstate::SetCameraTranslation(m_scene);
}

void sad::animations::setstate::SetCameraTranslation::call(const sad::Point3D& offset)
{
    sad::Camera& c = m_scene->camera();
    c.setTranslationOffset(offset);
}

sad::animations::setstate::SetCameraTranslation::~SetCameraTranslation()
{

}

