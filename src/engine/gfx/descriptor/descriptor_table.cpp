/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:55:31 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 15:47:03 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_table.h"
#include "mvp_set.h"
#include "debug.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorTable::DescriptorTable() {
    m_sets[(u32)DescriptorSetIndex::Mvp] = new MVPSet();
}

DescriptorTable::~DescriptorTable() {
    for (const IDescriptorSet* set : m_sets)
        delete set;
}

/* ========================================================================== */

void DescriptorTable::init(const Device& device) {
    for (IDescriptorSet* set : m_sets) set->init(device);

    LINFO("Descriptor table initialized.");
}

void DescriptorTable::destroy(const Device& device) {
    for (IDescriptorSet* set : m_sets) set->destroy(device);

    LINFO("Descriptor table destroyed.");
}

void DescriptorTable::fill(const Device& device, const GameState& state) {
    for (IDescriptorSet* set : m_sets) set->fill(device, state);

    LINFO("Descriptor table flled up.");
}

/* ========================================================================== */

IDescriptorSet const* DescriptorTable::operator[](const DescriptorSetIndex index) const noexcept {
    return m_sets[(u32)index];
}

IDescriptorSet* DescriptorTable::operator[](const DescriptorSetIndex index) noexcept {
    return m_sets[(u32)index];
}

} // namespace vox::gfx