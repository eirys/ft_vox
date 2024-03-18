/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:55:31 by etran             #+#    #+#             */
/*   Updated: 2024/03/18 11:05:57 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_table.h"
#include "mvp_set.h"
#include "world_set.h"

#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorTable::DescriptorTable() {
    m_sets[(u32)DescriptorSetIndex::Mvp] = new MVPSet();
    m_sets[(u32)DescriptorSetIndex::WorldData] = new WorldSet();
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

    WorldSet* world = (WorldSet*)m_sets[(u32)DescriptorSetIndex::WorldData];
    world->update(device, state, cmdBuffer);

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
    MVPSet* mvp = (MVPSet*)m_sets[(u32)DescriptorSetIndex::Mvp];
    mvp->update(state);
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