#include "animations/animationssavedcamerarotation.h"

sad::animations::SavedCameraRotation::SavedCameraRotation(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    sad::Scene* scene = static_cast<sad::Scene*>(o);
    sad::Camera& camera = scene->camera();
    m_angle = camera.angle();
    m_pivot = camera.temporaryRotationOffset();
    m_rotation_direction = camera.rotationVectorDirection();
}

sad::animations::SavedCameraRotation::~SavedCameraRotation()
{

}

void sad::animations::SavedCameraRotation::restore()
{
    sad::Scene* scene = static_cast<sad::Scene*>(this->m_object);
    sad::Camera& camera = scene->camera();
    camera.setAngle(m_angle);
    camera.setTemporaryRotationOffset(m_pivot);
    camera.setRotationVectorDirection(m_rotation_direction);
}
