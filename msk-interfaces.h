#pragma once

struct ITgScene
{
    virtual void render() = 0;
    virtual void update(float dt) = 0;
    virtual ~ITgScene() = default;
};

struct ITgWindow
{
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void run() = 0;
    virtual void addScene(const std::shared_ptr<ITgScene>& scene) = 0;
    virtual void update(float dt) = 0;
	virtual void render() = 0;
    virtual ~ITgWindow() = default;
};