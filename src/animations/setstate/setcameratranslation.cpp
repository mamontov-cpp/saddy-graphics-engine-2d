#include "animations/setstate/setcameratranslation.h"

sad::animations::setstate::SetCameraTranslation::SetCameraTranslation(sad::Scene* s) : m_scene(s)
{

}

void sad::animations::setstate::SetCameraTranslation::call(const sad::Point3D& offset)
{
    sad::Camera& c = m_scene->camera();
    c.TranslationOffset = offset;
}

sad::animations::setstate::SetCameraTranslation::~SetCameraTranslation()
{

}

