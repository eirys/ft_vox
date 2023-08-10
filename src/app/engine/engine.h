/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/08/10 22:14:49 by etran            ###   ########.fr       */
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
# include "scene_render_pass.h"
# include "texture_handler.h"
# include "descriptor_set.h"
# include "command_pool.h"
# include "input_buffer.h"
# include "player.h"
# include "command_buffer.h"

namespace scop {
class Timer;
} // namespace scop

namespace scop::graphics {

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

	void						init(
		::scop::Window& window,
		const std::vector<TextureHandler::Texture>& images,
		const UniformBufferObject::Light& light,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices
	);
	void						destroy();

	void						idle();
	void						render(
		::scop::Window& window,
		const vox::Player& player,
		Timer& timer
	);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance					_vk_instance;
	DebugModule					_debug_module;

	Device						_device;
	SwapChain					_swap_chain;
	SceneRenderPass				_render_pass;
	TextureHandler				_texture_sampler;
	DescriptorSet				_descriptor_set;
	CommandPool					_command_pool;
	InputBuffer					_input_buffer;

	CommandBuffer				_main_command_buffer;

	VkSemaphore					_image_available_semaphores;
	VkSemaphore					_render_finished_semaphores;
	VkFence						_in_flight_fences;

	VkPipelineLayout			_pipeline_layout;
	struct {
		VkPipeline				scene;
		//VkPipeline				shadows;
	}							_pipelines;
	//VkPipeline					_pipeline;

	std::size_t					_nb_indices;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_createInstance();
	void						_createGraphicsPipelines();
	void						_createSyncObjects();

	void						_createGraphicsPipelineLayout();
	void						_createScenePipeline(
		VkGraphicsPipelineCreateInfo& info
	);
	void						_createShadowPipeline(
		VkGraphicsPipelineCreateInfo& info
	);
	bool						_checkValidationLayerSupport();
	std::vector<const char*>	_getRequiredExtensions();
	VkShaderModule				_createShaderModule(const std::string& path);
	void						_recordDrawingCommand(
		std::size_t indices_size,
		uint32_t image_index
	);

}; // class Engine

} // namespace scop::graphics