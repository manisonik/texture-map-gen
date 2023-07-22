#pragma once
#include "msk-core.h"
#include "msk-gl-framerenderer.h"

using namespace Gdiplus;

class MView : 
	public MskView
{
protected:
	GLboolean m_bMouseDown;
	HWND m_hWnd;
	HWND m_hWndDropTarget;
	INT m_nWidth;
	INT m_nHeight;
	Rect m_rcMargins;
	RECT m_AdjustedClientRect;
	BOOL m_isLoaded;
	DWORD m_dwStyle;
	DWORD m_dwStyleEx;
	DWORD m_nDockFlags;
	HICON m_hIcon;
	HBRUSH m_hBackground;
	HCURSOR m_hCursor;
	Rect m_rcBounds;
	CREATESTRUCT m_CS;
	Point m_ptLocation;
	HINSTANCE m_hInstance;
	HDC m_hDC;
	HGLRC m_hRC;
	GLuint m_nPixelFormat;
	MView* m_pParent;
	EventArgs* eventArgs;
	glm::mat4 m_matProj;
	MskApp* mskApp;
	std::string m_strText;
	std::string m_strClassName;
	std::string m_strMenuName;
	std::vector<std::shared_ptr<MskControl>> m_pControls;
	std::vector<std::shared_ptr<MskScene>> m_pScene;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;
	std::shared_ptr<MskFont> mskFont;
	std::shared_ptr<MskTextRenderer> mskTextRenderer;
	std::shared_ptr<MskTextureRenderer> mskTextureRenderer;
	std::shared_ptr<MskShapeRenderer> mskShapeRenderer;
	std::shared_ptr<MskLineRenderer> mskLineRenderer;
	std::shared_ptr<MskMesh> mskCubeMesh;
	std::shared_ptr<MskMesh> mskQuad;
	std::shared_ptr<MskMesh> mskMeshGrid;
	std::shared_ptr<MskTexture> mskTexture;
	std::shared_ptr<MskCamera> mskCamera;
	std::shared_ptr<MskFrameRenderer> mskFrameRenderer;
	std::shared_ptr<MskMultisampleFrameRenderer> mskSSAA;

public:
	MView(MskApp* mskApp);
	~MView();

	void OnResize(MskObject * sender, ResizeEventArgs * e);

	// Callbacks
	void OnDestroy(MskObject * sender, EventArgs * e);
	void OnRender(MskObject * sender, EventArgs * e);
	void OnUpdate(MskObject* sender, UpdateEventArgs* e);

	// MskView Methods
	int AddControl(std::shared_ptr<MskControl>& control);
	GLvoid AddScene(const std::shared_ptr<MskScene>& scene);
	void ClearScreen();
	HDC GetDeviceContext();
	void SwapBuffers();
	void MakeCurrent();
	void Show();
	void Hide();

	// MView Methods
	void RegisterWindow();
	GLboolean InitializeGL(GLvoid);
	void SetupOpenGL();
	static LRESULT CALLBACK RouterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK OnProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
};

