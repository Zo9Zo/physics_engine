#pragma once

#include <vector>

#include "physics.hpp"

class Renderer {
   public:
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    static Renderer& Instance();
    void Render(std::vector<Particle>*) const;
   
   private:
    Renderer() = default;
};
