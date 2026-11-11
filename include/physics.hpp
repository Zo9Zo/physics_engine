#pragma once

#include <SFML/Graphics.hpp>

#include "config.hpp"

class DynamicObject {
   public:
    DynamicObject(float mass, const sf::Vector2f& position) : mass_{mass}, position_{position} {
    }

    float Mass() const;

    sf::Vector2f Velocity() const;

    sf::Vector2f Position() const;

    void AddForce(const sf::Vector2f&);

    void SetVelocity(const sf::Vector2f&);

    void Move(float);

   private:
    const float mass_;
    sf::Vector2f force_{};
    sf::Vector2f velocity_{};
    sf::Vector2f position_;
};

class StaticObject {
   public:
    StaticObject(const sf::Vector2f& position) : position_{position} {
    }

    sf::Vector2f Position() const;

   private:
    sf::Vector2f position_;
};

class Particle : public DynamicObject {
   public:
    Particle(float radius, float mass, const sf::Vector2f& position) : radius_{radius}, DynamicObject(mass, position) {
    }

    float Radius() const;

   private:
    const float radius_;
};

enum DIRECTION {VERTICAL, HORIZONTAL};

class Wall : public StaticObject {
   public:
    Wall(DIRECTION direction, const sf::Vector2f& position) : direction_{direction}, StaticObject{position} {
    }

    DIRECTION Direction() const;

   private:
    const DIRECTION direction_;
};

bool HaveCollided(const Particle&, const Wall&);

void Collide(Particle*, Wall*);

// void Collide(PhysicalObject*, PhysicalObject*);