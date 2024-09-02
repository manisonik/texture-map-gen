#pragma once
#include "msk-glm-camera.h"

namespace TexGen {
	class TgFragment
	{
	private:
		TgCamera m_camera;
		int m_x;
		int m_y;
		int m_width;
		int m_height;

	public:
		TgFragment(int x, int y, int width, int height);
		~TgFragment();
		void render();
		virtual void onRender() = 0;
		
		const TgCamera& getCamera() const;
	};
}

