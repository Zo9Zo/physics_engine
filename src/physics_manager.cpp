#include "physics.hpp"
#include "physics_manager.hpp"

PhysicsManager& PhysicsManager::Instance() {
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::MoveObjects(float dt) {
    for (auto& object : rigid_bodies_) {
        if (object->Type() == RigidBody::TYPE::STATIC) {
            continue;
        }
        if (object->InverseMass() > Config::epsilon) {
            object->AddForce({0.0f, -1.0f / object->InverseMass() * Config::g});
        }
        object->Move(dt);
    }
}

void PhysicsManager::CollideObjects() {
    for (int i = 0; i < rigid_bodies_.size(); ++i) {
        RigidBody* rigid_body1 = rigid_bodies_[i];
        for (int j = i + 1; j < rigid_bodies_.size(); ++j) {
            RigidBody* rigid_body2 = rigid_bodies_[j];
            if (rigid_body1->Type() == RigidBody::TYPE::STATIC &&
                rigid_body2->Type() == RigidBody::TYPE::STATIC) {
                continue;
            }
            Collide(rigid_body1, rigid_body2);
        }
    }
}

void PhysicsManager::Simulate(float dt) {
    if (state_ == STATE::PAUSED) {
        return;
    }
    CollideObjects();
    MoveObjects(dt);
}

PhysicsManager::STATE PhysicsManager::State() const {
    return state_;
}

void PhysicsManager::SetState(PhysicsManager::STATE state) {
    state_ = state;
}
