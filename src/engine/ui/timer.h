/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 22:51:33 by etran             #+#    #+#             */
/*   Updated: 2024/09/11 15:50:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <chrono>

#include "types.h"
#include "debug.h"

namespace ui {

class Timer final {
public:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using Clock = std::chrono::high_resolution_clock;

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32    LOG_INTERVAL = 3;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init() noexcept {
        m_startTime = Clock::now();
        m_counter = 0;
    }

    void update() noexcept {
        static auto LOG_INTERVAL_S = std::chrono::seconds(LOG_INTERVAL);

        ++m_counter;

        const Clock::time_point now = Clock::now();
        const u32               elapsed = (u32)std::chrono::duration<f32>(now - m_startTime).count();

        if (elapsed >= LOG_INTERVAL) {
            u32 fps = m_counter / elapsed;
            LLOG("FPS: " << fps << " | Drawing "<< vox::gfx::ChunkGfx::chunksDrawn << " chunks.");
            init();
        }
    }

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    Clock::time_point   m_startTime;
    u32                 m_counter = 0;

}; // class Timer

} // namespace ui
