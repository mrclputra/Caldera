#include <spdlog/spdlog.h>
#include <QSurfaceFormat>
#include <QStyle>

#include "App.h"
#include "spdlog/sinks/ringbuffer_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> g_ring_sink;

int main(int argc, char* argv[]) {
	// configure logger
	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	g_ring_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(20); // decide how many lines to save here

	auto logger = std::make_shared<spdlog::logger>("main", spdlog::sinks_init_list{stdout_sink, g_ring_sink});
	spdlog::set_default_logger(logger);
	spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [%s:%#] %v");

	SPDLOG_INFO("spdlog v{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR); // todo: move these somewhere else
	SPDLOG_INFO("Qt {}", QT_VERSION_STR);
	SPDLOG_INFO("Platform: {}", QGuiApplication::platformName().toStdString());

	// configure app style
	QApplication::setStyle("Fusion");
	SPDLOG_INFO("Style: {}", qApp->style()->objectName().toStdString());

	// configure QT OpenGL
	QSurfaceFormat fmt;
	fmt.setVersion(4, 5);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	fmt.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(fmt);

	// entry point
	App app(argc, argv, 1600, 900, "Caldera");
	return app.run();
}
