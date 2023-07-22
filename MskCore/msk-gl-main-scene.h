#pragma once
#include "msk-gl-scene.h"
#include "msk-gl-line-renderer.h"
#include "msk-gl-renderer.h"
#include "msk-gl-mesh.h"
#include "msk-glm-ray.h"
#include "msk-glm-camera.h"
#include "msk-glm-mousepicker.h"
#include "msk-gl-stencil-renderer.h"
#include "msk-gl-mesh-builder.h"
#include "msk-gl-spritebatch.h"
#include "msk-gl-text-renderer.h"

namespace Manisonik {
    class MMainScene :
        public MScene
    {
    private:
        std::shared_ptr<MRenderer> m_pRenderer;
        std::shared_ptr<MLineRenderer> m_pLineRenderer;
        std::shared_ptr<MMesh> m_pMeshIcoSphere;
        std::shared_ptr<MMesh> m_pMeshSphere;
        std::shared_ptr<MMesh> m_pMeshCylinder;
        std::shared_ptr<MMesh> m_pMeshCube;
        std::shared_ptr<MMesh> m_pMeshTest;
        std::shared_ptr<MMesh> m_pGrid;
        std::shared_ptr<MRay> m_pRay;
        std::shared_ptr<MCamera> m_pSceneCamera;
        std::shared_ptr<MCamera> m_pCameraUI;
        std::shared_ptr<MTextRenderer> m_pTextRenderer;
        std::shared_ptr<MFont> m_pFont;
        std::shared_ptr<MMousePicker> m_pMousePicker;
        std::shared_ptr<MStencilRenderer> m_pStencilRenderer;
        std::shared_ptr<MSpriteBatch> m_pSpriteBatch;
        std::vector<std::shared_ptr<MTextureRegion>> m_pTexRgn;
        std::shared_ptr<MTexture> m_pTexture;
        std::function<void(int)> m_pStencilFunc;
    public:
        MMainScene(int x, int y, int width, int height);
        ~MMainScene();

        void RenderPass(int pass);

        void OnResize(MObject* sender, MResizeEventArgs* e);
        void OnMouseUp(MObject* sender, MMouseEventArgs* e);
        void OnMouseOver(MObject* sender, MMouseEventArgs* e);
        void OnMouseDown(MObject* sender, MMouseEventArgs* e);
        void OnMouseClick(MObject* sender, MMouseEventArgs* e);
        void OnMouseMove(MObject* sender, MMouseEventArgs* e);
        void OnKeyUp(MObject* sender, MKeyEventArgs* e);
        void OnKeyDown(MObject* sender, MKeyEventArgs* e);
        void OnRender();
        void OnUpdate(float dt);
        void OnInitialize();
    };
}

