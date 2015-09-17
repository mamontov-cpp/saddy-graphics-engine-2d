#include "animations/animationssavedcamerarotation.h"

sad::animations::SavedCameraRotation::SavedCameraRotation(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    sad::Scene* scene = static_cast<sad::Scene*>(o);
    sad::Camera& camera = scene->camera();
    m_angle = camera.Angle;
    m_pivot = camera.TemporaryRotationOffset;
    m_rotationdirection = camera.RotationVectorDirection;
}

sad::animations::SavedCameraRotation::~SavedCameraRotation()
{

}

void sad::animations::SavedCameraRotation::restore()
{
    sad::Scene* scene = static_cast<sad::Scene*>(this->m_object);
    sad::Camera& camera = scene->camera();
    camera.Angle = m_angle;
    camera.TemporaryRotationOffset = m_pivot;
    camera.RotationVectorDirection = m_rotationdirection;
}
