#pragma once
#include "msk-core.h"

class MStencilRenderer : 
	public MskStencilRenderer
{
private:
	MskApp* m_pMskApp;

	// Shaders
	std::shared_ptr<MskShader> m_pVertexShader;
	std::shared_ptr<MskShader> m_pFragmentShader;
	std::shared_ptr<MskShaderProgram> m_pShaderProgram;

	GLboolean m_bDrawing;

	GLvoid InitShaders();
public:
	MStencilRenderer(MskApp* mskApp);
	~MStencilRenderer();

	void Begin();
	void Draw(std::function<void(int)> renderPass);
	void End();
	void Flush();
	void Draw(std::shared_ptr<MskMesh>& mesh);
	void SetProjectionMatrix(glm::mat4& matProj);
};

