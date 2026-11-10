#include "renderer.hpp"

int main() {
    const Renderer& renderer = Renderer::Instance();
    renderer.Render();
}