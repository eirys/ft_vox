/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:55:31 by etran             #+#    #+#             */
/*   Updated: 2024/06/14 00:02:12 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_table.h"
#include "pfd_set.h"
#include "world_set.h"
#include "gbuffer_set.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorTable::DescriptorTable() {
    m_sets[(u32)DescriptorSetIndex::Pfd] = new PFDSet();
    m_sets[(u32)DescriptorSetIndex::WorldData] = new WorldSet();
    m_sets[(u32)DescriptorSetIndex::GBuffer] = new GBufferSet();
}

DescriptorTable::~DescriptorTable() {
    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i) delete m_sets[i];
}

/* ========================================================================== */

void DescriptorTable::init(
    const Device& device,
    const ICommandBuffer* cmdBuffer,
    const game::GameState& state
) {
    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i) m_sets[i]->init(device, cmdBuffer);

    LINFO("Descriptor table initialized.");
}

void DescriptorTable::destroy(const Device& device) {
    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i) m_sets[i]->destroy(device);

    LINFO("Descriptor table destroyed.");
}

void DescriptorTable::fill(const Device& device) {
    for (u32 i = 0; i < DESCRIPTOR_TABLE_SIZE; ++i) m_sets[i]->fill(device);

    LINFO("Descriptor table flled up.");
}

void DescriptorTable::update(const game::GameState& state) {
    PFDSet* pfd = (PFDSet*)m_sets[(u32)DescriptorSetIndex::Pfd];
    pfd->update(state);
}

/* ========================================================================== */

IDescriptorSet const* DescriptorTable::operator[](const DescriptorSetIndex index) const noexcept {
    return m_sets[(u32)index];
}

IDescriptorSet* DescriptorTable::operator[](const DescriptorSetIndex index) noexcept {
    return m_sets[(u32)index];
}

IDescriptorSet const* DescriptorTable::operator[](const u32 index) const noexcept {
    return m_sets[(u32)index];
}

IDescriptorSet* DescriptorTable::operator[](const u32 index) noexcept {
    return m_sets[(u32)index];
}

} // namespace vox::gfx