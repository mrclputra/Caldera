#pragma once
#include <spdlog/spdlog.h>
#include <QOpenGLFunctions_4_5_Core>

class Renderer {
public:
    explicit Renderer(QOpenGLFunctions_4_5_Core* f);

    void initialize();
    void render(float deltaTime);
    void resize(int w, int h);
    void onKeyPress(int key);
    void onMouseMove(int dx, int dy);

private:
    QOpenGLFunctions_4_5_Core* f;
};
