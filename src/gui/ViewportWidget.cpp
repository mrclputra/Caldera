#include "ViewportWidget.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <QMouseEvent>
#include <QPainter>

#include "engine/Renderer.h"

extern std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> g_ring_sink;

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::initializeGL() {
    initializeOpenGLFunctions();

    SPDLOG_INFO("GL vendor: {}",   reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    SPDLOG_INFO("GL renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    SPDLOG_INFO("GL version: {}",  reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderer = std::make_unique<Renderer>(this, this->width(), this->height());
    renderer->initialize();
    timer.start();
}

void ViewportWidget::paintGL() {
    float delta_time = timer.elapsed() / 1000.0f;
    timer.restart();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->render(delta_time);

    update();

    QPainter painter(this);
    painter.setFont(QFont("Consolas", 9));

    // diagnostics
    painter.setPen(QColor(255, 255, 255, 128));
    painter.drawText(8, 16, QString("speed: %1").arg(renderer->scene.camera.speed, 0, 'f', 1));
    painter.drawText(8, 30, QString("fps: %1").arg(1.0f / delta_time, 0, 'f', 0));

    // log overlay
    if (g_ring_sink) {
        const auto lines = g_ring_sink->last_formatted();
        constexpr int line_h = 12;
        int y = height() - 8 - static_cast<int>(lines.size()) * line_h;
        for (const auto& line : lines) {
            painter.drawText(8, y, QString::fromStdString(line).trimmed());
            y += line_h;
        }
    }

    painter.end();
}

void ViewportWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    renderer->resize(w, h);
}

void ViewportWidget::enterEvent(QEnterEvent* e) {
    setFocus();
    QOpenGLWidget::enterEvent(e);
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
    if (mouse_capture) {
        QPoint delta = e->pos() - last_mouse_pos;
        renderer->onMouseMove(delta.x(), delta.y());
    }
    last_mouse_pos = e->pos();
    QOpenGLWidget::mouseMoveEvent(e);
}

void ViewportWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton) {
        // capture mouse inputs in the viewport only if right click
        mouse_capture = true;
        last_mouse_pos = e->pos();
    }
    QOpenGLWidget::mousePressEvent(e);
}

void ViewportWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::RightButton)
        mouse_capture = false;
    QOpenGLWidget::mouseReleaseEvent(e);
}

void ViewportWidget::wheelEvent(QWheelEvent* e) {
    renderer->onScroll(e->angleDelta().y());
    QOpenGLWidget::wheelEvent(e);
}