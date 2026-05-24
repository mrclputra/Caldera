#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "spdlog/spdlog.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // bind mainwindow stuff here
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onLoad);
}
MainWindow::~MainWindow() {
    delete ui;
}

// slots
void MainWindow::onExit() {
    SPDLOG_INFO("EXIT CALLED");
    QApplication::quit();
}
void MainWindow::onLoad() {
    SPDLOG_INFO("LOAD CALLED");
    // todo: should probably move this to the viewportwidget class
    static QString last_dir;
    QString path = QFileDialog::getOpenFileName(this, "Load", last_dir, "3D Models (*.ply *.gltf *.glb);;All Files (*)");
    if (!path.isEmpty()) {
        last_dir = QFileInfo(path).absolutePath();
        ui->glViewer->loadFile(path);
    }
}