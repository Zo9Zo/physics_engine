#include <cmath>

#include "config.hpp"
#include "physics.hpp"

float DynamicObject::Mass() const { 
    return mass_;
}

sf::Vector2f DynamicObject::Velocity() const {
    return velocity_;
}

sf::Vector2f DynamicObject::Position() const { 
    return position_;
}

void DynamicObject::AddForce(const sf::Vector2f& force) {
    force_ += force;
}

void DynamicObject::SetVelocity(const sf::Vector2f& velocity) {
    velocity_ = velocity;
}

void DynamicObject::Move(float dt) {
    velocity_ += force_ / mass_ * dt;
    position_.x += velocity_.x * dt;
    position_.y -= velocity_.y * dt;
    force_ = {};
}

sf::Vector2f StaticObject::Position() const { 
    return position_;
}

float Particle::Radius() const {
    return radius_;
}

DIRECTION Wall::Direction() const {
    return direction_;
}

bool HaveCollided(const Particle& particle, const Wall& wall) {
    if (wall.Direction() == DIRECTION::HORIZONTAL) {
        const float dy = std::abs(wall.Position().y - particle.Position().y);
        return dy < particle.Radius();
    }
    const float dx = std::abs(wall.Position().x - particle.Position().x);
    return dx < particle.Radius();
}

void Collide(Particle* particle, Wall* wall) {
    if (!HaveCollided(*particle, *wall)) {
        return;
    } //TODO: Move this check to the physics manager
    
    if (wall->Direction() == DIRECTION::HORIZONTAL) {
        const sf::Vector2f new_velocity = {particle->Velocity().x, -particle->Velocity().y};
        particle->SetVelocity(new_velocity);
        return;
    }
    const sf::Vector2f new_velocity = {-particle->Velocity().x, particle->Velocity().y};
    particle->SetVelocity(new_velocity);
}

// void Collide(PhysicalObject* obj1, PhysicalObject* obj2) {

// }