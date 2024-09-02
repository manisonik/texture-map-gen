#pragma once
#include "stdafx.h"
#include "msk-object.h"
#include "msk-gl-multisampleframerenderer.h"
#include "msk-structs.h"
#include "msk-gl-scene.h"

namespace TexGen {
	class Tg : public TgObject
	{
	private:
		std::vector<std::shared_ptr<TgScene>> m_pScenes;
		//std::unique_ptr<TgMultisampleFrameRenderer> m_pMSAA;
		std::unique_ptr<ITgWindow> m_window;
	public:
		Tg(std::unique_ptr<ITgWindow>&& window);
		virtual ~Tg();

		void addScene(const std::shared_ptr<ITgScene>& scene);
		void run();
		void update(float dt);
		void render();
	};
}

