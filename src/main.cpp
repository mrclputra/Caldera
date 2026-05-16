#include <spdlog/spdlog.h>
#include "App.h"

int main(int argc, char* argv[]) {
	spdlog::set_pattern("[%H:%M:%S - %t] [%^%l%$] %v");
	spdlog::info("SPDLOG initialized v{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR);

	App app(argc, argv, 1280, 720, "Caldera");
	return app.run();
}