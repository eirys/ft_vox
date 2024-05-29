/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:10:34 by etran             #+#    #+#             */
/*   Updated: 2024/05/28 12:15:53 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "types.h"

namespace game {

class Block final {
public:

    Block(const u8 data) noexcept
        : m_data(data) {
    }

    Block() = default;
    ~Block() = default;

    /* ====================================================================== */

    explicit operator u8() const noexcept {
        return m_data;
    }

    /* ====================================================================== */

    bool isVoid() const noexcept {
        return m_data == 0;
    }

private:

    u8 m_data = 0;

}; // class Block


} // namespace game