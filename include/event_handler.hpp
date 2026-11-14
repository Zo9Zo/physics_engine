#pragma once

#include <SFML/Graphics.hpp>

#include "physics_manager.hpp"

class EventHandler {
   public:
    EventHandler(const EventHandler&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
    EventHandler(EventHandler&&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;

    static EventHandler& Instance();

    void Handle(sf::RenderWindow*, PhysicsManager*) const;
   
   private:
    EventHandler() = default;
};
