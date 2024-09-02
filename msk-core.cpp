#include "stdafx.h"
#include "msk-app.h"
#include "msk-gl-main-scene.h"
#include "msk-glfw-window.h"

using namespace TexGen;

int main()
{
    Tg tg(std::make_unique<TgGlfwindow>(640, 480));

    std::shared_ptr<ITgScene> scene = std::make_shared<TgMainScene>(0, 0, 640, 480);
    tg.addScene(scene);
    tg.run();
   
    return 0;
}