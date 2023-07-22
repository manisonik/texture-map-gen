#pragma once
#include "stdafx.h"
#include "msk-object.h"
#include "msk-gl-multisampleframerenderer.h"
#include "msk-structs.h"
#include "msk-event.h"
#include "msk-eventargs.h"
#include "msk-gl-scene.h"

namespace Manisonik {
	class MskApp : public MObject
	{
	private:
		HDC m_hDC;
		HGLRC m_hRC;
		GLuint m_nPixelFormat;
		HWND m_hWnd;
		HINSTANCE m_hInstance;
		HICON m_hIcon;
		BOOL m_bMouseDown;
		std::vector<std::shared_ptr<MScene>> m_pScenes;
		std::shared_ptr<MMultisampleFrameRenderer> m_pMSAA;
	public:
		MskApp();
		virtual ~MskApp();

		void AddScene(std::shared_ptr<MScene> scene);
		void Run();
		void Update(float dt);
		void Render();

	protected:
		void RegisterWindow();
		void SetupWindow();
		bool SetupOpenGL();
		bool InitializeGL();
		static LRESULT CALLBACK RouterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		virtual LRESULT CALLBACK OnProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	};
}

