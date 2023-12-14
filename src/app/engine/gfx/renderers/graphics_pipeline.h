/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:12:47 by etran             #+#    #+#             */
/*   Updated: 2023/11/18 20:57:08 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

// Std
# include <memory> // std::shared_ptr

namespace scop {
struct UniformBufferObject;
class InputHandler;
}

namespace scop::core {
class Device;
}

namespace scop::gfx {

class CommandBuffer;

class RenderPass;
class TextureHandler;
class Target;
class DescriptorSet;

struct RenderPassInfo;
struct TargetInfo;

class GraphicsPipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using RenderPassPtr = std::shared_ptr<RenderPass>;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;
	using TargetPtr = std::shared_ptr<Target>;
	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~GraphicsPipeline() = default;

	/* ========================================================================= */

	virtual void		init(
		scop::core::Device& device,
		RenderPassInfo& rp_info,
		TargetInfo& tar_info) = 0;
	virtual void		assemble(
		scop::core::Device& device,
		VkGraphicsPipelineCreateInfo& info) = 0;
	virtual void		destroy(scop::core::Device& device);
	virtual void		draw(
		VkPipelineLayout layout,
		CommandBuffer& command_buffer,
		const scop::InputHandler& input,
		int32_t image_index) = 0;

	/* ========================================================================= */

	virtual void		update(const scop::UniformBufferObject& ubo) noexcept;

	/* ========================================================================= */

	VkPipeline			getPipeline() const noexcept;
	TextureHandlerPtr	getTextureHandler() const noexcept;
	RenderPassPtr		getRenderPass() const noexcept;
	TargetPtr			getTarget() const noexcept;
	DescriptorSetPtr	getDescriptor() const noexcept;

protected:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkPipeline			_pipeline = VK_NULL_HANDLE;

	TextureHandlerPtr	_texture;
	RenderPassPtr		_render_pass;
	TargetPtr			_target;
	DescriptorSetPtr	_descriptor;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	GraphicsPipeline() = default;
	GraphicsPipeline(GraphicsPipeline&& other) = default;
	GraphicsPipeline& operator=(GraphicsPipeline&& other) = default;

	GraphicsPipeline(const GraphicsPipeline& other) = delete;
	GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

	/* ========================================================================= */

	virtual void		_beginRenderPass(
		CommandBuffer& command_buffer,
		int32_t image_index = 0) = 0;

}; // class GraphicsPipeline

} // namespace scop::gfx