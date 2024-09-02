 
 #pragma once

 namespace TexGen
 {
    class TgWin32Window : ITgWindow
    {
        private:
            HDC m_hDC;
            HGLRC m_hRC;
            unsigned int m_nPixelFormat;
            HWND m_hWnd;
            HINSTANCE m_hInstance;
            HICON m_hIcon;
            bool m_bMouseDown;

        public:
            TgWin32Window();
            ~TgWin32Window();

            // ITgWindow overrides
            void show() override;
            void hide() override;
            void addScene(const std::shared_ptr<ITgScene>& scene) override;
            void run() override;
            void update(float dt) override;
            void render() override;

        protected:
            void registerWindow();
            void setupWindow();
            bool setupOpenGL();
            bool initializeGL();
            static LRESULT CALLBACK routerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
            LRESULT CALLBACK onProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
    };
 }