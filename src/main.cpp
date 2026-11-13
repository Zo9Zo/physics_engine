#include <vector>

#include "config.hpp"
#include "renderer.hpp"
#include "physics.hpp"
#include "physics_manager.hpp"

int main() {
    std::vector<Particle> particles{};

    const sf::Vector2f particle_position1 = {Config::window_resolution.x * 0.4f, 100.0f};
    Particle particle1(Config::particle_radius * 2.0f, Config::particle_mass * 10.0f, particle_position1);
    particle1.SetVelocity({100.0f, 0.0f});
    particles.push_back(std::move(particle1));
    
    const sf::Vector2f particle_position2 = {Config::window_resolution.x * 0.75f, 100.0f};
    Particle particle2(Config::particle_radius, Config::particle_mass, particle_position2);
    particle2.SetVelocity({-100.0f, 0.0f});
    particles.push_back(std::move(particle2));

    const Renderer& renderer = Renderer::Instance();
    renderer.Render(&particles);
}
