/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 22:37:05 by etran             #+#    #+#             */
/*   Updated: 2023/11/19 22:55:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <string> // std::string

namespace scop::core {
class Device;
}

namespace scop::gfx {

VkShaderModule	createShaderModule(
	scop::core::Device& device,
	const std::string& path);

} // namespace scop::gfx
