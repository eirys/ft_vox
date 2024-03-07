/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_decl.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:55:18 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 14:57:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"

namespace vox::gfx {

struct RecordInfo {
    virtual ~RecordInfo() = default;

    u32 m_targetIndex = 0;
};

} // namespace vox::gfx
