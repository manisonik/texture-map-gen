#include "stdafx.h"
#include "msk-glm-ray.h"
#include "msk-glm-camera.h"

MRay::MRay(glm::vec3 origin)
{
	this->origin = origin;
}

MRay::~MRay()
{
}

glm::vec3 MRay::GetDirection()
{
	return direction;
}

GLvoid MRay::SetOrigin(glm::vec3 origin)
{
	this->origin = origin;
}

GLvoid MRay::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
}

GLboolean MRay::Intersect(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& bary)
{
	return glm::intersectRayTriangle(origin, direction, v0, v1, v2, bary);
}
