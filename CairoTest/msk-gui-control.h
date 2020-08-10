#pragma once
#include "msk-core.h"
#include "msk-gl-framerenderer.h"

class MControl : 
	public MskControl
{
protected:
	HWND m_hWnd;
	glm::mat4 m_matProj;
	std::shared_ptr<MskShapeRenderer> shapeRenderer;
	std::shared_ptr<MskTextureRenderer> textureRenderer;
	std::shared_ptr<MskFont> mskFont;
	std::shared_ptr<MskTextRenderer> mskTextRenderer;
	std::shared_ptr<MskMesh> meshCube;
	std::shared_ptr<MskCamera> mskCamera;
	std::shared_ptr<MskTexture> mskTexture;

public:
	MControl(MskApp* mskApp);
	~MControl();

	// Register callbacks
	void OnRender(MskObject * sender, EventArgs * e);
	void OnResize(MskObject * sender, ResizeEventArgs * e);
	void OnUpdate(MskObject* sender, UpdateEventArgs* e);
};

