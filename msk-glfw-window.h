#pragma once

namespace TexGen
{
    class TgGlfwindow : public ITgWindow
    {
        public:
            TgGlfwindow(int width, int height);
            ~TgGlfwindow();

            void show() override;
            void hide() override;
            void run() override;
            void addScene(const std::shared_ptr<ITgScene>& scene) override;
            void update(float dt) override;
	        void render() override;

        private:
            GLFWwindow* m_window;
            std::vector<std::shared_ptr<ITgScene>> m_scene;

            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
            static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
            static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    };
}