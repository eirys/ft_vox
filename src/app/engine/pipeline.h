/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 03:12:47 by etran             #+#    #+#             */
/*   Updated: 2023/10/06 12:39:09 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# include <vulkan/vulkan.h>

# include "render_pass.h"
# include "texture_handler.h"
# include "target.h"

// Std
# include <memory> // std::shared_ptr
# include <string> // std::string

namespace scop {
class Image;
struct UniformBufferObject;
struct Camera;
}

namespace scop::core {
class Device;
class CommandBuffer;
}

namespace scop::gfx {

class InputHandler;
class DescriptorSet;

class Pipeline {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using RenderPassPtr = std::shared_ptr<RenderPass>;
	using TextureHandlerPtr = std::shared_ptr<TextureHandler>;
	using TargetPtr = std::shared_ptr<Target>;
	using DescriptorSetPtr = std::shared_ptr<DescriptorSet>;

	using Texture = scop::Image;
	using UniformBufferObject = scop::UniformBufferObject;
	using Camera = scop::Camera;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	virtual ~Pipeline() = default;

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
		scop::core::CommandBuffer& command_buffer,
		const InputHandler& input,
		int32_t image_index) = 0;
	virtual void		update(const UniformBufferObject& ubo) noexcept;

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

	Pipeline() = default;
	Pipeline(Pipeline&& other) = default;
	Pipeline& operator=(Pipeline&& other) = default;

	Pipeline(const Pipeline& other) = delete;
	Pipeline& operator=(const Pipeline& other) = delete;

	/* ========================================================================= */

	virtual void		_beginRenderPass(
		scop::core::CommandBuffer& command_buffer,
		int32_t image_index = 0) = 0;

	/* UTILS =================================================================== */

	VkShaderModule		_createShaderModule(
		scop::core::Device& device,
		const std::string& path);

}; // class Pipeline

} // namespace scop::gfx