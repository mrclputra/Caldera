#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_4_5_Core>
#include <set>

#include "Scene.h"
#include "components/Shader.h"
#include "Gizmos.h"

class Renderer {
public:
    explicit Renderer(QOpenGLFunctions_4_5_Core* f, int w, int h);

    void initialize();
    void render(float deltaTime);
    void resize(int w, int h);
    void onKeyPress(int key);
    void onKeyRelease(int key);
    void onMouseMove(int dx, int dy);
    void onScroll(int delta);

    Scene scene;
    Gizmos gizmos;

private:
    QOpenGLFunctions_4_5_Core* f;
    std::shared_ptr<Shader> shader;
    std::set<int> held_keys;
    float point_size = 14.0f;

    int width, height;
};

#endif // RENDERER_H