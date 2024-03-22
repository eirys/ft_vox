/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:42:53 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 11:06:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <array>

#include "idescriptor_set.h"

namespace game {
class GameState;
}

namespace vox::gfx {

class Device;
class ICommandBuffer;

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
    ~DescriptorTable();

    DescriptorTable(DescriptorTable&& other) = delete;
    DescriptorTable(const DescriptorTable& other) = delete;
    DescriptorTable& operator=(DescriptorTable&& other) = delete;
    DescriptorTable& operator=(const DescriptorTable& other) = delete;

    /* ====================================================================== */

    void init(
        const Device& device,
        const ICommandBuffer* cmdBuffer,
        const game::GameState& state);
    void destroy(const Device& device);

    void fill(const Device& device);
    void update(const game::GameState& state);

/* ========================================================================== */

    IDescriptorSet const*    operator[](const DescriptorSetIndex index) const noexcept;
    IDescriptorSet*          operator[](const DescriptorSetIndex index) noexcept;

    IDescriptorSet const*    operator[](const u32 index) const noexcept;
    IDescriptorSet*          operator[](const u32 index) noexcept;

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    std::array<IDescriptorSet*, DESCRIPTOR_TABLE_SIZE>  m_sets{};

}; // class DescriptorTable

} // namespace vox::gfx