#include <cmath>
#include <cstdint>
#include <optional>

#include "config.hpp"
#include "physics.hpp"

Collider::TYPE CircleCollider::GetType() const {
    return Collider::TYPE::CIRCLE;
}

float CircleCollider::Radius() const {
    return radius_;
}

Collider::TYPE LineCollider::GetType() const {
    return Collider::TYPE::LINE;
}

LineCollider::DIRECTION LineCollider::Direction() const {
    return direction_;
}

Transform* StaticObject::GetTransform() { 
    return &transform_;
}

const Transform& StaticObject::GetTransform() const { 
    return transform_;
}

const Collider* StaticObject::GetCollider() const {
    return collider_.get();
}

float DynamicObject::Mass() const { 
    return mass_;
}

sf::Vector2f DynamicObject::Velocity() const {
    return velocity_;
}

Transform* DynamicObject::GetTransform() { 
    return &transform_;
}

const Transform& DynamicObject::GetTransform() const { 
    return transform_;
}

const Collider* DynamicObject::GetCollider() const {
    return collider_.get();
}

void DynamicObject::AddForce(const sf::Vector2f& force) {
    force_ += force;
}

void DynamicObject::SetVelocity(const sf::Vector2f& velocity) {
    velocity_ = velocity;
}

void DynamicObject::Move(float dt) {
    velocity_ += force_ / mass_ * dt;
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

// Circle---Circle
std::optional<sf::Vector2f> CollisionNormal(const CircleCollider& circle_collider1,
    const Transform& circle_transform1, const CircleCollider& circle_collider2,
    const Transform& circle_transform2) {
    if (!HaveCollided(circle_collider1, circle_transform1, circle_collider2, circle_transform2)) {
        return std::nullopt;
    }
    sf::Vector2f r = circle_transform1.position - circle_transform2.position;
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
        return sf::Vector2f{0.0f, 1.0f};
    }
    return sf::Vector2f{1.0f, 0.0f};
}

// Line---Circle
std::optional<sf::Vector2f> CollisionNormal(const LineCollider& line_collider,
    const Transform& line_transform, const CircleCollider& circle_collider,
    const Transform& circle_transform) {
    return CollisionNormal(circle_collider, circle_transform, line_collider, line_transform);
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

void Collide(DynamicObject* object1, const StaticObject& object2) {
    const Collider* const collider1 = object1->GetCollider();
    const Collider* const collider2 = object2.GetCollider();
    Collider::TYPE collider_type1 = collider1->GetType();
    Collider::TYPE collider_type2 = collider2->GetType();
    const Transform* const transform1 = object1->GetTransform();
    const Transform* const transform2 = &object2.GetTransform();
    auto optional_normal = collision_matrix[static_cast<uint32_t>(collider_type1)]
        [static_cast<uint32_t>(collider_type2)](*collider1, *transform1, *collider2, *transform2);
    if (!optional_normal.has_value()) {
        return;
    }
    const sf::Vector2f normal = std::move(optional_normal).value();
    const sf::Vector2f v = object1->Velocity();
    const sf::Vector2f new_velocity = v - 2 * (v.dot(normal)) * normal;
    object1->SetVelocity(new_velocity);
}

void Collide(DynamicObject* object1, DynamicObject* object2) {
    const Collider* const collider1 = object1->GetCollider();
    const Collider* const collider2 = object2->GetCollider();
    Collider::TYPE collider_type1 = collider1->GetType();
    Collider::TYPE collider_type2 = collider2->GetType();
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
    const float m1 = object1->Mass();
    const float m2 = object2->Mass();
    const sf::Vector2f helper_vector = 2.0f / (m1 + m2) * ((v1 - v2).dot(normal)) * normal;
    const sf::Vector2f new_velocity1 = v1 - m2 * helper_vector;
    const sf::Vector2f new_velocity2 = v2 + m1 * helper_vector;
    object1->SetVelocity(new_velocity1);
    object2->SetVelocity(new_velocity2);
}
