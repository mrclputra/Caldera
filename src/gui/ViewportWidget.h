#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QElapsedTimer>
#include <QPoint>
#include <memory>
#include <array>
#include <numeric>

#include "utilities/Loader.h"

class Renderer;

class ViewportWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit ViewportWidget(QWidget* parent = nullptr);
    ~ViewportWidget();

    void loadFile(const QString& path);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void enterEvent(QEnterEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    std::unique_ptr<Renderer> renderer;
    LoaderThread loader_thread;
    QElapsedTimer timer;
    bool mouse_capture = false;
    QPoint lock_pos;

    static constexpr int FPS_SAMPLES = 60;
    std::array<float, FPS_SAMPLES> delta_samples{};
    int delta_index = 0;
};

#endif // VIEWPORTWIDGET_H
