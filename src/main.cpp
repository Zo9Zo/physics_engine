#include <vector>

#include "config.hpp"
#include "renderer.hpp"
#include "physics.hpp"

int main() {
    constexpr sf::Vector2f particle_position = {Config::window_resolution.x * 0.5f, 100};
    Particle particle(Config::particle_radius, Config::particle_mass, particle_position);

    std::vector<Particle> particles = {particle};

    const Renderer& renderer = Renderer::Instance();
    renderer.Render(std::move(particles));
}