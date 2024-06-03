/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:23:55 by etran             #+#    #+#             */
/*   Updated: 2024/06/03 09:48:25 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "enum.h"
#include "types.h"

namespace vox::gfx {

/* FENCES =================================================================== */

enum class FenceIndex: u32 {
    DrawInFlight    = 0,    // Signals that a graphics command is ongoing.
    // ComputeInFlight,        // Signals that a compute command is ongoing.

    Count,

    First = DrawInFlight,
    Last = DrawInFlight
};

constexpr u32 FENCE_COUNT = (u32)FenceIndex::Count;

/* SEMAPHORES =============================================================== */

enum class SemaphoreIndex: u32 {
    ImageAvailable,         // Signals that an image is available for rendering.
    RenderFinished,         // Signals that rendering has finished.
    // ComputeFinished,        // Signals that a compute command has finished.
    // GraphicsFinished,       // Signals that a graphics command has finished.
    // VertexBufferReady,

    Count
};

constexpr u32 SEMAPHORE_COUNT = (u32)SemaphoreIndex::Count;

} // namespace vox::gfx