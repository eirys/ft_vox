/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/07/21 21:04:28 by etran            ###   ########.fr       */
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
# include "swap_chain.h"
# include "render_pass.h"
# include "texture_sampler.h"
# include "descriptor_set.h"
# include "command_pool.h"
# include "input_buffer.h"
# include "player.h"
# include "command_buffer.h"

# define FRAG_SHADER_BIN "shaders/frag.spv"
# define VERT_SHADER_BIN "shaders/vert.spv"

namespace scop {
class Timer;

namespace graphics {

class Engine {
public:
	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static const std::vector<const char*>	validation_layers;
	static constexpr std::size_t	max_frames_in_flight = 1;

	#ifndef NDEBUG
	static constexpr bool			enable_validation_layers = false;
	#else
	static constexpr bool			enable_validation_layers = true;
	#endif

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	Engine() = default;
	~Engine() = default;

	Engine(Engine&& other) = delete;
	Engine(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;

	/* ========================================================================= */

	void							init(
		scop::Window& window,
		const std::vector<TextureSampler::Texture>& images,
		const UniformBufferObject::Light& light,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void							destroy();

	void							idle();
	void							render(
		scop::Window& window,
		const vox::Player& player,
		Timer& timer
	);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance						_vk_instance;
	DebugModule						_debug_module;

	Device							_device;

	SwapChain						_swap_chain;
	RenderPass						_render_pass;
	TextureSampler					_texture_sampler;
	DescriptorSet					_descriptor_set;
	CommandPool						_command_pool;
	InputBuffer						_input_buffer;

	CommandBuffer					_main_command_buffer;

	VkSemaphore						_image_available_semaphores;
	VkSemaphore						_render_finished_semaphores;
	VkFence							_in_flight_fences;

	VkPipelineLayout				_pipeline_layout;
	VkPipeline						_pipeline;

	std::size_t						_nb_indices;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void							_createInstance();
	void							_createGraphicsPipeline();
	void							_createSyncObjects();

	bool							_checkValidationLayerSupport();
	std::vector<const char*>		_getRequiredExtensions();
	VkShaderModule					_createShaderModule(const std::string& path);
	void							_recordDrawingCommand(
		std::size_t indices_size,
		uint32_t image_index
	);

}; // class Engine

} // namespace graphics
} // namespace scop

