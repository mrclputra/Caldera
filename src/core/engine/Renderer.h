#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <set>

#include "Scene.h"
#include "components/Shader.h"

class Renderer {
public:
    explicit Renderer(QOpenGLFunctions_4_5_Core* f, int width, int height);

    void initialize();
    void render(float deltaTime);
    void resize(int w, int h);
    void onKeyPress(int key);
    void onKeyRelease(int key);
    void onMouseMove(int dx, int dy);

private:
    QOpenGLFunctions_4_5_Core* f;
    Scene scene;
    std::shared_ptr<Shader> shader;
    std::set<int> held_keys;
    int width, height;
};
