#pragma once

#include "physics.hpp"

class PhysicsManager {
   public:
    enum class STATE {ACTIVE, PAUSED};

    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;
    PhysicsManager(PhysicsManager&&) = delete;
    PhysicsManager& operator=(PhysicsManager&&) = delete;

    static PhysicsManager& Instance();

    template <typename T>
        requires std::derived_from<T, RigidBody>
    void AddObject(T* object) {
        rigid_bodies_.push_back(&object);
    }
    
    template <typename T>
        requires std::derived_from<T, RigidBody>
    void AddObjects(std::vector<T>* objects) {
        for (auto& object : *objects) {
            rigid_bodies_.push_back(&object);
        }
    }
    
    void MoveObjects(float);
    
    void CollideObjects();

    void Simulate(float);

    STATE State() const;

    void SetState(STATE);

   private:
    PhysicsManager() = default;
    
    std::vector<RigidBody*> rigid_bodies_;
    STATE state_{STATE::ACTIVE};
};
