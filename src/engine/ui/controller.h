/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 19:33:01 by etran             #+#    #+#             */
/*   Updated: 2024/05/30 22:23:46 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"
#include "game_decl.h"
#include "types.h"

namespace ui {

class Window;

struct Camera {
    static constexpr f32    NEAR_PLANE = 0.1f;
    static constexpr f32    FAR_PLANE = 500.0f;
    static constexpr f32    ASPECT_RATIO = 1200.0f / 800.0f;

    math::Vect3 m_position = {RENDER_DISTANCE / 2.0 * CHUNK_SIZE, 20.0f, RENDER_DISTANCE / 2.0 * CHUNK_SIZE};
    math::Vect3 m_front;
    math::Vect3 m_right;
    math::Vect3 m_up;
    float       m_fov = 85.0f;
};

/**
 * @brief Player controller
 */
class Controller final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    Controller() = default;
    ~Controller() = default;

    Controller(Controller&& x) = delete;
    Controller(const Controller& x) = delete;
    Controller& operator=(Controller&& rhs) = delete;
    Controller& operator=(const Controller& rhs) = delete;

    /* ====================================================================== */

    void init(const Window& win);
    void update(const Window& win);

    const Camera&   getCamera() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Camera  m_camera;

    float   m_lastX = 0.0f;
    float   m_lastY = 0.0f;

    float   m_yaw = 0.0f;
    float   m_pitch = 0.0f;

}; // class Controller

} // namespace ui