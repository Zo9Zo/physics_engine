#include "physics.hpp"
#include "physics_manager.hpp"

PhysicsManager& PhysicsManager::Instance() {
    static PhysicsManager instance;
    return instance;
}

void PhysicsManager::MoveObjects(float dt) {
    for (auto& object : dynamic_objects_) {
        object->AddForce({0.0f, -object->Mass() * Config::g});
        object->Move(dt);
    }
}

void PhysicsManager::CollideObjects() {
    for (int i = 0; i < dynamic_objects_.size(); ++i) {
        DynamicObject* dynamic_object1 = dynamic_objects_[i];
        for (int j = 0; j < static_objects_.size(); ++j) {
            const StaticObject& static_object = *static_objects_[j];
            Collide(dynamic_object1, static_object);
        }
        for (int j = i + 1; j < dynamic_objects_.size(); ++j) {
            DynamicObject* dynamic_object2 = dynamic_objects_[j];
            Collide(dynamic_object1, dynamic_object2);
        }
    }
}

void PhysicsManager::Simulate(float dt) {
    CollideObjects();
    MoveObjects(dt);
}
