/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mvp_set.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:38:00 by etran             #+#    #+#             */
/*   Updated: 2024/02/29 22:40:32 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../descriptor_set.h"

namespace vox::gfx {

// Placeholder
class MVPSet final: public DescriptorSet {
private:
    /* ====================================================================== */
    /*                                TYPEDEFS                                */
    /* ====================================================================== */

    using super = DescriptorSet;

public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    MVPSet(): super(DescriptorSetIndex::Mvp) {}

    void init(const Device& device) override {
        (void)device;
    }

private:

};

} // namespace vox::gfx