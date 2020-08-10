#include "stdafx.h"
#include "msk-core.h"
#include "msk-app.h"
#include "msk-gui-control.h"
#include "msk-gui-view.h"
#include "msk-gui-oglwindow.h"
#include "msk-gui-image.h"
#include "msk-gl-shader.h"
#include "msk-gl-shaderprogram.h"
#include "msk-gl-font.h"
#include "msk-gl-textrenderer.h"
#include "msk-gl-shaperenderer.h"
#include "msk-glm-camera.h"
#include "msk-gl-texture.h"
#include "msk-gl-texturerenderer.h"
#include "msk-gl-mesh.h"

void MskFactory::CreateApplication(std::shared_ptr<MskApp>& mskApp)
{
	mskApp = std::shared_ptr<MApplication>(new MApplication());
}

void MskFactory::CreateControl(std::shared_ptr<MskControl>& control)
{
	//control = std::shared_ptr<MControl>(new MControl());
}

void MskFactory::CreateView(std::shared_ptr<MskView>& view)
{
	//view = std::shared_ptr<MView>(new MView(this));
}

void MskFactory::CreateImage(std::shared_ptr<MskImage>& image)
{
	image = std::shared_ptr<MImage>(new MImage());
}

void MskFactory::CreateShader(const MskShaderType& type, std::shared_ptr<MskShader>& shader)
{
	shader = std::shared_ptr<MShader>(new MShader(type));
}

void MskFactory::CreateShaderProgram(std::shared_ptr<MskShaderProgram>& shaderProgram)
{
	shaderProgram = std::shared_ptr<MShaderProgram>(new MShaderProgram());
}

void MskFactory::CreateFTFont(std::string filepath, std::shared_ptr<MskFont>& font)
{
	font = std::shared_ptr<MFont>(new MFont(filepath));
}

void MskFactory::CreateTextRenderer(std::shared_ptr<MskTextRenderer>& textRenderer)
{
	textRenderer = std::shared_ptr<MTextRenderer>(new MTextRenderer());
}

void MskFactory::CreateShapeRenderer(std::shared_ptr<MskShapeRenderer>& shapeRenderer)
{
	shapeRenderer = std::shared_ptr<MShapeRenderer>(new MShapeRenderer());
}

void MskFactory::CreateCamera(std::shared_ptr<MskCamera>& mskCamera)
{
	mskCamera = std::shared_ptr<MCamera>(new MCamera());
}

void MskFactory::CreateTexture(std::string filename, std::shared_ptr<MskTexture>& mskTexture)
{
	//mskTexture = std::shared_ptr<MTexture>(new MTexture(this));
}

void MskFactory::CreateTextureRenderer(std::shared_ptr<MskTextureRenderer>& mskTextureRenderer)
{
	mskTextureRenderer = std::shared_ptr<MTextureRenderer>(new MTextureRenderer());
}

void MskFactory::CreateMesh(MskMeshes shapes, std::shared_ptr<MskMesh>& mskMesh)
{
	mskMesh = std::shared_ptr<MMesh>(new MMesh());
}