#pragma once
#include "msk-gl-shaderprogram.h"
#include "msk-gl-texture.h"

namespace Manisonik {
	class MMultisampleFrameRenderer
	{
	private:
		// Shaders
		std::shared_ptr<MShader> m_pVertexShader;
		std::shared_ptr<MShader> m_pFragmentShader;
		std::shared_ptr<MShaderProgram> m_pShaderProgram;
		std::shared_ptr<MTexture> m_pTexture;
		std::vector<glm::vec3> m_vVertexData;
		std::vector<uint32_t> m_vIndexData;
		std::vector<glm::vec2> m_vTexCoords;
		GLuint m_hRbo;
		GLuint m_hFboColor;
		GLuint m_hFboMSAA;
		GLuint m_hFboDepth;
		GLuint m_hFbo;
		GLuint m_hDepthBuffer;
		GLint m_nMaxSamples = 0;
		GLsizei m_nWidth = 1024;
		GLsizei m_nHeight = 1024;
		GLsizei m_nSamples = 4;
		GLuint m_hVAO;
		GLuint m_hTBO;
		GLuint m_hVBO;
		GLuint m_hIBO;

		void InitTextures();
		void InitFrameBuffer();
		void InitShaders();
		void InitBuffers();
		void FillBuffers();

	public:
		MMultisampleFrameRenderer();
		virtual ~MMultisampleFrameRenderer();

		GLvoid Begin();
		GLvoid End();
		GLvoid Resize(GLsizei width, GLsizei height);
		std::shared_ptr<MTexture> GetTexture();
	};
}

