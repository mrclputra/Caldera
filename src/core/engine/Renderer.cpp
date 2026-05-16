#include "Renderer.h"

Renderer::Renderer(QOpenGLFunctions_4_5_Core* f) : f(f) {}

void Renderer::initialize() {}

void Renderer::render(float deltaTime) {}

void Renderer::resize(int w, int h) {}

void Renderer::onKeyPress(int key) {
	spdlog::info("renderer key press: {}", key);
}

void Renderer::onMouseMove(int dx, int dy) {
	spdlog::info("renderer mouse move: {}, {}", dx, dy);
}
