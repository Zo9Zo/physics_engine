#include <SFML/Graphics.hpp>
#include <vector>

#include "config.hpp"
#include "event_handler.hpp"
#include "renderer.hpp"


Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

void Renderer::Render(std::vector<Particle>&& particles) const {
    sf::RenderWindow window(sf::VideoMode(Config::window_resolution), "Particle Collision");
    window.setFramerateLimit(Config::max_fps);

    const EventHandler& handler = EventHandler::Instance();

    std::vector<Wall> walls;
    walls.push_back({DIRECTION::HORIZONTAL, {0, 0}});
    walls.push_back({DIRECTION::HORIZONTAL, static_cast<sf::Vector2f>(Config::window_resolution)});
    walls.push_back({DIRECTION::VERTICAL, {0, 0}});
    walls.push_back({DIRECTION::VERTICAL, static_cast<sf::Vector2f>(Config::window_resolution)});

    while (window.isOpen()) {
        window.clear();
        for (auto& particle: particles) {
            sf::CircleShape circle(particle.Radius());
            circle.setOrigin({particle.Radius(), particle.Radius()});
            circle.setFillColor(sf::Color::Cyan);
            circle.setPosition(particle.Position());
            circle.setOutlineColor(sf::Color::White);
            circle.setOutlineThickness(1.0f);
            window.draw(circle);

            particle.AddForce({0.0f, -1000.0f});
            particle.Move(Config::dt / Config::max_fps); //TODO: change to actual fps
            for (auto& wall : walls) {
                Collide(&particle, &wall);
            }
        }
        window.display();

        handler.Handle(&window);
    }
}

Renderer::Renderer() {
}