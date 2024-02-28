/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:55:31 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 00:01:37 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_table.h"
#include "mvp_set.h"

namespace vox::gfx {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

DescriptorTable::DescriptorTable() {
    m_sets.reserve(DESCRIPTOR_TABLE_SIZE);
    m_sets[(u32)DescriptorIndex::Mvp] = new MVPSet();
}

/* ========================================================================== */

void DescriptorTable::init(const Device& device) {
    for (IDescriptorSet* set : m_sets)
        set->init(device);
}

void DescriptorTable::destroy(const Device& device) {
    for (IDescriptorSet* set : m_sets)
        set->destroy(device);
}

/* ========================================================================== */

IDescriptorSet const* DescriptorTable::operator[](const u32 index) const noexcept {
    return m_sets[index];
}

IDescriptorSet* DescriptorTable::operator[](const u32 index) noexcept {
    return m_sets[index];
}

} // namespace vox::gfx