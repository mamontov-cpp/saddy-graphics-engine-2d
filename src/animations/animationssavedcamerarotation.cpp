#include "animations/animationssavedcamerarotation.h"
#include "opticksupport.h"

sad::animations::SavedCameraRotation::SavedCameraRotation(sad::db::Object* o)
: sad::animations::SavedObjectState(o)
{
    PROFILER_EVENT;
    sad::Scene* scene = static_cast<sad::Scene*>(o);
    sad::Camera& camera = scene->camera();
    m_angle = camera.angle();
    m_pivot = camera.temporaryRotationOffset();
    m_rotation_direction = camera.rotationVectorDirection();
}

sad::animations::SavedCameraRotation::~SavedCameraRotation()
{
    PROFILER_EVENT;

}

void sad::animations::SavedCameraRotation::restore()
{
    PROFILER_EVENT;
    sad::Scene* scene = static_cast<sad::Scene*>(this->m_object);
    sad::Camera& camera = scene->camera();
    camera.setAngle(m_angle);
    camera.setTemporaryRotationOffset(m_pivot);
    camera.setRotationVectorDirection(m_rotation_direction);
}
