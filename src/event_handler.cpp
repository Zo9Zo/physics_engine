#include <SFML/Graphics.hpp>

#include "event_handler.hpp"

EventHandler& EventHandler::Instance() {
    static EventHandler instance;
    return instance;
}

void EventHandler::Handle(sf::RenderWindow* window) const {
    while (const auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window->close();
            return;
        }
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) {
                window->close();
                return;
            }
        }
    }
}
