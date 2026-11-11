#pragma once

#include <SFML/Graphics.hpp>

namespace Config {
    inline constexpr sf::Vector2u window_resolution(800, 600);
    inline constexpr uint32_t max_fps = 120;

    inline constexpr float particle_radius = 10.0f;
    inline constexpr float particle_mass = 10.0f;
    // inline constexpr float max_velocity = 50.0f;

    // inline constexpr float epsilon = 1e-9;
    inline constexpr float dt = 1.0f;
    inline constexpr float g = 98.1f;
};