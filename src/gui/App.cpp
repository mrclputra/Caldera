#include "App.h"

App::App(int argc, char* argv[], int width, int height, const char* title)
    : m_argc(argc), m_qapp(m_argc, argv) {
    //m_qapp.setStyle("Fusion");
    m_window.resize(width, height);
    m_window.setWindowTitle(title);
    spdlog::info("Initialized new QT window");
}

int App::run() {
    m_window.show();
    return m_qapp.exec();
}
