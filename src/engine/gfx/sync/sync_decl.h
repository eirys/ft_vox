/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_decl.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:23:55 by etran             #+#    #+#             */
/*   Updated: 2024/06/13 23:17:17 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "enum.h"
#include "types.h"

namespace vox::gfx {

/* FENCES =================================================================== */

enum class FenceIndex: u32 {
    DrawInFlight,       // Signals that a graphics command is ongoing.

    Count
};

constexpr u32 FENCE_COUNT = (u32)FenceIndex::Count;

/* SEMAPHORES =============================================================== */

enum class SemaphoreIndex: u32 {
    ImageAvailable,         // Signals that an image is available for rendering.
    RenderFinished,         // Signals that rendering has finished.
    OffscreenFinished,      // Signals that offscreen rendering has finished.

    Count
};

constexpr u32 SEMAPHORE_COUNT = (u32)SemaphoreIndex::Count;

} // namespace vox::gfx