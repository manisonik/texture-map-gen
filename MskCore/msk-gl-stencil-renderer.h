#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-mesh.h"

namespace Manisonik {
	class MStencilRenderer
	{
		GLboolean m_bDrawing;
	public:
		MStencilRenderer();
		~MStencilRenderer();

		void Begin();
		void Draw(const std::function<void(int)>& renderPass);
		void End();
	};
}

