#pragma once
#include "msk-gl-shader.h"

namespace TexGen {
	class TgShaderProgram {
	private:
		unsigned int m_program;
		unsigned int m_shaderCount;
		std::map<std::string, int> m_attributeLocations;
		std::map<std::string, int> m_uniformLocations;

	public:
		TgShaderProgram();
		virtual ~TgShaderProgram();

		void attachShader(const TgShader& shader);
		void link();
		void use();
		void disable();
		int addAttribute(const std::string& name);
		int addUniform(const std::string& name);
		void setUniform(const std::string& name, float x, float y, float z);
		void setUniform(const std::string& name, const glm::vec3& v);
		void setUniform(const std::string& name, const glm::vec4& v);
		void setUniform(const std::string& name, const glm::mat3& m);
		void setUniform(const std::string& name, const glm::mat4& m);
		void setUniform(const std::string& name, float value);
		void setUniform(const std::string& name, int value);
		void setUniform(const std::string& name, bool value);
	};
}
