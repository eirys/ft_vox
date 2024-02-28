/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:42:53 by etran             #+#    #+#             */
/*   Updated: 2024/02/28 23:59:42 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>

#include "idescriptor_set.h"

namespace vox::gfx {

class Device;

/**
 * @brief Descriptor sets table.
 * @note This class will be used to store all the descriptor sets.
*/
class DescriptorTable final {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    DescriptorTable();

    ~DescriptorTable() = default;

    DescriptorTable(DescriptorTable&& other) = delete;
    DescriptorTable(const DescriptorTable& other) = delete;
    DescriptorTable& operator=(DescriptorTable&& other) = delete;
    DescriptorTable& operator=(const DescriptorTable& other) = delete;

    /* ====================================================================== */

    void init(const Device& device);
    void destroy(const Device& device);

/* ========================================================================== */

    IDescriptorSet const*    operator[](const u32 index) const noexcept;
    IDescriptorSet*          operator[](const u32 index) noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::vector<IDescriptorSet*>  m_sets;

}; // class DescriptorTable

} // namespace vox::gfx
