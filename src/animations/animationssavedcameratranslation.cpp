#include "animations/animationssavedcameratranslation.h"

sad::animations::SavedCameraTranslation::SavedCameraTranslation(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    sad::Scene* scene = static_cast<sad::Scene*>(o);
    sad::Camera& camera = scene->camera();
    m_translation = camera.translationOffset();
}

sad::animations::SavedCameraTranslation::~SavedCameraTranslation()
{

}

void sad::animations::SavedCameraTranslation::restore()
{
    sad::Scene* scene = static_cast<sad::Scene*>(this->m_object);
    sad::Camera& camera = scene->camera();
    camera.setTranslationOffset(m_translation);
}
