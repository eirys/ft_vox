/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_pipeline.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 09:24:16 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:38:02 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "pipeline.h"

namespace scop::gfx {

class Buffer;

class ShadowsPipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Pipeline;
	using super::Texture;
	using super::DescriptorSetPtr;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ShadowsPipeline();
	~ShadowsPipeline() = default;

	ShadowsPipeline(ShadowsPipeline&& other) = delete;
	ShadowsPipeline(const ShadowsPipeline& other) = delete;
	ShadowsPipeline& operator=(ShadowsPipeline&& other) = delete;
	ShadowsPipeline& operator=(const ShadowsPipeline& other) = delete;

	/* ========================================================================= */

	using super::destroy;

	void	init(
		Device& device,
		RenderPassInfo& rp_info,
		TargetInfo& tar_info) override;
	void	assemble(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;
	void	plugDescriptor(
		Device& device,
		Buffer& ubo,
		const InputHandler& input);
	void	draw(
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		const InputHandler& input,
		int32_t image_index) override;

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

	void		_beginRenderPass(
		CommandBuffer& command_buffer,
		int32_t image_index = 0) override;

}; // class ShadowsPipeline

} // namespace scop::gfx