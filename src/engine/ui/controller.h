/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 19:33:01 by etran             #+#    #+#             */
/*   Updated: 2024/03/17 01:14:47 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.h"

namespace ui {

class Window;

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

    const math::Vect3&  getView() const;
    const math::Vect3&  getPosition() const noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    float   m_lastX = 0.0f;
    float   m_lastY = 0.0f;

    float   m_yaw = 0.0f;
    float   m_pitch = 0.0f;

    math::Vect3 m_direction = {0.0f, 0.0f, 0.0f};
    math::Vect3 m_position = {0.0f, 0.0f, 0.0f};

}; // class Controller

} // namespace ui