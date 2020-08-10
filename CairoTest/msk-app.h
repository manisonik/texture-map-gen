#pragma once
#include "msk-gui-view.h"

class MApplication : 
	public MskApp, 
	public std::enable_shared_from_this<MApplication>
{
protected:
	std::shared_ptr<MskCamera> mskCamera;
	std::shared_ptr<MskView> mskView;
	std::shared_ptr<MskFont> mskFont;
	std::shared_ptr<MskTextRenderer> mskTextRenderer;
	std::shared_ptr<MskShapeRenderer> shapeRenderer;
	std::vector<std::shared_ptr<MskControl>> m_pControls;
	GLuint m_nWidth;
	GLuint m_nHeight;
	GLfloat m_nZoom;

	// Callbacks
	void OnUpdate(MskObject * sender, UpdateEventArgs * e);
	void OnLoad(MskObject * sender, EventArgs * e);
	void OnMouseClick(MskObject * sender, EventArgs * e);
	void OnResize(MskObject * sender, ResizeEventArgs * e);
	void OnRender(MskObject * sender, EventArgs * e);
	void OnKeyDown(MskObject* sender, KeyEventArgs* e);
	void OnMouseMove(MskObject* sender, MouseEventArgs* e);
	void OnKeyUp(MskObject* sender, KeyEventArgs* e);
public:
	MApplication();
	virtual ~MApplication();

	// MskApplication Methods
	GLvoid CreateControl(std::shared_ptr<MskControl>& control);
	GLvoid CreateView(std::shared_ptr<MskView>& view);
	GLvoid CreateImage(std::shared_ptr<MskImage>& image);
	GLvoid CreateShader(const MskShaderType & type, std::shared_ptr<MskShader>& shader);
	GLvoid CreateShaderProgram(std::shared_ptr<MskShaderProgram>& shaderProgram);
	GLvoid CreateFTFont(std::string filepath, std::shared_ptr<MskFont>& font);
	GLvoid CreateTextRenderer(std::shared_ptr<MskTextRenderer>& textRenderer);
	GLvoid CreateShapeRenderer(std::shared_ptr<MskShapeRenderer>& shapeRenderer);
	GLvoid CreateCamera(std::shared_ptr<MskCamera>& mskCamera);
	GLvoid CreateTexture(std::shared_ptr<MskTexture>& mskTexture);
	GLvoid CreateTextureRenderer(std::shared_ptr<MskTextureRenderer>& mskTextureRenderer);
	GLvoid CreateFrameRenderer(std::shared_ptr<MskFrameRenderer>& mskFrameRenderer);
	GLvoid CreateMultisampleFrameRenderer(std::shared_ptr<MskMultisampleFrameRenderer>& mskFrameRenderer);
	GLvoid CreateMesh(std::shared_ptr<MskMesh>& mskMesh);
	GLvoid CreateCairo(std::shared_ptr<MskCairo>& cairo);
	GLvoid CreateLineRenderer(std::shared_ptr<MskLineRenderer>& mskLineRenderer);
	GLint  Run();
	GLuint GetWidth();
	GLuint GetHeight();
	std::shared_ptr<MskCamera> GetCamera();
};

