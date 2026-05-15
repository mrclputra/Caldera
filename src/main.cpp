#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
	spdlog::set_pattern("[%H:%M:%S - %t] [%^%l%$] %v");
	spdlog::info("SPDLOG initialized v{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR);
	return 0;
}