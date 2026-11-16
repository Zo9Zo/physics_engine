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

    virtual Collider::TYPE Type() const = 0;
    
    virtual ~Collider() = default;
};

class CircleCollider : public Collider {
   public:
    CircleCollider(float radius) : radius_{radius} {
    }

    Collider::TYPE Type() const override;

    float Radius() const;

   private:
    const float radius_;
};

class LineCollider : public Collider {
   public:
    enum class DIRECTION {VERTICAL, HORIZONTAL};

    LineCollider(DIRECTION direction) : direction_{direction} {
    }
    
    Collider::TYPE Type() const override;

    DIRECTION Direction() const;

   private:
    const DIRECTION direction_;
};

class RigidBody {
   public:
    enum class TYPE {STATIC, DYNAMIC};

    RigidBody(TYPE type, std::shared_ptr<Collider> collider,
        const sf::Vector2f& position, float restitution) : type_{type}, collider_{std::move(collider)},
        transform_(position), restitution_{restitution} {
    }

    RigidBody(TYPE type, std::shared_ptr<Collider> collider,
        const sf::Vector2f& position, float restitution, float mass) : type_{type},
        collider_{std::move(collider)}, transform_(position), restitution_{restitution},
        inverse_mass_{1.0f / mass} {
    }
    
    TYPE Type() const;

    const Collider* GetCollider() const;

    Transform* GetTransform();

    const Transform& GetTransform() const;
    
    float Restitution() const;

    float InverseMass() const;

    sf::Vector2f Velocity() const;

    void SetVelocity(const sf::Vector2f&);

    void AddForce(const sf::Vector2f&);

    void Move(float);

   private:
    const TYPE type_;
    std::shared_ptr<Collider> collider_;
    Transform transform_;
    const float restitution_ = 1.0f;
    const float inverse_mass_ = 0.0f;
    sf::Vector2f velocity_{};
    sf::Vector2f force_{};
};

// class StaticObject {
//    public:
//     StaticObject(const sf::Vector2f& position, std::shared_ptr<Collider> collider) : transform_(position),
//         collider_{std::move(collider)} {
//     }

//     Transform* GetTransform();

//     const Transform& GetTransform() const;
    
//     const Collider* GetCollider() const;

//    private:
//     Transform transform_;
//     std::shared_ptr<Collider> collider_;
// };

// class DynamicObject {
//    public:
//     DynamicObject(float mass, const sf::Vector2f& position, std::shared_ptr<Collider> collider) : mass_{mass},
//         transform_(position), collider_{std::move(collider)} {
//     }

//     float Mass() const;

//     sf::Vector2f Velocity() const;

//     Transform* GetTransform();

//     const Transform& GetTransform() const;

//     const Collider* GetCollider() const;

//     void AddForce(const sf::Vector2f&);

//     void SetVelocity(const sf::Vector2f&);

//     void Move(float);

//    private:
//     const float mass_;
//     sf::Vector2f force_{};
//     sf::Vector2f velocity_{};
//     Transform transform_;
//     std::shared_ptr<Collider> collider_;
// };

class Particle : public RigidBody{
   public:
    Particle(float radius, const sf::Vector2f& position, float restitution, float mass) : RigidBody(TYPE::DYNAMIC,
        std::make_shared<CircleCollider>(radius), position, restitution, mass) {
    }
};

class Wall : public RigidBody {
   public:
    Wall(LineCollider::DIRECTION direction, const sf::Vector2f& position, float restitution) : RigidBody(TYPE::STATIC,
        std::make_shared<LineCollider>(direction), position, restitution) {
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

void Collide(RigidBody*, const RigidBody&);

void Collide(const RigidBody&, RigidBody*);

void Collide(RigidBody*, RigidBody*);
