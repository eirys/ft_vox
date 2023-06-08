/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/06/08 21:46:58 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Graphics
# ifndef GLFW_INCLUDE_VULKAN
#  define GLFW_INCLUDE_VULKAN
# endif

# include <GLFW/glfw3.h>

// Std
# include <optional>	// std::optional
# include <vector>		// std::vector

# include "debug_module.h"
# include "device.h"
# include "render_target.h"
# include "texture_sampler.h"
# include "descriptor_set.h"
# include "command_buffer.h"
# include "vertex_input.h"
# include "player.h"

namespace scop {
class Timer;

namespace graphics {

struct QueueFamilyIndices {
	std::optional<uint32_t>	graphics_family;
	std::optional<uint32_t>	present_family;

	bool	isComplete() {
		return graphics_family.has_value() && present_family.has_value();
	}
};

class Engine {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static const std::vector<const char*>	validation_layers;
	static constexpr std::size_t			max_frames_in_flight = 1;

	#ifndef NDEBUG
	static constexpr bool					enable_validation_layers = false;
	#else
	static constexpr bool					enable_validation_layers = true;
	#endif

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Engine() = default;
	Engine(Engine&& other) = default;
	~Engine() = default;

	Engine(const Engine& other) = delete;
	Engine& operator=(const Engine& other) = delete;

	/* ========================================================================= */

	void						init(
		scop::Window& window,
		const scop::Image& image,
		const UniformBufferObject::Light& light,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void						destroy();

	void						idle();
	void						render(
		scop::Window& window,
		const vox::Player& player,
		Timer& timer
	);

private:
	/* ========================================================================= */
	/*                               HELPER OBJECTS                              */
	/* ========================================================================= */

	struct PushConstantData {
		int32_t	image_index;
	};

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static constexpr const char*	vertex_shader_bin = "shaders/vert.spv";
	static constexpr const char*	fragment_shader_bin = "shaders/frag.spv";

	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance						vk_instance;
	DebugModule						debug_module;

	Device							device;

	RenderTarget					render_target;
	TextureSampler					texture_sampler;
	DescriptorSet					descriptor_set;
	CommandBuffer					command_buffer;
	VertexInput						vertex_input;

	VkSemaphore						image_available_semaphores;
	VkSemaphore						render_finished_semaphores;
	VkFence							in_flight_fences;

	VkPipelineLayout				pipeline_layout;
	VkPipeline						engine;

	std::size_t						nb_indices;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	/* INIT ==================================================================== */

	void							createInstance();
	void							createGraphicsPipeline();
	void							createSyncObjects();

	bool							checkValidationLayerSupport();
	std::vector<const char*>		getRequiredExtensions();
	VkShaderModule					createShaderModule(
		const std::vector<char>& code
	);
	void							recordCommandBuffer(
		std::size_t indices_size,
		VkCommandBuffer command_buffer,
		uint32_t image_index
	);

}; // class Engine

/* ========================================================================== */
/*                                    UTILS                                   */
/* ========================================================================== */

VkCommandBuffer	beginSingleTimeCommands(
	VkDevice device,
	VkCommandPool command_pool
);

void	endSingleTimeCommands(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkCommandBuffer command_buffer
);

VkImageView	createImageView(
	VkDevice logical_device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags aspect_flags,
	uint32_t mip_level
);

void	copyBuffer(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkBuffer src_buffer,
	VkBuffer dst_buffer,
	VkDeviceSize size
);

void	copyBufferToImage(
	VkDevice device,
	VkQueue queue,
	VkCommandPool command_pool,
	VkBuffer buffer,
	VkImage image,
	uint32_t width,
	uint32_t height
);

} // namespace graphics
} // namespace scop

