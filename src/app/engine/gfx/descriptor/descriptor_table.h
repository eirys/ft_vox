/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:42:53 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 16:19:45 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "descriptor_set.h"
#include "types.h"

namespace vox::gfx {

class DescriptorTable final {
public:
    /* ====================================================================== */
    /*                                  ENUMS                                 */
    /* ====================================================================== */

    enum class DescriptorIndex: u32 {

        First = 0,
        Last = 0
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr u32 DESCRIPTOR_TABLE_COUNT = enumSize<DescriptorIndex>();

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DescriptorTable() = default;
    ~DescriptorTable() = default;

    DescriptorTable(DescriptorTable&& other) = delete;
    DescriptorTable(const DescriptorTable& other) = delete;
    DescriptorTable& operator=(DescriptorTable&& other) = delete;
    DescriptorTable& operator=(const DescriptorTable& other) = delete;

    /* ====================================================================== */

    void init();
    void destroy();

/* ========================================================================== */

    const DescriptorSet&    operator[](const DescriptorIndex index) const noexcept;
    DescriptorSet&          operator[](const DescriptorIndex index) noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<DescriptorSet>  m_sets;

}; // class DescriptorTable


};

} // namespace vox::gfx
