/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_pipeline.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 09:24:16 by etran             #+#    #+#             */
/*   Updated: 2023/08/16 09:24:16 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace scop::graphics {

class ShadowsPipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Pipeline;
	using Texture = super::Texture;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ShadowsPipeline() = default;
	~ShadowsPipeline() = default;

	ShadowsPipeline(ShadowsPipeline&& other) = delete;
	ShadowsPipeline(const ShadowsPipeline& other) = delete;
	ShadowsPipeline& operator=(ShadowsPipeline&& other) = delete;
	ShadowsPipeline& operator=(const ShadowsPipeline& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void	init(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info,
		Target::TargetInfo& tar_info,
		const std::vector<Texture>& textures,
		VkGraphicsPipelineCreateInfo& info) override;
	void	draw(
		Device& device,
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		InputHandler& input,
		int32_t image_index) override;
	void	update(const ::scop::UniformBufferObject& ubo) noexcept override;

	void	setDescriptor(super::DescriptorSetPtr desc_ptr) noexcept;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getRenderPass;
	using super::getTextureHandler;
	using super::getTarget;
	using super::getDescriptor;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	_createRenderPass(
		Device& device,
		const RenderPass::RenderPassInfo& rp_info);
	void	_createTarget(
		Device& device,
		Target::TargetInfo& info) override;
	void	_createPipeline(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;

}; // class ShadowsPipeline

} // namespace scop::graphics