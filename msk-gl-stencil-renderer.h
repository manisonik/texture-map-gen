#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-mesh.h"

namespace TexGen {
	class TgStencilRenderer
	{
	private:
		bool m_drawing;
	public:
		TgStencilRenderer();
		~TgStencilRenderer();

		void begin();
		void draw(const std::function<void(int)>& renderPass);
		void end();
	};
}

