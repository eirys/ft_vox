/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   descriptor_table.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 19:55:31 by etran             #+#    #+#             */
/*   Updated: 2024/03/07 13:52:14 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "descriptor_table.h"
#include "mvp_set.h"

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
}

void DescriptorTable::destroy(const Device& device) {
    for (IDescriptorSet* set : m_sets) set->destroy(device);
}

/* ========================================================================== */

IDescriptorSet const* DescriptorTable::operator[](const DescriptorSetIndex index) const noexcept {
    return m_sets[(u32)index];
}

IDescriptorSet* DescriptorTable::operator[](const DescriptorSetIndex index) noexcept {
    return m_sets[(u32)index];
}

} // namespace vox::gfx