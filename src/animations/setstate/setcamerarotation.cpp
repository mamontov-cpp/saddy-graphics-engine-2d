#include "animations/setstate/setcamerarotation.h"

sad::animations::setstate::SetCameraRotation::SetCameraRotation(sad::Scene* s, const sad::Point3D& p) : m_scene(s), m_pivot(p)
{

}

sad::animations::setstate::AbstractSetStateCommand* 
sad::animations::setstate::SetCameraRotation::clone() const
{
    return new sad::animations::setstate::SetCameraRotation(m_scene, m_pivot);
}

void sad::animations::setstate::SetCameraRotation::call(const double& angle)
{
	sad::Camera& c = m_scene->camera();
	c.Angle = angle;
	c.RotationVectorDirection = sad::Vector3D(0, 0, 1);
	c.TemporaryRotationOffset = m_pivot;
}

sad::animations::setstate::SetCameraRotation::~SetCameraRotation()
{

}
