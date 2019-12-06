#include "Camera.h"

void Camera::SetEye(vec3 eye)
{
	Eye = eye;
}

void Camera::SetLookVec(vec3 look)
{
	lookVec = look;
}

vec3 Camera::GetCamEye()
{
	return camEye;
}

vec3 Camera::GetCamLook()
{
	return camLook;
}

vec3 Camera::GetUp()
{
	return Up;
}

vec3 Camera::GetrightDir()
{
	return rightDir;
}

vec3 Camera::Gethorzfor()
{
	return horizForward;
}

void Camera::UpdateDirections()
{
	horizForward = cross(normalize(Up), normalize(rightDir));
	rightDir = cross(normalize(forwardDir), normalize(Up));
}

vec3 Camera::MouseFollow(vec3 lastMouse, vec3 mouseNow)
{
	lookDummy = glm::rotate((lastMouse.y - mouseNow.y) * 0.2f, rightDir) * lookDummy;
	lookDummy = glm::rotate((lastMouse.x - mouseNow.x) * 0.2f, Up) * lookDummy;

	forwardDir.x = lookDummy.x;
	forwardDir.y = lookDummy.y;
	forwardDir.z = lookDummy.z;

	camLook = 2.f * normalize(forwardDir);

	return camLook;
}
