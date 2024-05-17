/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:23:55 by etran             #+#    #+#             */
/*   Updated: 2024/03/02 12:30:22 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "enum.h"
#include "types.h"

namespace vox::gfx {

/* FENCES =================================================================== */

enum class FenceIndex: u32 {
    DrawInFlight    = 0,    // Signals that a graphics command is ongoing.
    ComputeInFlight,        // Signals that a compute command is ongoing.

    First = DrawInFlight,
    Last = ComputeInFlight
};

constexpr u32 FENCE_COUNT = enumSize<FenceIndex>();

/* SEMAPHORES =============================================================== */

enum class SemaphoreIndex: u32 {
    ImageAvailable  = 0,    // Signals that an image is available for rendering.
    RenderFinished,         // Signals that rendering has finished.
    ComputeFinished,        // Signals that a compute command has finished.
    GraphicsFinished,       // Signals that a graphics command has finished.

    First = ImageAvailable,
    Last = GraphicsFinished
};

constexpr u32 SEMAPHORE_COUNT = enumSize<SemaphoreIndex>();

} // namespace vox::gfx