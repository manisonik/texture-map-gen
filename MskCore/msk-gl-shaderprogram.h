#pragma once
#include "msk-gl-shader.h"

namespace Manisonik {
	class MShaderProgram {
	private:
		GLuint m_hProgram;
		GLuint m_nShaderCount;
		std::map<std::string, int> m_pAttributeLocations;
		std::map<std::string, int> m_pUniformLocations;

	public:
		MShaderProgram();
		virtual ~MShaderProgram();

		void AttachShader(std::shared_ptr<MShader>& shader);
		void Link();
		void Use();
		void Disable();
		int AddAttribute(const std::string name);
		int AddUniform(const std::string name);
		void SetUniform(const std::string name, float x, float y, float z);
		void SetUniform(const std::string name, const glm::vec3& v);
		void SetUniform(const std::string name, const glm::vec4& v);
		void SetUniform(const std::string name, const glm::mat3& m);
		void SetUniform(const std::string name, const glm::mat4& m);
		void SetUniform(const std::string name, float value);
		void SetUniform(const std::string name, int value);
		void SetUniform(const std::string name, bool value);
	};
}
