#pragma once
#include "msk-structs.h"
#include "msk-object.h"
#include "msk-gl-fragment.h"
#include "msk-glm-camera.h"

namespace TexGen {
	class TgScene : public ITgScene
	{
	private:
		std::vector<std::shared_ptr<TgFragment>> m_fragmentList;
		TgCamera m_sceneCamera;

	public:
		int m_x;
		int m_y;
		int m_width;
		int m_height;

		TgScene(int x, int y, int width, int height);
		virtual ~TgScene();

		void render() override;
		void update(float dt) override;
		void addFragment(std::shared_ptr<TgFragment> fragment);
		TgCamera& getCamera();
		
	protected:
		virtual void onRender() = 0;
		virtual void onUpdate(float dt) = 0;

		//void onResize(int width, int height);
        //void onMouseUp(int x, int y, eMouseButton button);
        //void onMouseDown(int x, int y, eMouseButton button);
        //void onMouseOver(int x, int y, eMouseButton button);
        //void onMouseClick(int x, int y, eMouseButton button);
        //void onMouseMove(int x, int y, eMouseButton button);
        //void onKeyUp(eKeys keyCode);
        //void onKeyDown(eKeys keyCode);
	};
}

