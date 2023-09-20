/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <etran@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 17:14:35 by etran             #+#    #+#             */
/*   Updated: 2023/08/15 19:34:30 by etran            ###   ########.fr       */
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
# include "descriptor_pool.h"
# include "command_pool.h"
# include "input_handler.h"
# include "command_buffer.h"
# include "texture_handler.h"
# include "pipeline.h"

namespace vox {
class GameState;
}

namespace scop {
class Timer;
struct Camera;
}


namespace scop::graphics {

class Engine {
public:
	/* ========================================================================= */
	/*                                  TYPEDEFS                                 */
	/* ========================================================================= */

	using Texture = TextureHandler::Texture;
	using PipelinePtr = std::shared_ptr<Pipeline>;

	using UniformBufferObject = ::scop::UniformBufferObject;
	using GameState = ::vox::GameState;
	using Window = ::scop::Window;

	/* ========================================================================= */
	/*                               CONST MEMBERS                               */
	/* ========================================================================= */

	static const std::vector<const char*>	validation_layers;
	static constexpr std::size_t	max_frames_in_flight = 1;

	#ifdef NDEBUG
	static constexpr bool			enable_validation_layers = true;
	#else
	static constexpr bool			enable_validation_layers = false;
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
		Window& window,
		const GameState& game,
		const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices);
	void						destroy();

	void						idle();
	void						render(
		Window& window,
		const vox::GameState& game,
		Timer& timer);

private:
	/* ========================================================================= */
	/*                               CLASS MEMBERS                               */
	/* ========================================================================= */

	VkInstance					_vk_instance;
	DebugModule					_debug_module;

	Device						_device;
	SwapChain					_swap_chain;
	DescriptorPool				_descriptor_pool;
	CommandPool					_command_pool;
	InputHandler				_input_handler;

	CommandBuffer				_draw_buffer;

	VkSemaphore					_image_available_semaphores;
	VkSemaphore					_render_finished_semaphores;
	VkFence						_in_flight_fences;

	VkPipelineLayout			_pipeline_layout;
	struct {
		PipelinePtr				scene;
		PipelinePtr				shadows;
	}							_pipelines;

	/* ========================================================================= */
	/*                                  METHODS                                  */
	/* ========================================================================= */

	void						_createInstance();
	void						_createGraphicsPipelines();
	void						_assembleGraphicsPipelines();
	void						_createGraphicsPipelineLayout();
	void						_createSyncObjects();
	void						_createDescriptors();

	void						_initDescriptors(const GameState& game) noexcept;
	// UniformBufferObject			_updateUbo(const GameState& game) const noexcept;
	Camera						_generateCameraMatrix(const GameState& game) const noexcept;

	void						_updatePresentation(Window& window);

	bool						_checkValidationLayerSupport();
	std::vector<const char*>	_getRequiredExtensions();

}; // class Engine

} // namespace scop::graphics