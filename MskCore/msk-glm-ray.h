#pragma once

namespace Manisonik {
	class MRay
	{
	private:
		glm::vec3 direction;
		glm::vec3 origin;

	public:
		MRay(glm::vec3 origin);
		~MRay();

		glm::vec3 GetDirection();
		GLvoid SetOrigin(glm::vec3 origin);
		GLvoid SetDirection(glm::vec3 direction);
		GLboolean Intersect(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& bary);
	};
}

