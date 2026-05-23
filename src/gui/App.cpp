#include "App.h"

App::App(
    const int argc,
    char* argv[],
    const int width,
    const int height,
    const char* title
) : argc(argc), qapp(this->argc, argv) {
    window.resize(width, height);
    window.setWindowTitle(title);
    SPDLOG_INFO("Initialized new QT window");
}

int App::run() {
    window.show();
    return qapp.exec();
}
