/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clouds_pipeline.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 01:30:39 by etran             #+#    #+#             */
/*   Updated: 2024/03/19 11:01:38 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clouds_pipeline.h"

namespace vox::gfx {

enum class CloudsDescriptorSet: u32 {
    Mvp = 0,

    First = Mvp,
    Last = Mvp
};

static constexpr u32 DESCRIPTOR_SET_COUNT = enumSize<CloudsDescriptorSet>();

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

CloudsPipeline::CloudsPipeline() {
    m_renderPass = new CloudsRenderPass();
}

} // namespace vox::gfx