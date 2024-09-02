#pragma once
#include "msk-enum.h"

namespace TexGen {
	class TgShader
	{
	private:
		unsigned m_id;
		std::string m_log;
		std::string m_type;
		std::string m_source;

	public:
		TgShader(const eShaderType& type);
		virtual ~TgShader();

		unsigned int getId() const;
		const std::string& getSource() const;
		void loadFromString(const std::string& source);
		void loadFromFile(const std::string& filename);
		void compile();
		void getError();
	};
}
