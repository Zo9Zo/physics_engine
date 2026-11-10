#pragma once

class EventHandler {
   public:
    EventHandler(const EventHandler&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
    EventHandler(EventHandler&&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;

    static EventHandler& Instance();

    void Handle(sf::RenderWindow*) const;
   
   private:
    EventHandler();
};