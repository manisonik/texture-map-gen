#include "stdafx.h"
#include "msk-glm-ray.h"

using namespace TexGen;

TgRay::TgRay(const glm::vec3& origin) :
	m_origin(origin)
{
}

TgRay::~TgRay()
{
}

const glm::vec3& TgRay::getDirection() const
{
	return m_direction;
}

void TgRay::setOrigin(const glm::vec3& origin)
{
	m_origin = origin;
}

void TgRay::setDirection(const glm::vec3& direction)
{
	m_direction = direction;
}

bool TgRay::intersect(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& bary) const
{
	//return glm::intersectRayTriangle(m_origin, m_direction, v0, v1, v2, bary, dist);
	return false;
}
