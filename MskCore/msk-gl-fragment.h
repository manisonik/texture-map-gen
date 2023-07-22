#pragma once
#include "msk-glm-camera.h"

namespace Manisonik {
	class MFragment
	{
	private:
		std::shared_ptr<MCamera> m_pCamera;
		int m_nX;
		int m_nY;
		int m_nWidth;
		int m_nHeight;

	public:
		MFragment(int x, int y, int width, int height);
		~MFragment();
		void Render();
		virtual void OnRender() = 0;
		
		std::shared_ptr<MCamera>& GetCamera();
	};
}

