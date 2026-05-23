#include <spdlog/spdlog.h>
#include <QSurfaceFormat>
#include <QStyle>

#include "App.h"

int main(int argc, char* argv[]) {
	spdlog::set_pattern("[%H:%M:%S - %t] [%^%l%$] %v");
	spdlog::info("SPDLOG initialized v{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR);

	spdlog::info("Qt version: {}", QT_VERSION_STR);
	spdlog::info("Platform: {}", QGuiApplication::platformName().toStdString());

	QApplication::setStyle("Fusion");
	spdlog::info("Style: {}", qApp->style()->objectName().toStdString());

	QSurfaceFormat fmt;
	fmt.setVersion(4, 5);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);

	App app(argc, argv, 1280, 720, "Caldera");
	return app.run();
}