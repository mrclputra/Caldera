#include "ViewportWidget.h"
#include "engine/Renderer.h"
#include <QKeyEvent>
#include <QMouseEvent>

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    renderer = std::make_unique<Renderer>(this, this->width(), this->height());
    renderer->initialize();
    timer.start();
}

void ViewportWidget::paintGL() {
    float deltaTime = timer.elapsed() / 1000.0f;
    timer.restart();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->render(deltaTime);

    update();
}

void ViewportWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    renderer->resize(w, h);
}

void ViewportWidget::keyPressEvent(QKeyEvent* e) {
    renderer->onKeyPress(e->key()); // propagate to renderer
    QOpenGLWidget::keyPressEvent(e);
}

void ViewportWidget::keyReleaseEvent(QKeyEvent* e) {
    renderer->onKeyRelease(e->key());
    QOpenGLWidget::keyReleaseEvent(e);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* e) {
    if (mouseCapture) {
        QPoint delta = e->pos() - lastMousePos;
        renderer->onMouseMove(delta.x(), delta.y());
    }
    lastMousePos = e->pos();
    QOpenGLWidget::mouseMoveEvent(e);
}

void ViewportWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        // capture mouse inputs in the viewport only if right click
        mouseCapture = true;
        lastMousePos = e->pos();
    }
    QOpenGLWidget::mousePressEvent(e);
}

void ViewportWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton)
        mouseCapture = false;
    QOpenGLWidget::mouseReleaseEvent(e);
}
