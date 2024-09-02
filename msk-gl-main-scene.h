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

namespace TexGen {
    class TgMainScene : public TgScene
    {
    private:
        TgRenderer m_renderer;
        TgLineRenderer m_lineRenderer;
        TgMesh m_meshIcoSphere;
        TgMesh m_meshSphere;
        TgMesh m_meshCylinder;
        TgMesh m_meshCube;
        TgMesh m_meshTest;
        TgMesh m_grid;
        TgRay m_ray;
        TgCamera m_sceneCamera;
        TgCamera m_cameraUI;
        TgTextRenderer m_textRenderer;
        TgFont m_font;
        TgMousePicker m_mousePicker;
        TgStencilRenderer m_stencilRenderer;
        TgSpriteBatch m_spriteBatch;
        std::vector<std::shared_ptr<TgTextureRegion>> m_texRgn;
        TgTexture m_texture;
        std::function<void(int)> m_stencilFunc;
    public:
        TgMainScene(int x, int y, int width, int height);
        ~TgMainScene();

        void renderPass(int pass);

        void onResize(int width, int height);
        void onMouseUp(int x, int y, eMouseButton button);
        void onMouseDown(int x, int y, eMouseButton button);
        void onMouseOver(int x, int y, eMouseButton button);
        void onMouseClick(int x, int y, eMouseButton button);
        void onMouseMove(int x, int y, eMouseButton button);
        void onKeyUp(eKeys keyCode);
        void onKeyDown(eKeys keyCode);

        void onRender() override;
        void onUpdate(float dt) override;
    };
}

