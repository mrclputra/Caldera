#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QElapsedTimer>
#include <QPoint>
#include <memory>

class Renderer;

class ViewportWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit ViewportWidget(QWidget* parent = nullptr);
    ~ViewportWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void keyPressEvent(QKeyEvent* e) override;
    void mouseMoveEvent(QMouseEvent * e) override;
    void mousePressEvent(QMouseEvent * e) override;
    void mouseReleaseEvent(QMouseEvent * e) override;

private:
    std::unique_ptr<Renderer> m_renderer;
    QElapsedTimer m_timer;
    QPoint m_lastMousePos;
    bool m_mouseCapture = false;
};

#endif // VIEWPORTWIDGET_H
