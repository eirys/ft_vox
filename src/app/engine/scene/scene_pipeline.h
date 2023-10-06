/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_pipeline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 17:31:39 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:37:51 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Std
# include <memory> // std::shared_ptr

# include "pipeline.h"
# include "buffer.h"

namespace scop::graphics {

class Device;

class ScenePipeline final: public Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using super = Pipeline;
	using super::Texture;
	using super::DescriptorSetPtr;

	using super::UniformBufferObject;
	using super::Camera;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	ScenePipeline();
	~ScenePipeline() = default;

	ScenePipeline(ScenePipeline&& other) = delete;
	ScenePipeline(const ScenePipeline& other) = delete;
	ScenePipeline& operator=(ScenePipeline&& other) = delete;
	ScenePipeline& operator=(const ScenePipeline& other) = delete;

	/* ========================================================================= */

	void	init(
		Device& device,
		RenderPass::RenderPassInfo& rp_info,
		Target::TargetInfo& tar_info) override;
	void	assemble(
		Device& device,
		VkGraphicsPipelineCreateInfo& info) override;
	void	plugDescriptor(
		Device& device,
		TextureHandlerPtr shadowmap,
		TextureHandlerPtr heightmap);
	void	destroy(Device& device) override;

	void	draw(
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		int32_t image_index) override;
	void	update(const UniformBufferObject& ubo) noexcept override;
	void	update(const Camera& camera) noexcept override;

	/* ========================================================================= */

	using super::getPipeline;
	using super::getRenderPass;
	using super::getTextureHandler;
	using super::getTarget;
	using super::getDescriptor;

	Buffer&	getUbo() noexcept;

private:
	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void	_beginRenderPass(
		CommandBuffer& command_buffer,
		int32_t image_index = 0) override;

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	Buffer					_ubo;

}; // class ScenePipeline

} // namespace scop::graphics