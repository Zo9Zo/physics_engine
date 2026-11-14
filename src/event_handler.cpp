#include <SFML/Graphics.hpp>

#include "event_handler.hpp"

EventHandler& EventHandler::Instance() {
    static EventHandler instance;
    return instance;
}

void EventHandler::Handle(sf::RenderWindow* window, PhysicsManager* physics_manager) const {
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
            if (key->code == sf::Keyboard::Key::Space) {
                if (physics_manager->State() == PhysicsManager::STATE::ACTIVE) {
                    physics_manager->SetState(PhysicsManager::STATE::PAUSED);
                } else {
                    physics_manager->SetState(PhysicsManager::STATE::ACTIVE);
                }
            }
        }
    }
}
