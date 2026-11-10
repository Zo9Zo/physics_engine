#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "event_handler.hpp"
#include "renderer.hpp"

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

void Renderer::Render() const {
    sf::RenderWindow window(sf::VideoMode(Config::window_resolution), "Particle Collision");

    const EventHandler& handler = EventHandler::Instance();
    
    sf::CircleShape circle(Config::particle_radius);
    circle.setOrigin({Config::particle_radius, Config::particle_radius});
    circle.setFillColor(sf::Color::Cyan);
    constexpr sf::Vector2f circle_position = static_cast<sf::Vector2f>(Config::window_resolution) * 0.5f;
    circle.setPosition(circle_position);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(1.0f);

    while (window.isOpen()) {
        window.clear();
        window.draw(circle);
        window.display();

        handler.Handle(&window);
    }
}

Renderer::Renderer() {
}