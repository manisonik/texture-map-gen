#pragma once
#include "msk-enum.h"

namespace Manisonik {
	class MShader
	{
	private:
		GLuint m_nId;
		std::string m_strLog;
		std::string m_strType;
		std::string m_strSource;

	public:
		MShader(const MskShaderType& type);
		virtual ~MShader();

		GLuint GetId();
		std::string GetSource();
		void LoadFromString(const std::string& source);
		void LoadFromFile(const std::string& filename);
		void Compile();
		void GetError();
	};
}
