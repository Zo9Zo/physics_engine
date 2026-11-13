#include <SFML/Graphics.hpp>
#include <vector>

#include "config.hpp"
#include "event_handler.hpp"
#include "physics.hpp"
#include "physics_manager.hpp"
#include "renderer.hpp"

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

void Renderer::Render(std::vector<Particle>* particles) const {
    sf::RenderWindow window(sf::VideoMode(Config::window_resolution), "Particle Collision");
    window.setFramerateLimit(Config::max_fps);
    
    PhysicsManager* physics_manager = &PhysicsManager::Instance(); //TODO: Make every singleton instance return a pointer
    physics_manager->AddObjects(particles);

    const EventHandler& handler = EventHandler::Instance();

    // TODO: Move this to object_manager
    std::vector<Wall> walls;
    walls.push_back({LineCollider::DIRECTION::HORIZONTAL, {0, 0}});
    walls.push_back({LineCollider::DIRECTION::HORIZONTAL, static_cast<sf::Vector2f>(Config::window_resolution)});
    walls.push_back({LineCollider::DIRECTION::VERTICAL, {0, 0}});
    walls.push_back({LineCollider::DIRECTION::VERTICAL, static_cast<sf::Vector2f>(Config::window_resolution)});
    
    physics_manager->AddObjects(&walls);

    while (window.isOpen()) {
        window.clear();
        for (int i = 0; i < particles->size(); ++i) {
            const Particle* particle = &particles->at(i);
            const CircleCollider* particle_collider = static_cast<const CircleCollider*>(particle->GetCollider());
            const float radius = particle_collider->Radius();
            sf::CircleShape circle(radius);
            circle.setOrigin({radius, radius});
            circle.setFillColor(sf::Color::Cyan);
            circle.setPosition(particle->GetTransform().position);
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(1.0f);
            window.draw(circle);
        }
        
        physics_manager->Simulate(Config::dt / Config::max_fps); //TODO: change to actual fps

        window.display();

        handler.Handle(&window);
    }
}
