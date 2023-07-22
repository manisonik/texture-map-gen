#pragma once
#include "msk-structs.h"
#include "msk-gl-fragment.h"
#include "msk-event.h"
#include "msk-eventargs.h"
#include "msk-glm-camera.h"

namespace Manisonik {
	class MScene : public MObject
	{
	private:
		std::vector<std::shared_ptr<MFragment>> m_pFragmentList;
		std::shared_ptr<MCamera> m_pSceneCamera;

	public:
		int m_nX;
		int m_nY;
		int m_nWidth;
		int m_nHeight;

		MScene(int x, int y, int width, int height);
		virtual ~MScene();

		// Events
		MEvent<MKeyEventArgs> KeyDownEvent;
		MEvent<MKeyEventArgs> KeyUpEvent;
		// Mouse events
		MEvent<MMouseEventArgs> MouseClickEvent;
		MEvent<MMouseEventArgs> MouseDownEvent;
		MEvent<MMouseEventArgs> MouseUpEvent;
		MEvent<MMouseEventArgs> MouseMoveEvent;
		// Resize events
		MEvent<MResizeEventArgs> ResizeEvent;

		void Render();
		void Update(float dt);
		void Initialize();
		void AddFragment(const std::shared_ptr<MFragment>& fragment);

		std::shared_ptr<MCamera>& GetCamera();


	protected:
		virtual void OnInitialize() = 0;
		virtual void OnRender() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnKeyDown(MObject* sender, MKeyEventArgs* e) = 0;
		virtual void OnKeyUp(MObject* sender, MKeyEventArgs* e) = 0;
		virtual void OnMouseClick(MObject* sender, MMouseEventArgs* e) = 0;
		virtual void OnMouseDown(MObject* sender, MMouseEventArgs* e) = 0;
		virtual void OnMouseUp(MObject* sender, MMouseEventArgs* e) = 0;
		virtual void OnMouseMove(MObject* sender, MMouseEventArgs* e) = 0;
		virtual void OnResize(MObject* sender, MResizeEventArgs* e) = 0;
	};
}

