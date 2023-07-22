#pragma once
#include "msk-core.h"

class MSceneFileMenu :
	public MskScene
{
private:
	std::shared_ptr<MskStencilRenderer> m_pStencilRenderer;
	std::shared_ptr<MskCamera> m_pCamera;
	std::shared_ptr<MskCamera> m_pSceneCamera;
	std::shared_ptr<MskShapeRenderer> m_pShapeRenderer;
	std::shared_ptr<MskTextRenderer> m_pTextRenderer;
	std::shared_ptr<MskTextureRenderer> m_pTextureRenderer;
	std::shared_ptr<MskLineRenderer> m_pLineRenderer;
	std::shared_ptr<MskTextureRenderer> m_pScene;
	std::shared_ptr<MskTexture> m_pTexture;
	std::shared_ptr<MskTexture> m_pCliffDiffuse;
	std::shared_ptr<MskTexture> m_pCliffNormal;
	std::shared_ptr<MskTexture> m_pCliffHeight;
	std::shared_ptr<MskFont> m_pFont;
	std::shared_ptr<MskMesh> m_pMeshCyclinder;
	std::shared_ptr<MskMesh> m_pMeshFileMenu;
	std::shared_ptr<MskMesh> m_pMeshSideWindow;
	std::shared_ptr<MskMesh> m_pMeshGrid;
	std::shared_ptr<MskMesh> m_pMeshButton;
	std::shared_ptr<MskMesh> m_pMeshCube;
	std::shared_ptr<MskCairo> m_pCairo;
	std::vector<std::shared_ptr<MskMesh>> m_vButtons;
	std::shared_ptr<MskRay> m_pRay;

	GLuint m_bHitTest;
	GLfloat m_nWidth;
	GLfloat m_nHeight;

	struct FileMenu {
		std::string text;
		glm::uvec2 pos;
		glm::uvec2 size;
		std::vector<FileMenu> nodes;
	};

	GLvoid InitMesh();
protected:
	// Register callbacks
	void OnRender(MskObject* sender, EventArgs* e);
	void RenderPass(int pass);
	void OnResize(MskObject* sender, ResizeEventArgs* e);
	void OnUpdate(MskObject* sender, UpdateEventArgs* e);
	void OnKeyUp(MskObject* sender, KeyEventArgs* e);
	void OnMouseClick(MskObject* sender, MouseEventArgs* e);
	void OnMouseMove(MskObject* sender, MouseEventArgs* e);

public:
	MSceneFileMenu(MskApp* mskApp);
	~MSceneFileMenu();
};

