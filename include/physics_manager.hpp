#pragma once

#include "physics.hpp"

class PhysicsManager {
   public:
    PhysicsManager(const PhysicsManager&) = delete;
    PhysicsManager& operator=(const PhysicsManager&) = delete;
    PhysicsManager(PhysicsManager&&) = delete;
    PhysicsManager& operator=(PhysicsManager&&) = delete;

    static PhysicsManager& Instance();

    template <typename T>
        requires std::derived_from<T, StaticObject>
    void AddObject(T* object) {
        static_objects_.push_back(&object);
    }
    
    template <typename T>
        requires std::derived_from<T, DynamicObject>
    void AddObject(T* object) {
        dynamic_objects_.push_back(&object);
    }
    
    template <typename T>
        requires std::derived_from<T, StaticObject>
    void AddObjects(std::vector<T>* objects) {
        for (auto& object : *objects) {
            static_objects_.push_back(&object);
        }
    }
    
    template <typename T>
        requires std::derived_from<T, DynamicObject>
    void AddObjects(std::vector<T>* objects) {
        for (auto& object : *objects) {
            dynamic_objects_.push_back(&object);
        }
    }
    
    void MoveObjects(float);
    
    void CollideObjects();

    void Simulate(float);

   private:
    PhysicsManager() = default;
    
    std::vector<StaticObject*> static_objects_;
    std::vector<DynamicObject*> dynamic_objects_;
};
