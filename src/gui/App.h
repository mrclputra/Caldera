#pragma once
#include <QApplication>
#include <spdlog/spdlog.h>
#include "MainWindow.h"

class App {
public:
    App(int argc, char* argv[], int width, int height, const char* title);
    int run();

private:
    int m_argc;  // QApplication holds a reference to argc, must outlive it
    QApplication m_qapp;
    MainWindow m_window;
};
