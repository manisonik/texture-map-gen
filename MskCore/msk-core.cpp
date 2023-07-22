#pragma once
#include "stdafx.h"
#include "msk-app.h"
#include "msk-gl-main-scene.h"

using namespace Manisonik;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::unique_ptr<MskApp> app = std::make_unique<MskApp>();
    app->AddScene(std::make_shared<MMainScene>(0, 0, 100, 100));
    app->Run();

    return 0;
}