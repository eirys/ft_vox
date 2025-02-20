#pragma once

#include "types.h"
#include "vector.h"

namespace game {

struct Sun final {

    math::vec3  m_direction = math::vec3( std::cos(M_PI * 0.25), std::sin(M_PI * 0.25), 0.0f);
    float       m_rotationSpeed = 0.1f * M_PI;

    void update(float time) {
        m_direction = math::vec3(
            std::cos(time * m_rotationSpeed),
            std::sin(time * m_rotationSpeed),
            0.0f);
    }
}; // struct Sun

} // namespace game