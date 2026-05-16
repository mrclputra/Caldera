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
    m_renderer = std::make_unique<Renderer>(this);
    m_renderer->initialize();
    m_timer.start();
}

void ViewportWidget::paintGL() {
    float deltaTime = m_timer.elapsed() / 1000.0f;
    m_timer.restart();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_renderer->render(deltaTime);

    update();
}

void ViewportWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    m_renderer->resize(w, h);
}

void ViewportWidget::keyPressEvent(QKeyEvent* e) {
    m_renderer->onKeyPress(e->key()); // propagate to renderer
    QOpenGLWidget::keyPressEvent(e);
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* e) {
    if (m_mouseCapture) {
        QPoint delta = e->pos() - m_lastMousePos;
        m_renderer->onMouseMove(delta.x(), delta.y());
    }
    m_lastMousePos = e->pos();
    QOpenGLWidget::mouseMoveEvent(e);
}

void ViewportWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        m_mouseCapture = true;
        m_lastMousePos = e->pos();
    }
    QOpenGLWidget::mousePressEvent(e);
}

void ViewportWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton)
        m_mouseCapture = false;
    QOpenGLWidget::mouseReleaseEvent(e);
}
