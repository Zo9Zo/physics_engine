#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>

#include "config.hpp"
#include "physics.hpp"

Collider::TYPE CircleCollider::Type() const {
    return Collider::TYPE::CIRCLE;
}

float CircleCollider::Radius() const {
    return radius_;
}

Collider::TYPE LineCollider::Type() const {
    return Collider::TYPE::LINE;
}

LineCollider::DIRECTION LineCollider::Direction() const {
    return direction_;
}

RigidBody::TYPE RigidBody::Type() const {
    return type_;
}

Transform* RigidBody::GetTransform() { 
    return &transform_;
}

const Transform& RigidBody::GetTransform() const { 
    return transform_;
}

const Collider* RigidBody::GetCollider() const {
    return collider_.get();
}

float RigidBody::Restitution() const {
    return restitution_;
}

float RigidBody::InverseMass() const { 
    return inverse_mass_;
}

sf::Vector2f RigidBody::Velocity() const {
    return velocity_;
}

void RigidBody::SetVelocity(const sf::Vector2f& velocity) {
    velocity_ = velocity;
}

void RigidBody::AddForce(const sf::Vector2f& force) {
    force_ += force;
}

void RigidBody::Move(float dt) {
    velocity_ += force_ * inverse_mass_ * dt;
    transform_.position.x += velocity_.x * dt;
    transform_.position.y -= velocity_.y * dt;
    force_ = {};
}

// Circle---Circle
bool HaveCollided(const CircleCollider& circle_collider1, const Transform& circle_transform1, 
    const CircleCollider& circle_collider2, const Transform& circle_transform2) {
    const sf::Vector2f r = circle_transform1.position - circle_transform2.position;
    const float r1 = circle_collider1.Radius();
    const float r2 = circle_collider2.Radius();
    return r.lengthSquared() < (r1 + r2) * (r1 + r2);
}

// Circle---Line
bool HaveCollided(const CircleCollider& circle_collider, const Transform& circle_transform,
    const LineCollider& line_collider, const Transform& line_transform) {
    if (line_collider.Direction() == LineCollider::DIRECTION::HORIZONTAL) {
        const float dy = std::abs(line_transform.position.y - circle_transform.position.y);
        return dy < circle_collider.Radius();
    }
    const float dx = std::abs(line_transform.position.x - circle_transform.position.x);
    return dx < circle_collider.Radius();
}

// Normal points from object1 to object2

// Circle---Circle
std::optional<sf::Vector2f> CollisionNormal(const CircleCollider& circle_collider1,
    const Transform& circle_transform1, const CircleCollider& circle_collider2,
    const Transform& circle_transform2) {
    if (!HaveCollided(circle_collider1, circle_transform1, circle_collider2, circle_transform2)) {
        return std::nullopt;
    }
    sf::Vector2f r = circle_transform2.position - circle_transform1.position;
    const float r_length = r.length();
    if (r_length < Config::epsilon) {
        return std::nullopt;
    }
    return r / r_length;
}

// Circle---Line
std::optional<sf::Vector2f> CollisionNormal(const CircleCollider& circle_collider,
    const Transform& circle_transform, const LineCollider& line_collider,
    const Transform& line_transform) {
    if (!HaveCollided(circle_collider, circle_transform, line_collider, line_transform)) {
        return std::nullopt;
    }
    if (line_collider.Direction() == LineCollider::DIRECTION::HORIZONTAL) {
        const float dy = std::abs(line_transform.position.y - circle_transform.position.y);
        if (dy > 0.0f) {
            return sf::Vector2f{0.0f, 1.0f};
        }
        return sf::Vector2f{0.0f, -1.0f};
    }
    const float dx = std::abs(line_transform.position.x - circle_transform.position.x);
    if (dx > 0.0f) {
        return sf::Vector2f{1.0f, 0.0f};
    }
    return sf::Vector2f{-1.0f, 0.0f};
}

// Line---Circle
std::optional<sf::Vector2f> CollisionNormal(const LineCollider& line_collider,
    const Transform& line_transform, const CircleCollider& circle_collider,
    const Transform& circle_transform) {
    std::optional<sf::Vector2f> optional_normal = CollisionNormal(circle_collider,
        circle_transform, line_collider, line_transform);
    if (!optional_normal.has_value()) { 
        return std::nullopt;
    }
    return -optional_normal.value();
}

template <typename ColliderType1, typename ColliderType2>
std::optional<sf::Vector2f> CollisionNormalDispatch(const Collider& collider1,
    const Transform& transform1, const Collider& collider2, const Transform& transform2) {
    return CollisionNormal(static_cast<const ColliderType1&>(collider1), transform1,
        static_cast<const ColliderType2&>(collider2), transform2);
}

using CollisionMatrix = std::optional<sf::Vector2f> (*) (const Collider&, const Transform&,
    const Collider&, const Transform&);

CollisionMatrix collision_matrix[static_cast<uint32_t>(Collider::TYPE::COUNT)]
    [static_cast<uint32_t>(Collider::TYPE::COUNT)] = {
    {
        CollisionNormalDispatch<CircleCollider, CircleCollider>,
        CollisionNormalDispatch<CircleCollider, LineCollider>
    },
    {
        CollisionNormalDispatch<LineCollider, CircleCollider>, // TODO: check if this can be nullptr
        nullptr
    }
};

// Object passed by pointer is dynamic, object passed by const reference is static

#include <iostream>

void Collide(RigidBody* object1, const RigidBody& object2) {
    const Collider* const collider1 = object1->GetCollider();
    const Collider* const collider2 = object2.GetCollider();
    Collider::TYPE collider_type1 = collider1->Type();
    Collider::TYPE collider_type2 = collider2->Type();
    const Transform* const transform1 = object1->GetTransform();
    const Transform transform2 = object2.GetTransform();
    auto optional_normal = collision_matrix[static_cast<uint32_t>(collider_type1)]
        [static_cast<uint32_t>(collider_type2)](*collider1, *transform1, *collider2, transform2);
    if (!optional_normal.has_value()) {
        return;
    }
    std::cout << "collided " << (int)object1->GetCollider()->Type() << " and " << (int)object2.GetCollider()->Type() << "\n";
    const sf::Vector2f normal = std::move(optional_normal).value();
    const sf::Vector2f v1 = object1->Velocity();
    const float restitution = std::min({object1->Restitution(), object2.Restitution()});
    const sf::Vector2f new_velocity1 = v1 - (1 + restitution) * v1.dot(normal) * normal;
    object1->SetVelocity(new_velocity1);
}

void Collide(const RigidBody& object1, RigidBody* object2) {
    Collide(object2, object1);
}

void Collide(RigidBody* object1, RigidBody* object2) {
    const Collider* const collider1 = object1->GetCollider();
    const Collider* const collider2 = object2->GetCollider();
    Collider::TYPE collider_type1 = collider1->Type();
    Collider::TYPE collider_type2 = collider2->Type();
    const Transform* const transform1 = object1->GetTransform();
    const Transform* const transform2 = object2->GetTransform();
    auto optional_normal = collision_matrix[static_cast<uint32_t>(collider_type1)]
        [static_cast<uint32_t>(collider_type2)](*collider1, *transform1, *collider2, *transform2);
    if (!optional_normal.has_value()) {
        return;
    }
    const sf::Vector2f normal = std::move(optional_normal).value();
    const sf::Vector2f v1 = object1->Velocity();
    const sf::Vector2f v2 = object2->Velocity();
    const sf::Vector2f v_rel = v2 - v1;
    if (v_rel.dot(normal) > 0.0f) {
        return;
    }
    const float inv_m1 = object1->InverseMass();
    const float inv_m2 = object2->InverseMass();
    const float restitution = std::min({object1->Restitution(), object2->Restitution()});
    const sf::Vector2f helper_vector = -(1 + restitution) * (v2 - v1).dot(normal) / (inv_m1 + inv_m2) * normal;
    const sf::Vector2f new_velocity1 = v1 - inv_m1 * helper_vector;
    const sf::Vector2f new_velocity2 = v2 + inv_m2 * helper_vector;
    object1->SetVelocity(new_velocity1);
    object2->SetVelocity(new_velocity2);
}
