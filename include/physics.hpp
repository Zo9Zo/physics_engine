#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>

#include "config.hpp"

struct Transform {
    sf::Vector2f position;
};

class Collider {
   public:
    enum class TYPE {CIRCLE, LINE, COUNT};

    virtual Collider::TYPE GetType() const = 0;
    
    virtual ~Collider() = default;
};

class CircleCollider : public Collider {
   public:
    CircleCollider(float radius) : radius_{radius} {
    }

    Collider::TYPE GetType() const override;

    float Radius() const;

   private:
    const float radius_;
};

class LineCollider : public Collider {
   public:
    enum class DIRECTION {VERTICAL, HORIZONTAL};

    LineCollider(DIRECTION direction) : direction_{direction} {
    }
    
    Collider::TYPE GetType() const override;

    DIRECTION Direction() const;

   private:
    const DIRECTION direction_;
};

class StaticObject {
   public:
    StaticObject(const sf::Vector2f& position, std::shared_ptr<Collider> collider) : transform_(position),
        collider_{std::move(collider)} {
    }

    // StaticObject(const StaticObject&) = default;
    // StaticObject& operator=(const StaticObject&) = default;
    // StaticObject(StaticObject&&) = default;
    // StaticObject& operator=(StaticObject&&) = default;
    
    Transform* GetTransform();

    const Transform& GetTransform() const;
    
    const Collider* GetCollider() const;

   private:
    Transform transform_;
    std::shared_ptr<Collider> collider_;
};

class DynamicObject {
   public:
    DynamicObject(float mass, const sf::Vector2f& position, std::shared_ptr<Collider> collider) : mass_{mass},
        transform_(position), collider_{std::move(collider)} {
    }

    // DynamicObject(const DynamicObject&) = delete;
    // DynamicObject& operator=(const DynamicObject&) = delete;
    // DynamicObject(DynamicObject&&) = default;
    // DynamicObject& operator=(DynamicObject&&) = default;

    float Mass() const;

    sf::Vector2f Velocity() const;

    Transform* GetTransform();

    const Transform& GetTransform() const;

    const Collider* GetCollider() const;

    void AddForce(const sf::Vector2f&);

    void SetVelocity(const sf::Vector2f&);

    void Move(float);

   private:
    const float mass_;
    sf::Vector2f force_{};
    sf::Vector2f velocity_{};
    Transform transform_;
    std::shared_ptr<Collider> collider_;
};

class Particle : public DynamicObject {
   public:
    Particle(float radius, float mass, const sf::Vector2f& position) : DynamicObject(mass,
        position, std::make_shared<CircleCollider>(radius)) {
    }
};

class Wall : public StaticObject {
   public:
    Wall(LineCollider::DIRECTION direction, const sf::Vector2f& position) : StaticObject{position,
        std::make_shared<LineCollider>(direction)} {
    }
};

bool HaveCollided(const CircleCollider&, const Transform&, const CircleCollider&, const Transform&);

bool HaveCollided(const CircleCollider&, const Transform&, const LineCollider&, const Transform&);

std::optional<sf::Vector2f> CollisionNormal(const CircleCollider&, const Transform&,
    const CircleCollider&, const Transform&);

std::optional<sf::Vector2f> CollisionNormal(const CircleCollider&, const Transform&,
    const LineCollider&, const Transform&);
    
std::optional<sf::Vector2f> CollisionNormal(const LineCollider&, const Transform&,
    const CircleCollider&, const Transform&);

template <typename ColliderType1, typename ColliderType2>
std::optional<sf::Vector2f> CollisionNormalDispatch(const Collider&, const Transform&,
    const Collider&, const Transform&);

void Collide(DynamicObject*, const StaticObject&);

void Collide(DynamicObject*, DynamicObject*);
