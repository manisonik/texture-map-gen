#pragma once

namespace TexGen {
	class TgRay
	{
	private:
		glm::vec3 m_direction;
		glm::vec3 m_origin;
		glm::vec3 m_dist;

	public:
		TgRay(const glm::vec3& origin);
		~TgRay();

		const glm::vec3& getDirection() const;
		void setOrigin(const glm::vec3& origin);
		void setDirection(const glm::vec3& direction);
		bool intersect(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& bary) const;
	};
}

